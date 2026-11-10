// SPDX-License-Identifier: GPL-3.0-or-later
// EctoPad's GameCube memory-card library. Independent implementation written for
// EctoPad from the public GameCube card format; it keeps the kabufuda:: names
// and signatures Metaforce calls so it can replace AxioDL's kabufuda, which has
// no license. See components/kabufuda/README.md.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace kabufuda {

enum class ECardResult {
  CRC_MISMATCH = -1003, // Metroid Prime's own save CRC check
  FATAL_ERROR = -128,
  ENCODING = -13,
  NAMETOOLONG = -12,
  INSSPACE = -9,
  NOENT = -8,
  EXIST = -7,
  BROKEN = -6,
  IOERROR = -5,
  NOFILE = -4,
  NOCARD = -3,
  WRONGDEVICE = -2,
  BUSY = -1,
  READY = 0,
};

enum class EPermissions : uint8_t { Public = 1 << 2, NoCopy = 1 << 3, NoMove = 1 << 4, Global = 1 << 5, Company = 1 << 6 };
enum class EImageFormat : uint8_t { None, C8, RGB5A3 };
enum class EAnimationType { Loop = 0, Bounce = 2 };
enum class EAnimationSpeed { End, Fast, Middle, Slow };
enum class SeekOrigin { Begin, Current, End };
enum class ECardSlot : uint16_t { SlotA, SlotB };
enum class ECardSize : uint16_t {
  Card59Mb = 0x04,
  Card123Mb = 0x08,
  Card251Mb = 0x10,
  Card507Mb = 0x20,
  Card1019Mb = 0x40,
  Card2043Mb = 0x80,
};
enum class EEncoding : uint16_t { ASCII, SJIS };

constexpr uint32_t BlockSize = 0x2000;
constexpr uint32_t MaxFiles = 127;

class FileHandle {
  friend class Card;
  uint32_t m_fileNo = UINT32_MAX;
  uint32_t m_offset = 0;

public:
  FileHandle() = default;
  uint32_t getFileNo() const { return m_fileNo; }
  explicit operator bool() const { return m_fileNo != UINT32_MAX; }
};

struct ProbeResults {
  ECardResult x0_error;
  uint32_t x4_cardSize;   // megabits
  uint32_t x8_sectorSize; // bytes
};

// File status, as the GameCube SDK's CARDStat exposes it.
struct CardStat {
  std::array<char, 32> x0_fileName{};
  uint32_t x20_length = 0;
  uint32_t x24_time = 0; // seconds since 2000-01-01
  std::array<uint8_t, 4> x28_gameName{};
  std::array<uint8_t, 2> x2c_company{};
  uint8_t x2e_bannerFormat = 0;
  uint32_t x30_iconAddr = 0xFFFFFFFF;
  uint16_t x34_iconFormat = 0;
  uint16_t x36_iconSpeed = 0;
  uint32_t x38_commentAddr = 0xFFFFFFFF;

  const char* GetFileName() const { return x0_fileName.data(); }
  uint32_t GetFileLength() const { return x20_length; }
  uint32_t GetTime() const { return x24_time; }
  EImageFormat GetBannerFormat() const { return EImageFormat(x2e_bannerFormat & 3); }
  void SetBannerFormat(EImageFormat fmt) { x2e_bannerFormat = uint8_t((x2e_bannerFormat & ~3) | uint8_t(fmt)); }
  EImageFormat GetIconFormat(int idx) const { return EImageFormat((x34_iconFormat >> (2 * idx)) & 3); }
  void SetIconFormat(EImageFormat fmt, int idx) {
    x34_iconFormat = uint16_t((x34_iconFormat & ~(3u << (2 * idx))) | (uint16_t(fmt) << (2 * idx)));
  }
  EAnimationSpeed GetIconSpeed(int idx) const { return EAnimationSpeed((x36_iconSpeed >> (2 * idx)) & 3); }
  void SetIconSpeed(EAnimationSpeed speed, int idx) {
    x36_iconSpeed = uint16_t((x36_iconSpeed & ~(3u << (2 * idx))) | (uint16_t(speed) << (2 * idx)));
  }
  uint32_t GetIconAddr() const { return x30_iconAddr; }
  void SetIconAddr(uint32_t addr) { x30_iconAddr = addr; }
  uint32_t GetCommentAddr() const { return x38_commentAddr; }
  void SetCommentAddr(uint32_t addr) { x38_commentAddr = addr; }
};

