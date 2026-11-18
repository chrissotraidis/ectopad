// SPDX-License-Identifier: GPL-3.0-or-later
// EctoPad's GameCube memory-card library; see components/kabufuda/README.md.
//
// Card layout (8 KiB blocks):
//   block 0      header: serial, format time, size in megabits, encoding,
//                update counter and checksum pair at 0x1FA..0x1FF
//   blocks 1, 2  directory copies: 127 entries of 0x40 bytes, update counter
//                at 0x1FFA and checksum pair at 0x1FFC..0x1FFF
//   blocks 3, 4  block allocation table copies: checksum pair at 0x0000,
//                update counter 0x0004, free blocks 0x0006, last allocated
//                0x0008, then one 16-bit link per block from block 5
//   blocks 5..   file data, chained through the allocation table
// All multi-byte fields are big-endian. Of each copied pair, the valid copy
// with the newer update counter is current; changes are written to the other.
#include "kabufuda/Card.hpp"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace kabufuda {
namespace {

constexpr uint32_t SystemBlocks = 5;
constexpr uint32_t MegabitBytes = 1024 * 1024 / 8;
constexpr uint16_t BatEnd = 0xFFFF;
constexpr uint16_t BatFree = 0x0000;
constexpr int64_t Epoch2000 = 946684800; // 2000-01-01T00:00:00Z in Unix time

uint16_t rd16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }
uint32_t rd32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }
uint64_t rd64(const uint8_t* p) { return uint64_t(rd32(p)) << 32 | rd32(p + 4); }
void wr16(uint8_t* p, uint16_t v) { p[0] = uint8_t(v >> 8); p[1] = uint8_t(v); }
void wr32(uint8_t* p, uint32_t v) {
  p[0] = uint8_t(v >> 24);
  p[1] = uint8_t(v >> 16);
  p[2] = uint8_t(v >> 8);
  p[3] = uint8_t(v);
}
void wr64(uint8_t* p, uint64_t v) {
  wr32(p, uint32_t(v >> 32));
  wr32(p + 4, uint32_t(v));
}

// Sum and inverted sum of big-endian 16-bit words; an all-ones result is
// stored as zero.
void checksum(const uint8_t* p, size_t len, uint16_t& sum, uint16_t& inv) {
  sum = 0;
  inv = 0;
  for (size_t i = 0; i + 1 < len; i += 2) {
    const uint16_t w = rd16(p + i);
    sum = uint16_t(sum + w);
    inv = uint16_t(inv + uint16_t(~w));
  }
  if (sum == 0xFFFF)
    sum = 0;
  if (inv == 0xFFFF)
    inv = 0;
}

// Checked region, offset of the stored pair, per system block kind.
bool headerValid(const uint8_t* b) {
  uint16_t s, i;
  checksum(b, 0x1FC, s, i);
  return s == rd16(b + 0x1FC) && i == rd16(b + 0x1FE);
}
void sealHeader(uint8_t* b) {
  uint16_t s, i;
  checksum(b, 0x1FC, s, i);
  wr16(b + 0x1FC, s);
  wr16(b + 0x1FE, i);
}
bool dirValid(const uint8_t* b) {
  uint16_t s, i;
  checksum(b, 0x1FFC, s, i);
  return s == rd16(b + 0x1FFC) && i == rd16(b + 0x1FFE);
}
void sealDir(uint8_t* b) {
  uint16_t s, i;
  checksum(b, 0x1FFC, s, i);
  wr16(b + 0x1FFC, s);
  wr16(b + 0x1FFE, i);
}
bool batValid(const uint8_t* b) {
  uint16_t s, i;
  checksum(b + 4, BlockSize - 4, s, i);
  return s == rd16(b) && i == rd16(b + 2);
}
void sealBat(uint8_t* b) {
  uint16_t s, i;
  checksum(b + 4, BlockSize - 4, s, i);
  wr16(b, s);
  wr16(b + 2, i);
}

