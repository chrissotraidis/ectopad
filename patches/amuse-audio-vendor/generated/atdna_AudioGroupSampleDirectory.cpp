/* Auto generated atdna implementation */
#include "athena/DNAOp.hpp"
#include <amuse/AudioGroupSampleDirectory.hpp>

template <class Op>
void amuse::DSPADPCMHeader::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("x0_num_samples"sv), x0_num_samples, s);
  Do<Op>(athena::io::PropId("x4_num_nibbles"sv), x4_num_nibbles, s);
  Do<Op>(athena::io::PropId("x8_sample_rate"sv), x8_sample_rate, s);
  Do<Op>(athena::io::PropId("xc_loop_flag"sv), xc_loop_flag, s);
  Do<Op>(athena::io::PropId("xe_format"sv), xe_format, s);
  Do<Op>(athena::io::PropId("x10_loop_start_nibble"sv), x10_loop_start_nibble, s);
  Do<Op>(athena::io::PropId("x14_loop_end_nibble"sv), x14_loop_end_nibble, s);
  Do<Op>(athena::io::PropId("x18_ca"sv), x18_ca, s);
  Do<Op>(athena::io::PropId("x1c_coef"sv), x1c_coef, s);
  Do<Op>(athena::io::PropId("x3c_gain"sv), x3c_gain, s);
  Do<Op>(athena::io::PropId("x3e_ps"sv), x3e_ps, s);
  Do<Op>(athena::io::PropId("x40_hist1"sv), x40_hist1, s);
  Do<Op>(athena::io::PropId("x42_hist2"sv), x42_hist2, s);
  Do<Op>(athena::io::PropId("x44_loop_ps"sv), x44_loop_ps, s);
  Do<Op>(athena::io::PropId("x46_loop_hist1"sv), x46_loop_hist1, s);
  Do<Op>(athena::io::PropId("x48_loop_hist2"sv), x48_loop_hist2, s);
  Do<Op>(athena::io::PropId("m_pitch"sv), m_pitch, s);
  DoSeek<Op>(21, athena::SeekOrigin::Current, s);
}

AT_SPECIALIZE_DNA(amuse::DSPADPCMHeader)


std::string_view amuse::DSPADPCMHeader::DNAType() {
  return "amuse::DSPADPCMHeader"sv;
}


template <class Op>
void amuse::VADPCMHeader::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("m_pitchSampleRate"sv), m_pitchSampleRate, s);
  Do<Op>(athena::io::PropId("m_numSamples"sv), m_numSamples, s);
  Do<Op>(athena::io::PropId("m_loopStartSample"sv), m_loopStartSample, s);
  Do<Op>(athena::io::PropId("m_loopLengthSamples"sv), m_loopLengthSamples, s);
}

AT_SPECIALIZE_DNA(amuse::VADPCMHeader)


std::string_view amuse::VADPCMHeader::DNAType() {
  return "amuse::VADPCMHeader"sv;
}


template <class Op>
void amuse::WAVFormatChunk::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("sampleFmt"sv), sampleFmt, s);
  Do<Op>(athena::io::PropId("numChannels"sv), numChannels, s);
  Do<Op>(athena::io::PropId("sampleRate"sv), sampleRate, s);
  Do<Op>(athena::io::PropId("byteRate"sv), byteRate, s);
  Do<Op>(athena::io::PropId("blockAlign"sv), blockAlign, s);
  Do<Op>(athena::io::PropId("bitsPerSample"sv), bitsPerSample, s);
}

AT_SPECIALIZE_DNA(amuse::WAVFormatChunk)


std::string_view amuse::WAVFormatChunk::DNAType() {
  return "amuse::WAVFormatChunk"sv;
}


