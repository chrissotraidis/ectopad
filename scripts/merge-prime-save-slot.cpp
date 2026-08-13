#include <kabufuda/Card.hpp>

#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr size_t GciHeaderSize = 0x40;
constexpr size_t CardFileSize = 0x2000;
constexpr size_t PrimeSaveSize = 3004;
constexpr size_t PrimeHeaderSize = 4 + 3;
constexpr size_t PrimeSystemSize = 174;
constexpr size_t PrimeSlotSize = 940;
constexpr size_t PrimeSlotDataOffset = PrimeHeaderSize + PrimeSystemSize;

uint16_t ReadBe16(const uint8_t* ptr) {
  return uint16_t(ptr[0]) << 8 | uint16_t(ptr[1]);
}

uint32_t ReadBe32(const uint8_t* ptr) {
  return uint32_t(ptr[0]) << 24 | uint32_t(ptr[1]) << 16 | uint32_t(ptr[2]) << 8 | uint32_t(ptr[3]);
}

void WriteBe32(uint8_t* ptr, uint32_t value) {
  ptr[0] = uint8_t(value >> 24);
  ptr[1] = uint8_t(value >> 16);
  ptr[2] = uint8_t(value >> 8);
  ptr[3] = uint8_t(value);
}

uint32_t PrimeCrc32(const uint8_t* data, size_t size) {
  uint32_t crc = 0xffffffff;
  for (size_t i = 0; i < size; ++i) {
    crc ^= data[i];
    for (int bit = 0; bit < 8; ++bit) {
      crc = (crc >> 1) ^ (0xedb88320u & uint32_t(-int32_t(crc & 1)));
    }
  }
  return crc;
}

size_t SaveDataOffset(const kabufuda::CardStat& stat) {
  size_t offset = 4 + 64;
  switch (stat.GetBannerFormat()) {
  case kabufuda::EImageFormat::C8:
    offset += 3584;
    break;
  case kabufuda::EImageFormat::RGB5A3:
    offset += 6144;
    break;
  default:
    break;
  }

  bool paletteIcon = false;
  for (int idx = 0; idx < 8; ++idx) {
    const auto format = stat.GetIconFormat(idx);
    if (format == kabufuda::EImageFormat::None) {
      break;
    }
    if (format == kabufuda::EImageFormat::C8) {
      paletteIcon = true;
      offset += 1024;
    } else {
      offset += 2048;
    }
  }
  if (paletteIcon) {
    offset += 512;
  }
  return offset;
}

std::vector<uint8_t> ReadFile(const std::string& path) {
  std::ifstream stream(path, std::ios::binary | std::ios::ate);
  if (!stream) {
    return {};
  }
  const auto length = stream.tellg();
  if (length <= 0) {
    return {};
  }
  std::vector<uint8_t> data(static_cast<size_t>(length));
  stream.seekg(0);
  stream.read(reinterpret_cast<char*>(data.data()), std::streamsize(data.size()));
  return stream ? data : std::vector<uint8_t>{};
}