// Picks the current copy of a pair: the valid one, or the newer valid one.
int pickCopy(bool valid0, bool valid1, uint16_t counter0, uint16_t counter1) {
  if (valid0 && valid1)
    return int16_t(counter1 - counter0) > 0 ? 1 : 0;
  if (valid1)
    return 1;
  if (valid0)
    return 0;
  return -1;
}

uint32_t nowSince2000() {
  const int64_t t = int64_t(std::time(nullptr)) - Epoch2000;
  return t > 0 ? uint32_t(t) : 0;
}

bool namesEqual(const uint8_t* stored, const char* name) {
  const size_t len = std::strlen(name);
  if (len > 32)
    return false;
  if (std::memcmp(stored, name, len) != 0)
    return false;
  return len == 32 || stored[len] == 0;
}

// Directory entry fields.
constexpr size_t E_GAME = 0x00, E_MAKER = 0x04, E_UNUSED = 0x06, E_BANNER = 0x07, E_NAME = 0x08, E_TIME = 0x28,
                 E_ICONADDR = 0x2C, E_ICONFMT = 0x30, E_ANIMSPEED = 0x32, E_PERM = 0x34, E_COPY = 0x35,
                 E_FIRST = 0x36, E_COUNT = 0x38, E_UNUSED2 = 0x3A, E_COMMENT = 0x3C;

} // namespace

Card::Card(const char game[4], const char maker[2]) {
  if (game)
    std::memcpy(m_game.data(), game, 4);
  if (maker)
    std::memcpy(m_maker.data(), maker, 2);
}

Card::~Card() { close(); }

Card::Card(Card&& other) noexcept { *this = std::move(other); }

Card& Card::operator=(Card&& other) noexcept {
  if (this != &other) {
    close();
    m_game = other.m_game;
    m_maker = other.m_maker;
    m_path = std::move(other.m_path);
    m_fd = other.m_fd;
    m_image = std::move(other.m_image);
    m_dirCopy = other.m_dirCopy;
    m_batCopy = other.m_batCopy;
    m_error = other.m_error;
    other.m_fd = -1;
    other.m_error = ECardResult::NOCARD;
  }
  return *this;
}

bool Card::open(std::string_view path) {
  close();
  m_path = std::string(path);
  m_fd = ::open(m_path.c_str(), O_RDWR | O_CREAT, 0644);
  if (m_fd < 0) {
    m_error = ECardResult::NOCARD;
    return false;
  }
  loadImage();
  return true;
}

void Card::close() {
  if (m_fd >= 0) {
    if (m_error == ECardResult::READY)
      ::fsync(m_fd);
    ::close(m_fd);
  }
  m_fd = -1;
  m_image.clear();
  m_error = ECardResult::NOCARD;
}

void Card::loadImage() {
  struct stat st {};
  if (::fstat(m_fd, &st) != 0) {
    m_error = ECardResult::IOERROR;
    return;
  }
  const size_t size = size_t(st.st_size);
  if (size < SystemBlocks * BlockSize || size % BlockSize != 0) {
    m_error = ECardResult::BROKEN;
    return;
  }
  m_image.resize(size);
  size_t done = 0;
  while (done < size) {
    const ssize_t n = ::pread(m_fd, m_image.data() + done, size - done, off_t(done));
    if (n < 0 && errno == EINTR)
      continue;
    if (n <= 0) {
      m_error = ECardResult::IOERROR;
      return;
    }
    done += size_t(n);
  }
  const uint8_t* header = block(0);
  if (!headerValid(header) || size_t(rd16(header + 0x22)) * MegabitBytes != size) {
    m_error = ECardResult::BROKEN;
    return;
  }
  const uint16_t encoding = rd16(header + 0x24);
  if (encoding != uint16_t(EEncoding::ASCII) && encoding != uint16_t(EEncoding::SJIS)) {
    m_error = ECardResult::ENCODING;
    return;
  }
  const int dirCopy = pickCopy(dirValid(block(1)), dirValid(block(2)), rd16(block(1) + 0x1FFA), rd16(block(2) + 0x1FFA));
  const int batCopy = pickCopy(batValid(block(3)), batValid(block(4)), rd16(block(3) + 4), rd16(block(4) + 4));
  if (dirCopy < 0 || batCopy < 0) {
    m_error = ECardResult::BROKEN;
    return;
  }
  m_dirCopy = dirCopy;
  m_batCopy = batCopy;
  m_error = ECardResult::READY;
}