// A memory-card image file (Dolphin's raw .raw format). Operations complete
// synchronously, so getError() never reports BUSY.
class Card {
public:
  explicit Card(const char game[4] = nullptr, const char maker[2] = nullptr);
  ~Card();
  Card(Card&& other) noexcept;
  Card& operator=(Card&& other) noexcept;
  Card(const Card&) = delete;
  Card& operator=(const Card&) = delete;

  // Opens (or creates) the image. Returns false only if the file cannot be
  // opened; a damaged or empty image opens with getError() != READY.
  bool open(std::string_view path);
  void close();
  void commit();
  void waitForCompletion() const {}
  ECardResult getError() const { return m_error; }
  explicit operator bool() const { return m_error == ECardResult::READY; }

  ECardResult openFile(const char* filename, FileHandle& handleOut);
  ECardResult openFile(uint32_t fileNo, FileHandle& handleOut);
  ECardResult createFile(const char* filename, size_t size, FileHandle& handleOut);
  ECardResult closeFile(FileHandle& fh);
  void seek(FileHandle& fh, int32_t pos, SeekOrigin whence);
  int32_t tell(const FileHandle& fh) const { return int32_t(fh.m_offset); }
  ECardResult asyncWrite(FileHandle& fh, const void* buf, size_t size);
  ECardResult asyncRead(FileHandle& fh, void* dst, size_t size);
  ECardResult deleteFile(const char* filename);
  ECardResult deleteFile(uint32_t fileNo);
  ECardResult renameFile(const char* oldName, const char* newName);
  ECardResult getStatus(uint32_t fileNo, CardStat& statOut) const;
  ECardResult getStatus(const FileHandle& fh, CardStat& statOut) const { return getStatus(fh.m_fileNo, statOut); }
  ECardResult setStatus(uint32_t fileNo, const CardStat& stat);
  ECardResult setStatus(const FileHandle& fh, const CardStat& stat) { return setStatus(fh.m_fileNo, stat); }

  void getSerial(uint64_t& serial) const;
  void getFreeBlocks(int32_t& bytesNotUsed, int32_t& filesNotUsed) const;
  uint32_t fileCount() const;

  void format(ECardSlot slot, ECardSize size = ECardSize::Card2043Mb, EEncoding encoding = EEncoding::ASCII);
  static ProbeResults probeCardFile(std::string_view filename);

private:
  std::array<char, 4> m_game{};
  std::array<char, 2> m_maker{};
  std::string m_path;
  int m_fd = -1;
  std::vector<uint8_t> m_image;
  int m_dirCopy = 0; // 0 -> block 1, 1 -> block 2
  int m_batCopy = 0; // 0 -> block 3, 1 -> block 4
  ECardResult m_error = ECardResult::NOCARD;

  uint32_t totalBlocks() const { return uint32_t(m_image.size() / BlockSize); }
  uint8_t* block(uint32_t index) { return m_image.data() + size_t(index) * BlockSize; }
  const uint8_t* block(uint32_t index) const { return m_image.data() + size_t(index) * BlockSize; }
  uint8_t* dir() { return block(1 + m_dirCopy); }
  const uint8_t* dir() const { return block(1 + m_dirCopy); }
  uint8_t* bat() { return block(3 + m_batCopy); }
  const uint8_t* bat() const { return block(3 + m_batCopy); }
  uint8_t* entry(uint32_t fileNo) { return dir() + fileNo * 0x40; }
  const uint8_t* entry(uint32_t fileNo) const { return dir() + fileNo * 0x40; }
  bool entryUsed(uint32_t fileNo) const;
  bool entryIsOurs(uint32_t fileNo) const;
  int32_t findFile(const char* filename) const;
  uint16_t batNext(uint32_t blockIndex) const;
  void setBatNext(uint32_t blockIndex, uint16_t next);
  bool blockForOffset(uint32_t fileNo, uint32_t offset, uint32_t& blockOut) const;
  void loadImage();
  void writeBlocks(uint32_t first, uint32_t count);
  void flushMetadata(bool dirChanged, bool batChanged);
};

} // namespace kabufuda