bool ValidatePrimeFile(const std::vector<uint8_t>& fileData, size_t saveOffset, const char* label) {
  if (fileData.size() != CardFileSize || saveOffset + PrimeSaveSize > fileData.size()) {
    std::cerr << label << ": unexpected file size or save-data offset\n";
    return false;
  }
  const uint32_t stored = ReadBe32(fileData.data());
  const uint32_t computed = PrimeCrc32(fileData.data() + 4, fileData.size() - 4);
  if (stored != computed) {
    std::cerr << label << ": CRC mismatch (stored=" << std::hex << stored << " computed=" << computed << ")\n";
    return false;
  }
  return true;
}
} // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: merge-prime-save-slot <MemoryCardA.USA.raw copy> <USA Rev 2 donor.gci>\n";
    return 2;
  }

  const std::string cardPath = argv[1];
  const std::string gciPath = argv[2];
  std::vector<uint8_t> gci = ReadFile(gciPath);
  if (gci.size() != GciHeaderSize + CardFileSize || std::memcmp(gci.data(), "GM8E01", 6) != 0 ||
      std::memcmp(gci.data() + 8, "MetroidPrime A", 14) != 0 || ReadBe16(gci.data() + 0x38) != 1) {
    std::cerr << "donor is not a one-block GM8E/01 MetroidPrime A GCI\n";
    return 3;
  }

  kabufuda::Card card("GM8E", "01");
  if (!card.open(cardPath)) {
    std::cerr << "failed to open card copy\n";
    return 4;
  }
  card.waitForCompletion();
  if (card.getError() != kabufuda::ECardResult::READY) {
    std::cerr << "card copy failed integrity validation\n";
    return 5;
  }

  kabufuda::FileHandle handle;
  if (card.openFile("MetroidPrime A", handle) != kabufuda::ECardResult::READY) {
    std::cerr << "MetroidPrime A was not found on the card copy\n";
    return 6;
  }
  kabufuda::CardStat stat{};
  if (card.getStatus(handle, stat) != kabufuda::ECardResult::READY || stat.GetFileLength() != CardFileSize) {
    std::cerr << "existing Prime card file has an unexpected size\n";
    return 7;
  }

  std::vector<uint8_t> current(CardFileSize);
  if (card.asyncRead(handle, current.data(), current.size()) != kabufuda::ECardResult::READY) {
    std::cerr << "failed to read existing Prime card file\n";
    return 8;
  }
  card.waitForCompletion();

  const size_t currentSaveOffset = SaveDataOffset(stat);
  const uint8_t donorBannerFormat = gci[7] & 3;
  kabufuda::CardStat donorStat{};
  donorStat.SetBannerFormat(kabufuda::EImageFormat(donorBannerFormat));
  donorStat.x34_iconFormat = ReadBe16(gci.data() + 0x30);
  const size_t donorSaveOffset = SaveDataOffset(donorStat);
  std::vector<uint8_t> donor(gci.begin() + GciHeaderSize, gci.end());

  if (!ValidatePrimeFile(current, currentSaveOffset, "existing") ||
      !ValidatePrimeFile(donor, donorSaveOffset, "donor")) {
    return 9;
  }

  const uint8_t* currentSave = current.data() + currentSaveOffset;
  const uint8_t* donorSave = donor.data() + donorSaveOffset;
  if (ReadBe32(currentSave) != ReadBe32(donorSave)) {
    std::cerr << "Prime save versions differ\n";
    return 10;
  }
  if (currentSave[5] != 0) {
    std::cerr << "existing internal file slot 2 is occupied; refusing to replace it\n";
    return 11;
  }
  if (donorSave[4] == 0) {
    std::cerr << "donor internal file slot 1 is empty\n";
    return 12;
  }

  std::array<size_t, 3> currentSlotOffsets{};
  size_t cursor = PrimeSlotDataOffset;
  for (size_t slot = 0; slot < 3; ++slot) {
    if (currentSave[4 + slot] != 0) {
      currentSlotOffsets[slot] = cursor;
      cursor += PrimeSlotSize;
    }
  }
  std::array<size_t, 3> donorSlotOffsets{};
  cursor = PrimeSlotDataOffset;
  for (size_t slot = 0; slot < 3; ++slot) {
    if (donorSave[4 + slot] != 0) {
      donorSlotOffsets[slot] = cursor;
      cursor += PrimeSlotSize;
    }
  }

  std::array<uint8_t, PrimeSaveSize> merged{};
  std::memcpy(merged.data(), currentSave, PrimeSlotDataOffset);
  merged[5] = 1;
  cursor = PrimeSlotDataOffset;
  for (size_t slot = 0; slot < 3; ++slot) {
    if (slot == 1) {
      std::memcpy(merged.data() + cursor, donorSave + donorSlotOffsets[0], PrimeSlotSize);
      cursor += PrimeSlotSize;
    } else if (currentSave[4 + slot] != 0) {
      std::memcpy(merged.data() + cursor, currentSave + currentSlotOffsets[slot], PrimeSlotSize);
      cursor += PrimeSlotSize;
    }
  }

  const std::array<uint8_t, PrimeSlotSize> originalSlot1 = [&] {
    std::array<uint8_t, PrimeSlotSize> copy{};
    if (currentSave[4] != 0) {
      std::memcpy(copy.data(), currentSave + currentSlotOffsets[0], copy.size());
    }
    return copy;
  }();

  std::memcpy(current.data() + currentSaveOffset, merged.data(), merged.size());
  WriteBe32(current.data(), PrimeCrc32(current.data() + 4, current.size() - 4));
  card.seek(handle, 0, kabufuda::SeekOrigin::Begin);
  if (card.asyncWrite(handle, current.data(), current.size()) != kabufuda::ECardResult::READY) {
    std::cerr << "failed to write merged Prime file into card copy\n";
    return 13;
  }
  card.waitForCompletion();
  card.close();

  kabufuda::Card verify("GM8E", "01");
  if (!verify.open(cardPath)) {
    std::cerr << "failed to reopen merged card copy\n";
    return 14;
  }
  verify.waitForCompletion();
  kabufuda::FileHandle verifyHandle;
  if (verify.getError() != kabufuda::ECardResult::READY ||
      verify.openFile("MetroidPrime A", verifyHandle) != kabufuda::ECardResult::READY) {
    std::cerr << "merged card copy failed card/file validation\n";
    return 15;
  }
  kabufuda::CardStat verifyStat{};
  verify.getStatus(verifyHandle, verifyStat);
  std::vector<uint8_t> verifyData(CardFileSize);
  verify.asyncRead(verifyHandle, verifyData.data(), verifyData.size());
  verify.waitForCompletion();
  const size_t verifySaveOffset = SaveDataOffset(verifyStat);
  const uint8_t* verifySave = verifyData.data() + verifySaveOffset;
  if (!ValidatePrimeFile(verifyData, verifySaveOffset, "merged") || verifySave[5] == 0 ||
      (currentSave[4] != 0 && std::memcmp(verifySave + PrimeSlotDataOffset, originalSlot1.data(), PrimeSlotSize) != 0)) {
    std::cerr << "merged slot validation failed or internal file slot 1 changed\n";
    return 16;
  }

  std::cout << "merged USA Rev 2 donor into internal file slot 2; slot 1 preserved; card and Prime CRCs valid\n";
  return 0;
}
