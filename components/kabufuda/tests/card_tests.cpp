// SPDX-License-Identifier: GPL-3.0-or-later
// Round-trip tests for EctoPad's memory-card library, plus a directory dump of
// an existing image (pass its path) for comparison against other readers.
#include "kabufuda/Card.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <vector>

using namespace kabufuda;

static int failures = 0;
#define CHECK(cond)                                                                                                    \
  do {                                                                                                                 \
    if (!(cond)) {                                                                                                     \
      std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);                                             \
      ++failures;                                                                                                      \
    }                                                                                                                  \
  } while (0)

static uint64_t fnv(const std::vector<uint8_t>& v) {
  uint64_t h = 1469598103934665603ull;
  for (uint8_t b : v)
    h = (h ^ b) * 1099511628211ull;
  return h;
}

static int dump(const char* path) {
  // Work on a private copy so the image is never modified.
  std::string copy = std::string("/tmp/ectopad-card-dump-") + std::to_string(getpid()) + ".raw";
  {
    FILE* in = std::fopen(path, "rb");
    FILE* out = std::fopen(copy.c_str(), "wb");
    if (!in || !out)
      return 2;
    std::vector<char> buf(1 << 16);
    size_t n;
    while ((n = std::fread(buf.data(), 1, buf.size(), in)) > 0)
      std::fwrite(buf.data(), 1, n, out);
    std::fclose(in);
    std::fclose(out);
  }
  ProbeResults probe = Card::probeCardFile(copy);
  std::printf("probe error=%d size=%u sector=%u\n", int(probe.x0_error), probe.x4_cardSize, probe.x8_sectorSize);
  Card card{"GM8E", "01"};
  card.open(copy);
  std::printf("open error=%d\n", int(card.getError()));
  int32_t freeBytes = 0, freeFiles = 0;
  card.getFreeBlocks(freeBytes, freeFiles);
  std::printf("free bytes=%d files=%d\n", freeBytes, freeFiles);
  for (uint32_t i = 0; i < MaxFiles; ++i) {
    CardStat st;
    if (card.getStatus(i, st) != ECardResult::READY)
      continue;
    FileHandle fh;
    card.openFile(i, fh);
    std::vector<uint8_t> data(st.GetFileLength());
    card.asyncRead(fh, data.data(), data.size());
    std::printf("file %u name=%.32s game=%.4s len=%u time=%u banner=%d iconfmt=%04x speed=%04x icon=%08x comment=%08x "
                "data=%016llx\n",
                i, st.GetFileName(), reinterpret_cast<const char*>(st.x28_gameName.data()), st.GetFileLength(),
                st.GetTime(), int(st.GetBannerFormat()), st.x34_iconFormat, st.x36_iconSpeed, st.GetIconAddr(),
                st.GetCommentAddr(), (unsigned long long)fnv(data));
  }
  unlink(copy.c_str());
  return 0;
}

int main(int argc, char** argv) {
  if (argc > 1)
    return dump(argv[1]);

  const std::string path = std::string("/tmp/ectopad-card-test-") + std::to_string(getpid()) + ".raw";
  unlink(path.c_str());
  {
    Card card{"GM8E", "01"};
    CHECK(card.open(path));
    CHECK(card.getError() != ECardResult::READY); // empty file is not a card yet
    CHECK(Card::probeCardFile(path).x0_error == ECardResult::READY);
    card.format(ECardSlot::SlotA);
    CHECK(card.getError() == ECardResult::READY);
    int32_t bytes = 0, files = 0;
    card.getFreeBlocks(bytes, files);
    CHECK(bytes == 2043 * 0x2000);
    CHECK(files == 127);

    FileHandle fh;
    CHECK(card.createFile("MetroidPrime A", 0x6000, fh) == ECardResult::READY);
    CHECK(card.createFile("MetroidPrime A", 0x2000, fh) == ECardResult::EXIST);
    std::vector<uint8_t> pattern(0x5F00);
    for (size_t i = 0; i < pattern.size(); ++i)
      pattern[i] = uint8_t(i * 31 + 7);
    CHECK(card.openFile("MetroidPrime A", fh) == ECardResult::READY);
    card.seek(fh, 0x80, SeekOrigin::Begin);
    CHECK(card.asyncWrite(fh, pattern.data(), pattern.size()) == ECardResult::READY);
    CardStat st;
    CHECK(card.getStatus(fh, st) == ECardResult::READY);
    st.SetBannerFormat(EImageFormat::RGB5A3);
    st.SetIconFormat(EImageFormat::C8, 0);
    st.SetIconSpeed(EAnimationSpeed::Middle, 0);
    st.SetIconFormat(EImageFormat::None, 1);
    st.SetIconSpeed(EAnimationSpeed::End, 1);
    st.SetCommentAddr(4);
    st.SetIconAddr(68);
    CHECK(card.setStatus(fh, st) == ECardResult::READY);
    card.getFreeBlocks(bytes, files);
    CHECK(bytes == 2040 * 0x2000);
    CHECK(files == 126);
    uint64_t serial1 = 0;
    card.getSerial(serial1);
    card.commit();
    card.close();

    CHECK(card.open(path));
    CHECK(card.getError() == ECardResult::READY); // checksums verified on load
    uint64_t serial2 = 0;
    card.getSerial(serial2);
    CHECK(serial1 == serial2);
    CHECK(card.openFile("MetroidPrime A", fh) == ECardResult::READY);
    std::vector<uint8_t> back(pattern.size());
    card.seek(fh, 0x80, SeekOrigin::Begin);
    CHECK(card.asyncRead(fh, back.data(), back.size()) == ECardResult::READY);
    CHECK(back == pattern);
    CardStat st2;
    CHECK(card.getStatus(fh.getFileNo(), st2) == ECardResult::READY);
    CHECK(st2.GetFileLength() == 0x6000);
    CHECK(st2.GetBannerFormat() == EImageFormat::RGB5A3);
    CHECK(st2.GetIconFormat(0) == EImageFormat::C8);
    CHECK(st2.GetIconSpeed(0) == EAnimationSpeed::Middle);
    CHECK(st2.GetCommentAddr() == 4 && st2.GetIconAddr() == 68);
    CHECK(std::strncmp(st2.GetFileName(), "MetroidPrime A", 32) == 0);
    CHECK(card.closeFile(fh) == ECardResult::READY);
    CHECK(card.getStatus(fh.getFileNo(), st2) == ECardResult::READY); // the game does this after closing

    CHECK(card.renameFile("MetroidPrime A", "MetroidPrime B") == ECardResult::READY);
    CHECK(card.openFile("MetroidPrime A", fh) == ECardResult::NOFILE);
    CHECK(card.openFile("MetroidPrime B", fh) == ECardResult::READY);
    CHECK(card.deleteFile("MetroidPrime B") == ECardResult::READY);
    card.getFreeBlocks(bytes, files);
    CHECK(bytes == 2043 * 0x2000);
    CHECK(files == 127);
    CHECK(card.createFile("this-name-is-much-longer-than-thirty-two-bytes", 0x2000, fh) == ECardResult::NAMETOOLONG);
    CHECK(card.createFile("huge", size_t(2044) * 0x2000, fh) == ECardResult::INSSPACE);
    card.close();

    CHECK(card.open(path));
    CHECK(card.getError() == ECardResult::READY);
    CHECK(card.fileCount() == 0);
  }
  unlink(path.c_str());
  CHECK(Card::probeCardFile(path).x0_error == ECardResult::NOCARD);
  std::printf("%s (%d failures)\n", failures ? "FAILED" : "all card tests passed", failures);
  return failures ? 1 : 0;
}