template <class Op>
void amuse::WAVSampleChunk::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("smplManufacturer"sv), smplManufacturer, s);
  Do<Op>(athena::io::PropId("smplProduct"sv), smplProduct, s);
  Do<Op>(athena::io::PropId("smplPeriod"sv), smplPeriod, s);
  Do<Op>(athena::io::PropId("midiNote"sv), midiNote, s);
  Do<Op>(athena::io::PropId("midiPitchFrac"sv), midiPitchFrac, s);
  Do<Op>(athena::io::PropId("smpteFormat"sv), smpteFormat, s);
  Do<Op>(athena::io::PropId("smpteOffset"sv), smpteOffset, s);
  Do<Op>(athena::io::PropId("numSampleLoops"sv), numSampleLoops, s);
  Do<Op>(athena::io::PropId("additionalDataSize"sv), additionalDataSize, s);
}

AT_SPECIALIZE_DNA(amuse::WAVSampleChunk)


std::string_view amuse::WAVSampleChunk::DNAType() {
  return "amuse::WAVSampleChunk"sv;
}


template <class Op>
void amuse::WAVSampleLoop::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("cuePointId"sv), cuePointId, s);
  Do<Op>(athena::io::PropId("loopType"sv), loopType, s);
  Do<Op>(athena::io::PropId("start"sv), start, s);
  Do<Op>(athena::io::PropId("end"sv), end, s);
  Do<Op>(athena::io::PropId("fraction"sv), fraction, s);
  Do<Op>(athena::io::PropId("playCount"sv), playCount, s);
}

AT_SPECIALIZE_DNA(amuse::WAVSampleLoop)


std::string_view amuse::WAVSampleLoop::DNAType() {
  return "amuse::WAVSampleLoop"sv;
}


template <class Op>
void amuse::WAVHeader::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("riffMagic"sv), riffMagic, s);
  Do<Op>(athena::io::PropId("wavChuckSize"sv), wavChuckSize, s);
  Do<Op>(athena::io::PropId("wavMagic"sv), wavMagic, s);
  Do<Op>(athena::io::PropId("fmtMagic"sv), fmtMagic, s);
  Do<Op>(athena::io::PropId("fmtChunkSize"sv), fmtChunkSize, s);
  Do<Op>(athena::io::PropId("fmtChunk"sv), fmtChunk, s);
  Do<Op>(athena::io::PropId("smplMagic"sv), smplMagic, s);
  Do<Op>(athena::io::PropId("smplChunkSize"sv), smplChunkSize, s);
  Do<Op>(athena::io::PropId("smplChunk"sv), smplChunk, s);
  Do<Op>(athena::io::PropId("dataMagic"sv), dataMagic, s);
  Do<Op>(athena::io::PropId("dataChunkSize"sv), dataChunkSize, s);
}

AT_SPECIALIZE_DNA(amuse::WAVHeader)


std::string_view amuse::WAVHeader::DNAType() {
  return "amuse::WAVHeader"sv;
}


template <class Op>
void amuse::WAVHeaderLoop::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("riffMagic"sv), riffMagic, s);
  Do<Op>(athena::io::PropId("wavChuckSize"sv), wavChuckSize, s);
  Do<Op>(athena::io::PropId("wavMagic"sv), wavMagic, s);
  Do<Op>(athena::io::PropId("fmtMagic"sv), fmtMagic, s);
  Do<Op>(athena::io::PropId("fmtChunkSize"sv), fmtChunkSize, s);
  Do<Op>(athena::io::PropId("fmtChunk"sv), fmtChunk, s);
  Do<Op>(athena::io::PropId("smplMagic"sv), smplMagic, s);
  Do<Op>(athena::io::PropId("smplChunkSize"sv), smplChunkSize, s);
  Do<Op>(athena::io::PropId("smplChunk"sv), smplChunk, s);
  Do<Op>(athena::io::PropId("sampleLoop"sv), sampleLoop, s);
  Do<Op>(athena::io::PropId("dataMagic"sv), dataMagic, s);
  Do<Op>(athena::io::PropId("dataChunkSize"sv), dataChunkSize, s);
}

AT_SPECIALIZE_DNA(amuse::WAVHeaderLoop)