void Card::writeBlocks(uint32_t first, uint32_t count) {
  const size_t offset = size_t(first) * BlockSize;
  const size_t length = size_t(count) * BlockSize;
  size_t done = 0;
  while (done < length) {
    const ssize_t n = ::pwrite(m_fd, m_image.data() + offset + done, length - done, off_t(offset + done));
    if (n < 0 && errno == EINTR)
      continue;
    if (n <= 0) {
      m_error = ECardResult::IOERROR;
      return;
    }
    done += size_t(n);
  }
}

void Card::flushMetadata(bool dirChanged, bool batChanged) {
  if (dirChanged) {
    const int next = m_dirCopy ^ 1;
    uint8_t* dst = block(1 + next);
    std::memcpy(dst, dir(), BlockSize);
    wr16(dst + 0x1FFA, uint16_t(rd16(dst + 0x1FFA) + 1));
    sealDir(dst);
    m_dirCopy = next;
    writeBlocks(1 + next, 1);
  }
  if (batChanged) {
    const int next = m_batCopy ^ 1;
    uint8_t* dst = block(3 + next);
    std::memcpy(dst, bat(), BlockSize);
    wr16(dst + 4, uint16_t(rd16(dst + 4) + 1));
    sealBat(dst);
    m_batCopy = next;
    writeBlocks(3 + next, 1);
  }
}

void Card::commit() {
  if (m_fd >= 0)
    ::fsync(m_fd);
}

bool Card::entryUsed(uint32_t fileNo) const { return fileNo < MaxFiles && rd32(entry(fileNo) + E_GAME) != 0xFFFFFFFF; }

bool Card::entryIsOurs(uint32_t fileNo) const {
  return entryUsed(fileNo) && std::memcmp(entry(fileNo) + E_GAME, m_game.data(), 4) == 0 &&
         std::memcmp(entry(fileNo) + E_MAKER, m_maker.data(), 2) == 0;
}

int32_t Card::findFile(const char* filename) const {
  for (uint32_t i = 0; i < MaxFiles; ++i)
    if (entryIsOurs(i) && namesEqual(entry(i) + E_NAME, filename))
      return int32_t(i);
  return -1;
}

uint16_t Card::batNext(uint32_t blockIndex) const { return rd16(bat() + 0x0A + (blockIndex - SystemBlocks) * 2); }

void Card::setBatNext(uint32_t blockIndex, uint16_t next) { wr16(bat() + 0x0A + (blockIndex - SystemBlocks) * 2, next); }

bool Card::blockForOffset(uint32_t fileNo, uint32_t offset, uint32_t& blockOut) const {
  const uint8_t* e = entry(fileNo);
  uint32_t b = rd16(e + E_FIRST);
  for (uint32_t skip = offset / BlockSize; skip > 0; --skip) {
    if (b < SystemBlocks || b >= totalBlocks())
      return false;
    b = batNext(b);
  }
  if (b < SystemBlocks || b >= totalBlocks())
    return false;
  blockOut = b;
  return true;
}

ECardResult Card::openFile(const char* filename, FileHandle& handleOut) {
  if (m_error != ECardResult::READY)
    return m_error;
  const int32_t fileNo = findFile(filename);
  if (fileNo < 0)
    return ECardResult::NOFILE;
  handleOut.m_fileNo = uint32_t(fileNo);
  handleOut.m_offset = 0;
  return ECardResult::READY;
}

ECardResult Card::openFile(uint32_t fileNo, FileHandle& handleOut) {
  if (m_error != ECardResult::READY)
    return m_error;
  if (!entryUsed(fileNo))
    return ECardResult::NOFILE;
  handleOut.m_fileNo = fileNo;
  handleOut.m_offset = 0;
  return ECardResult::READY;
}

