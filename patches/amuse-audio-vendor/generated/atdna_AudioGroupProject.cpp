/* Auto generated atdna implementation */
#include "athena/DNAOp.hpp"
#include <amuse/AudioGroupProject.hpp>

template <athena::Endian DNAEn>
template <class Op>
void amuse::GroupHeader<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("groupEndOff"sv), groupEndOff, s);
  Do<Op>(athena::io::PropId("groupId"sv), groupId, s);
  Do<Op>(athena::io::PropId("type"sv), type, s);
  Do<Op>(athena::io::PropId("soundMacroIdsOff"sv), soundMacroIdsOff, s);
  Do<Op>(athena::io::PropId("samplIdsOff"sv), samplIdsOff, s);
  Do<Op>(athena::io::PropId("tableIdsOff"sv), tableIdsOff, s);
  Do<Op>(athena::io::PropId("keymapIdsOff"sv), keymapIdsOff, s);
  Do<Op>(athena::io::PropId("layerIdsOff"sv), layerIdsOff, s);
  Do<Op>(athena::io::PropId("pageTableOff"sv), pageTableOff, s);
  Do<Op>(athena::io::PropId("drumTableOff"sv), drumTableOff, s);
  Do<Op>(athena::io::PropId("midiSetupsOff"sv), midiSetupsOff, s);
}

AT_SPECIALIZE_DNA(amuse::GroupHeader<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::GroupHeader<athena::Endian::Little>)


template <>
std::string_view amuse::GroupHeader<athena::Endian::Big>::DNAType() {
  return "amuse::GroupHeader<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::GroupHeader<athena::Endian::Little>::DNAType() {
  return "amuse::GroupHeader<athena::Endian::Little>"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::SongGroupIndex::PageEntryDNA<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("objId"sv), objId, s);
  Do<Op>(athena::io::PropId("priority"sv), priority, s);
  Do<Op>(athena::io::PropId("maxVoices"sv), maxVoices, s);
  Do<Op>(athena::io::PropId("programNo"sv), programNo, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SongGroupIndex::PageEntryDNA<athena::Endian::Big>)
AT_SPECIALIZE_DNA_YAML(amuse::SongGroupIndex::PageEntryDNA<athena::Endian::Little>)


template <>
std::string_view amuse::SongGroupIndex::PageEntryDNA<athena::Endian::Big>::DNAType() {
  return "amuse::SongGroupIndex::PageEntryDNA<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::SongGroupIndex::PageEntryDNA<athena::Endian::Little>::DNAType() {
  return "amuse::SongGroupIndex::PageEntryDNA<athena::Endian::Little>"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::SongGroupIndex::MusyX1PageEntryDNA<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("objId"sv), objId, s);
  Do<Op>(athena::io::PropId("priority"sv), priority, s);
  Do<Op>(athena::io::PropId("maxVoices"sv), maxVoices, s);
  Do<Op>(athena::io::PropId("unk"sv), unk, s);
  Do<Op>(athena::io::PropId("programNo"sv), programNo, s);
  DoSeek<Op>(2, athena::SeekOrigin::Current, s);
}

AT_SPECIALIZE_DNA(amuse::SongGroupIndex::MusyX1PageEntryDNA<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::SongGroupIndex::MusyX1PageEntryDNA<athena::Endian::Little>)


template <>
std::string_view amuse::SongGroupIndex::MusyX1PageEntryDNA<athena::Endian::Big>::DNAType() {
  return "amuse::SongGroupIndex::MusyX1PageEntryDNA<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::SongGroupIndex::MusyX1PageEntryDNA<athena::Endian::Little>::DNAType() {
  return "amuse::SongGroupIndex::MusyX1PageEntryDNA<athena::Endian::Little>"sv;
}


template <class Op>
void amuse::SongGroupIndex::PageEntry::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("objId"sv), objId, s);
  Do<Op>(athena::io::PropId("priority"sv), priority, s);
  Do<Op>(athena::io::PropId("maxVoices"sv), maxVoices, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SongGroupIndex::PageEntry)


std::string_view amuse::SongGroupIndex::PageEntry::DNAType() {
  return "amuse::SongGroupIndex::PageEntry"sv;
}


template <class Op>
void amuse::SongGroupIndex::MusyX1MIDISetup::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("programNo"sv), programNo, s);
  Do<Op>(athena::io::PropId("volume"sv), volume, s);
  Do<Op>(athena::io::PropId("panning"sv), panning, s);
  Do<Op>(athena::io::PropId("reverb"sv), reverb, s);
  Do<Op>(athena::io::PropId("chorus"sv), chorus, s);
  DoSeek<Op>(3, athena::SeekOrigin::Current, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SongGroupIndex::MusyX1MIDISetup)


std::string_view amuse::SongGroupIndex::MusyX1MIDISetup::DNAType() {
  return "amuse::SongGroupIndex::MusyX1MIDISetup"sv;
}


template <class Op>
void amuse::SongGroupIndex::MIDISetup::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("programNo"sv), programNo, s);
  Do<Op>(athena::io::PropId("volume"sv), volume, s);
  Do<Op>(athena::io::PropId("panning"sv), panning, s);
  Do<Op>(athena::io::PropId("reverb"sv), reverb, s);
  Do<Op>(athena::io::PropId("chorus"sv), chorus, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SongGroupIndex::MIDISetup)


std::string_view amuse::SongGroupIndex::MIDISetup::DNAType() {
  return "amuse::SongGroupIndex::MIDISetup"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::SFXGroupIndex::SFXEntryDNA<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("sfxId"sv), sfxId, s);
  Do<Op>(athena::io::PropId("objId"sv), objId, s);
  Do<Op>(athena::io::PropId("priority"sv), priority, s);
  Do<Op>(athena::io::PropId("maxVoices"sv), maxVoices, s);
  Do<Op>(athena::io::PropId("defVel"sv), defVel, s);
  Do<Op>(athena::io::PropId("panning"sv), panning, s);
  Do<Op>(athena::io::PropId("defKey"sv), defKey, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
}

AT_SPECIALIZE_DNA(amuse::SFXGroupIndex::SFXEntryDNA<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::SFXGroupIndex::SFXEntryDNA<athena::Endian::Little>)


template <>
std::string_view amuse::SFXGroupIndex::SFXEntryDNA<athena::Endian::Big>::DNAType() {
  return "amuse::SFXGroupIndex::SFXEntryDNA<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::SFXGroupIndex::SFXEntryDNA<athena::Endian::Little>::DNAType() {
  return "amuse::SFXGroupIndex::SFXEntryDNA<athena::Endian::Little>"sv;
}


template <class Op>
void amuse::SFXGroupIndex::SFXEntry::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("objId"sv), objId, s);
  Do<Op>(athena::io::PropId("priority"sv), priority, s);
  Do<Op>(athena::io::PropId("maxVoices"sv), maxVoices, s);
  Do<Op>(athena::io::PropId("defVel"sv), defVel, s);
  Do<Op>(athena::io::PropId("panning"sv), panning, s);
  Do<Op>(athena::io::PropId("defKey"sv), defKey, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SFXGroupIndex::SFXEntry)


std::string_view amuse::SFXGroupIndex::SFXEntry::DNAType() {
  return "amuse::SFXGroupIndex::SFXEntry"sv;
}