std::string_view amuse::WAVHeaderLoop::DNAType() {
  return "amuse::WAVHeaderLoop"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::AudioGroupSampleDirectory::EntryDNA<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("m_sfxId"sv), m_sfxId, s);
  DoSeek<Op>(2, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("m_sampleOff"sv), m_sampleOff, s);
  Do<Op>(athena::io::PropId("m_unk"sv), m_unk, s);
  Do<Op>(athena::io::PropId("m_pitch"sv), m_pitch, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("m_sampleRate"sv), m_sampleRate, s);
  Do<Op>(athena::io::PropId("m_numSamples"sv), m_numSamples, s);
  Do<Op>(athena::io::PropId("m_loopStartSample"sv), m_loopStartSample, s);
  Do<Op>(athena::io::PropId("m_loopLengthSamples"sv), m_loopLengthSamples, s);
  Do<Op>(athena::io::PropId("m_adpcmParmOffset"sv), m_adpcmParmOffset, s);
}

AT_SPECIALIZE_DNA(amuse::AudioGroupSampleDirectory::EntryDNA<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::AudioGroupSampleDirectory::EntryDNA<athena::Endian::Little>)


template <>
std::string_view amuse::AudioGroupSampleDirectory::EntryDNA<athena::Endian::Big>::DNAType() {
  return "amuse::AudioGroupSampleDirectory::EntryDNA<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::AudioGroupSampleDirectory::EntryDNA<athena::Endian::Little>::DNAType() {
  return "amuse::AudioGroupSampleDirectory::EntryDNA<athena::Endian::Little>"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::AudioGroupSampleDirectory::MusyX1SdirEntry<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("m_sfxId"sv), m_sfxId, s);
  DoSeek<Op>(2, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("m_sampleOff"sv), m_sampleOff, s);
  Do<Op>(athena::io::PropId("m_pitchSampleRate"sv), m_pitchSampleRate, s);
  Do<Op>(athena::io::PropId("m_numSamples"sv), m_numSamples, s);
  Do<Op>(athena::io::PropId("m_loopStartSample"sv), m_loopStartSample, s);
  Do<Op>(athena::io::PropId("m_loopLengthSamples"sv), m_loopLengthSamples, s);
}

AT_SPECIALIZE_DNA(amuse::AudioGroupSampleDirectory::MusyX1SdirEntry<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::AudioGroupSampleDirectory::MusyX1SdirEntry<athena::Endian::Little>)


template <>
std::string_view amuse::AudioGroupSampleDirectory::MusyX1SdirEntry<athena::Endian::Big>::DNAType() {
  return "amuse::AudioGroupSampleDirectory::MusyX1SdirEntry<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::AudioGroupSampleDirectory::MusyX1SdirEntry<athena::Endian::Little>::DNAType() {
  return "amuse::AudioGroupSampleDirectory::MusyX1SdirEntry<athena::Endian::Little>"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::AudioGroupSampleDirectory::MusyX1AbsSdirEntry<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("m_sfxId"sv), m_sfxId, s);
  DoSeek<Op>(2, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("m_sampleOff"sv), m_sampleOff, s);
  Do<Op>(athena::io::PropId("m_unk"sv), m_unk, s);
  Do<Op>(athena::io::PropId("m_pitchSampleRate"sv), m_pitchSampleRate, s);
  Do<Op>(athena::io::PropId("m_numSamples"sv), m_numSamples, s);
  Do<Op>(athena::io::PropId("m_loopStartSample"sv), m_loopStartSample, s);
  Do<Op>(athena::io::PropId("m_loopLengthSamples"sv), m_loopLengthSamples, s);
}

AT_SPECIALIZE_DNA(amuse::AudioGroupSampleDirectory::MusyX1AbsSdirEntry<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::AudioGroupSampleDirectory::MusyX1AbsSdirEntry<athena::Endian::Little>)


template <>
std::string_view amuse::AudioGroupSampleDirectory::MusyX1AbsSdirEntry<athena::Endian::Big>::DNAType() {
  return "amuse::AudioGroupSampleDirectory::MusyX1AbsSdirEntry<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::AudioGroupSampleDirectory::MusyX1AbsSdirEntry<athena::Endian::Little>::DNAType() {
  return "amuse::AudioGroupSampleDirectory::MusyX1AbsSdirEntry<athena::Endian::Little>"sv;
}