ECardResult Card::createFile(const char* filename, size_t size, FileHandle& handleOut) {
  if (m_error != ECardResult::READY)
    return m_error;
  if (std::strlen(filename) > 32)
    return ECardResult::NAMETOOLONG;
  if (findFile(filename) >= 0)
    return ECardResult::EXIST;
  uint32_t fileNo = MaxFiles;
  for (uint32_t i = 0; i < MaxFiles; ++i) {
    if (!entryUsed(i)) {
      fileNo = i;
      break;
    }
  }
  if (fileNo == MaxFiles)
    return ECardResult::NOENT;
  const uint32_t needed = uint32_t((size + BlockSize - 1) / BlockSize);
  if (needed == 0 || needed > rd16(bat() + 6))
    return ECardResult::INSSPACE;

  // Allocate a chain, searching forward from the last allocated block.
  std::vector<uint16_t> chain;
  chain.reserve(needed);
  uint32_t cursor = rd16(bat() + 8);
  const uint32_t total = totalBlocks();
  for (uint32_t scanned = 0; chain.size() < needed && scanned < total - SystemBlocks; ++scanned) {
    cursor = cursor + 1 >= total || cursor + 1 < SystemBlocks ? SystemBlocks : cursor + 1;
    if (batNext(cursor) == BatFree)
      chain.push_back(uint16_t(cursor));
  }
  if (chain.size() < needed)
    return ECardResult::INSSPACE;
  for (size_t i = 0; i < chain.size(); ++i)
    setBatNext(chain[i], i + 1 < chain.size() ? chain[i + 1] : BatEnd);
  wr16(bat() + 6, uint16_t(rd16(bat() + 6) - needed));
  wr16(bat() + 8, chain.back());

  uint8_t* e = entry(fileNo);
  std::memset(e, 0, 0x40);
  std::memcpy(e + E_GAME, m_game.data(), 4);
  std::memcpy(e + E_MAKER, m_maker.data(), 2);
  e[E_UNUSED] = 0xFF;
  e[E_BANNER] = 0;
  std::memcpy(e + E_NAME, filename, std::strlen(filename));
  wr32(e + E_TIME, nowSince2000());
  wr32(e + E_ICONADDR, 0xFFFFFFFF);
  e[E_PERM] = uint8_t(EPermissions::Public);
  e[E_COPY] = 0;
  wr16(e + E_FIRST, chain.front());
  wr16(e + E_COUNT, uint16_t(needed));
  wr16(e + E_UNUSED2, 0xFFFF);
  wr32(e + E_COMMENT, 0xFFFFFFFF);

  flushMetadata(true, true);
  handleOut.m_fileNo = fileNo;
  handleOut.m_offset = 0;
  return m_error;
}

ECardResult Card::closeFile(FileHandle& fh) {
  // Metroid Prime reads a file's status through its handle after closing it
  // (to compare save times), so closing keeps the file number.
  fh.m_offset = 0;
  return ECardResult::READY;
}

void Card::seek(FileHandle& fh, int32_t pos, SeekOrigin whence) {
  if (!entryUsed(fh.m_fileNo))
    return;
  const int64_t length = int64_t(rd16(entry(fh.m_fileNo) + E_COUNT)) * BlockSize;
  int64_t base = 0;
  if (whence == SeekOrigin::Current)
    base = fh.m_offset;
  else if (whence == SeekOrigin::End)
    base = length;
  fh.m_offset = uint32_t(std::clamp<int64_t>(base + pos, 0, length));
}

ECardResult Card::asyncWrite(FileHandle& fh, const void* buf, size_t size) {
  if (m_error != ECardResult::READY)
    return m_error;
  if (!entryUsed(fh.m_fileNo))
    return ECardResult::NOFILE;
  const auto* src = static_cast<const uint8_t*>(buf);
  while (size > 0) {
    uint32_t b;
    if (!blockForOffset(fh.m_fileNo, fh.m_offset, b))
      return ECardResult::BROKEN;
    const uint32_t within = fh.m_offset % BlockSize;
    const size_t chunk = std::min<size_t>(size, BlockSize - within);
    std::memcpy(block(b) + within, src, chunk);
    writeBlocks(b, 1);
    if (m_error != ECardResult::READY)
      return m_error;
    src += chunk;
    size -= chunk;
    fh.m_offset += uint32_t(chunk);
  }
  return ECardResult::READY;
}

ECardResult Card::asyncRead(FileHandle& fh, void* dst, size_t size) {
  if (m_error != ECardResult::READY)
    return m_error;
  if (!entryUsed(fh.m_fileNo))
    return ECardResult::NOFILE;
  auto* out = static_cast<uint8_t*>(dst);
  while (size > 0) {
    uint32_t b;
    if (!blockForOffset(fh.m_fileNo, fh.m_offset, b))
      return ECardResult::BROKEN;
    const uint32_t within = fh.m_offset % BlockSize;
    const size_t chunk = std::min<size_t>(size, BlockSize - within);
    std::memcpy(out, block(b) + within, chunk);
    out += chunk;
    size -= chunk;
    fh.m_offset += uint32_t(chunk);
  }
  return ECardResult::READY;
}

ECardResult Card::deleteFile(uint32_t fileNo) {
  if (m_error != ECardResult::READY)
    return m_error;
  if (!entryUsed(fileNo))
    return ECardResult::NOFILE;
  uint8_t* e = entry(fileNo);
  uint32_t b = rd16(e + E_FIRST);
  uint32_t freed = 0;
  for (uint32_t guard = 0; b >= SystemBlocks && b < totalBlocks() && guard < totalBlocks(); ++guard) {
    const uint16_t next = batNext(b);
    setBatNext(b, BatFree);
    ++freed;
    if (next == BatEnd)
      break;
    b = next;
  }
  wr16(bat() + 6, uint16_t(rd16(bat() + 6) + freed));
  std::memset(e, 0xFF, 0x40);
  flushMetadata(true, true);
  return m_error;
}

ECardResult Card::deleteFile(const char* filename) {
  if (m_error != ECardResult::READY)
    return m_error;
  const int32_t fileNo = findFile(filename);
  return fileNo < 0 ? ECardResult::NOFILE : deleteFile(uint32_t(fileNo));
}

ECardResult Card::renameFile(const char* oldName, const char* newName) {
  if (m_error != ECardResult::READY)
    return m_error;
  if (std::strlen(newName) > 32)
    return ECardResult::NAMETOOLONG;
  const int32_t fileNo = findFile(oldName);
  if (fileNo < 0)
    return ECardResult::NOFILE;
  const int32_t clash = findFile(newName);
  if (clash >= 0 && clash != fileNo)
    return ECardResult::EXIST;
  uint8_t* e = entry(uint32_t(fileNo));
  std::memset(e + E_NAME, 0, 32);
  std::memcpy(e + E_NAME, newName, std::strlen(newName));
  wr32(e + E_TIME, nowSince2000());
  flushMetadata(true, false);
  return m_error;
}

ECardResult Card::getStatus(uint32_t fileNo, CardStat& statOut) const {
  if (m_error != ECardResult::READY)
    return m_error;
  if (!entryUsed(fileNo))
    return ECardResult::NOFILE;
  const uint8_t* e = entry(fileNo);
  statOut = CardStat{};
  std::memcpy(statOut.x0_fileName.data(), e + E_NAME, 32);
  statOut.x20_length = uint32_t(rd16(e + E_COUNT)) * BlockSize;
  statOut.x24_time = rd32(e + E_TIME);
  std::memcpy(statOut.x28_gameName.data(), e + E_GAME, 4);
  std::memcpy(statOut.x2c_company.data(), e + E_MAKER, 2);
  statOut.x2e_bannerFormat = e[E_BANNER];
  statOut.x30_iconAddr = rd32(e + E_ICONADDR);
  statOut.x34_iconFormat = rd16(e + E_ICONFMT);
  statOut.x36_iconSpeed = rd16(e + E_ANIMSPEED);
  statOut.x38_commentAddr = rd32(e + E_COMMENT);
  return ECardResult::READY;
}

ECardResult Card::setStatus(uint32_t fileNo, const CardStat& stat) {
  if (m_error != ECardResult::READY)
    return m_error;
  if (!entryUsed(fileNo))
    return ECardResult::NOFILE;
  uint8_t* e = entry(fileNo);
  e[E_BANNER] = uint8_t((e[E_BANNER] & ~3) | (stat.x2e_bannerFormat & 3));
  wr32(e + E_ICONADDR, stat.x30_iconAddr);
  wr16(e + E_ICONFMT, stat.x34_iconFormat);
  wr16(e + E_ANIMSPEED, stat.x36_iconSpeed);
  wr32(e + E_COMMENT, stat.x38_commentAddr);
  wr32(e + E_TIME, nowSince2000());
  flushMetadata(true, false);
  return m_error;
}

void Card::getSerial(uint64_t& serial) const {
  serial = 0;
  if (m_image.size() < BlockSize)
    return;
  for (int i = 0; i < 4; ++i)
    serial ^= rd64(block(0) + i * 8);
}

void Card::getFreeBlocks(int32_t& bytesNotUsed, int32_t& filesNotUsed) const {
  bytesNotUsed = 0;
  filesNotUsed = 0;
  if (m_error != ECardResult::READY)
    return;
  bytesNotUsed = int32_t(rd16(bat() + 6)) * int32_t(BlockSize);
  for (uint32_t i = 0; i < MaxFiles; ++i)
    if (!entryUsed(i))
      ++filesNotUsed;
}

uint32_t Card::fileCount() const {
  uint32_t n = 0;
  if (m_error == ECardResult::READY)
    for (uint32_t i = 0; i < MaxFiles; ++i)
      if (entryUsed(i))
        ++n;
  return n;
}

void Card::format(ECardSlot slot, ECardSize size, EEncoding encoding) {
  (void)slot;
  if (m_fd < 0) {
    m_error = ECardResult::NOCARD;
    return;
  }
  const size_t bytes = size_t(size) * MegabitBytes;
  const uint32_t blocks = uint32_t(bytes / BlockSize);
  m_image.assign(bytes, 0x00);

  uint8_t* header = block(0);
  std::memset(header, 0xFF, BlockSize);
  const uint64_t formatTime = (uint64_t(std::time(nullptr)) << 16) ^ uint64_t(::getpid());
  for (int i = 0; i < 12; ++i) // card serial: any stable per-format value
    header[i] = uint8_t(formatTime >> ((i % 8) * 8)) ^ uint8_t(0x5A + i * 17);
  wr64(header + 0x0C, formatTime);
  wr32(header + 0x14, 0);      // SRAM bias
  wr32(header + 0x18, 0);      // SRAM language
  wr32(header + 0x1C, 0);
  wr16(header + 0x20, 0);      // device id
  wr16(header + 0x22, uint16_t(size));
  wr16(header + 0x24, uint16_t(encoding));
  wr16(header + 0x1FA, 0);
  sealHeader(header);

  for (int copy = 0; copy < 2; ++copy) {
    uint8_t* d = block(1 + copy);
    std::memset(d, 0xFF, BlockSize);
    wr16(d + 0x1FFA, uint16_t(copy));
    sealDir(d);
    uint8_t* t = block(3 + copy);
    std::memset(t, 0x00, BlockSize);
    wr16(t + 4, uint16_t(copy));
    wr16(t + 6, uint16_t(blocks - SystemBlocks));
    wr16(t + 8, uint16_t(SystemBlocks - 1));
    sealBat(t);
  }
  m_dirCopy = 1;
  m_batCopy = 1;

  if (::ftruncate(m_fd, off_t(bytes)) != 0) {
    m_error = ECardResult::IOERROR;
    return;
  }
  m_error = ECardResult::READY;
  writeBlocks(0, blocks);
  if (m_error == ECardResult::READY)
    ::fsync(m_fd);
}

ProbeResults Card::probeCardFile(std::string_view filename) {
  struct stat st {};
  if (::stat(std::string(filename).c_str(), &st) != 0)
    return {ECardResult::NOCARD, 0, 0};
  return {ECardResult::READY, uint32_t(uint64_t(st.st_size) / MegabitBytes), BlockSize};
}

} // namespace kabufuda
