/* Auto generated atdna implementation */
#include "athena/DNAOp.hpp"
#include <amuse/AudioGroupPool.hpp>

template <athena::Endian DNAEn>
template <class Op>
void amuse::PoolHeader<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("soundMacrosOffset"sv), soundMacrosOffset, s);
  Do<Op>(athena::io::PropId("tablesOffset"sv), tablesOffset, s);
  Do<Op>(athena::io::PropId("keymapsOffset"sv), keymapsOffset, s);
  Do<Op>(athena::io::PropId("layersOffset"sv), layersOffset, s);
}

AT_SPECIALIZE_DNA(amuse::PoolHeader<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::PoolHeader<athena::Endian::Little>)


template <>
std::string_view amuse::PoolHeader<athena::Endian::Big>::DNAType() {
  return "amuse::PoolHeader<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::PoolHeader<athena::Endian::Little>::DNAType() {
  return "amuse::PoolHeader<athena::Endian::Little>"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::ObjectHeader<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("size"sv), size, s);
  Do<Op>(athena::io::PropId("objectId"sv), objectId, s);
  DoSeek<Op>(2, athena::SeekOrigin::Current, s);
}

AT_SPECIALIZE_DNA(amuse::ObjectHeader<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::ObjectHeader<athena::Endian::Little>)


template <>
std::string_view amuse::ObjectHeader<athena::Endian::Big>::DNAType() {
  return "amuse::ObjectHeader<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::ObjectHeader<athena::Endian::Little>::DNAType() {
  return "amuse::ObjectHeader<athena::Endian::Little>"sv;
}


template <class Op>
void amuse::SoundMacro::ICmd::Enumerate(typename Op::StreamT& s) {
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::ICmd)


std::string_view amuse::SoundMacro::ICmd::DNAType() {
  return "amuse::SoundMacro::ICmd"sv;
}


template <class Op>
void amuse::SoundMacro::CmdEnd::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdEnd)


std::string_view amuse::SoundMacro::CmdEnd::DNAType() {
  return "amuse::SoundMacro::CmdEnd"sv;
}


template <class Op>
void amuse::SoundMacro::CmdStop::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdStop)


std::string_view amuse::SoundMacro::CmdStop::DNAType() {
  return "amuse::SoundMacro::CmdStop"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSplitKey::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("key"sv), key, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSplitKey)


std::string_view amuse::SoundMacro::CmdSplitKey::DNAType() {
  return "amuse::SoundMacro::CmdSplitKey"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSplitVel::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("velocity"sv), velocity, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSplitVel)


std::string_view amuse::SoundMacro::CmdSplitVel::DNAType() {
  return "amuse::SoundMacro::CmdSplitVel"sv;
}


template <class Op>
void amuse::SoundMacro::CmdWaitTicks::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("keyOff"sv), keyOff, s);
  Do<Op>(athena::io::PropId("random"sv), random, s);
  Do<Op>(athena::io::PropId("sampleEnd"sv), sampleEnd, s);
  Do<Op>(athena::io::PropId("absolute"sv), absolute, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdWaitTicks)


std::string_view amuse::SoundMacro::CmdWaitTicks::DNAType() {
  return "amuse::SoundMacro::CmdWaitTicks"sv;
}


template <class Op>
void amuse::SoundMacro::CmdLoop::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("keyOff"sv), keyOff, s);
  Do<Op>(athena::io::PropId("random"sv), random, s);
  Do<Op>(athena::io::PropId("sampleEnd"sv), sampleEnd, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
  Do<Op>(athena::io::PropId("times"sv), times, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdLoop)


std::string_view amuse::SoundMacro::CmdLoop::DNAType() {
  return "amuse::SoundMacro::CmdLoop"sv;
}


template <class Op>
void amuse::SoundMacro::CmdGoto::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdGoto)


std::string_view amuse::SoundMacro::CmdGoto::DNAType() {
  return "amuse::SoundMacro::CmdGoto"sv;
}


template <class Op>
void amuse::SoundMacro::CmdWaitMs::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("keyOff"sv), keyOff, s);
  Do<Op>(athena::io::PropId("random"sv), random, s);
  Do<Op>(athena::io::PropId("sampleEnd"sv), sampleEnd, s);
  Do<Op>(athena::io::PropId("absolute"sv), absolute, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("ms"sv), ms, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdWaitMs)


std::string_view amuse::SoundMacro::CmdWaitMs::DNAType() {
  return "amuse::SoundMacro::CmdWaitMs"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPlayMacro::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("addNote"sv), addNote, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
  Do<Op>(athena::io::PropId("priority"sv), priority, s);
  Do<Op>(athena::io::PropId("maxVoices"sv), maxVoices, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPlayMacro)


std::string_view amuse::SoundMacro::CmdPlayMacro::DNAType() {
  return "amuse::SoundMacro::CmdPlayMacro"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSendKeyOff::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("variable"sv), variable, s);
  Do<Op>(athena::io::PropId("lastStarted"sv), lastStarted, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSendKeyOff)


std::string_view amuse::SoundMacro::CmdSendKeyOff::DNAType() {
  return "amuse::SoundMacro::CmdSendKeyOff"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSplitMod::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("modValue"sv), modValue, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSplitMod)


std::string_view amuse::SoundMacro::CmdSplitMod::DNAType() {
  return "amuse::SoundMacro::CmdSplitMod"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPianoPan::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("scale"sv), scale, s);
  Do<Op>(athena::io::PropId("centerKey"sv), centerKey, s);
  Do<Op>(athena::io::PropId("centerPan"sv), centerPan, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPianoPan)


std::string_view amuse::SoundMacro::CmdPianoPan::DNAType() {
  return "amuse::SoundMacro::CmdPianoPan"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetAdsr::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("table"sv), table, s);
  Do<Op>(athena::io::PropId("dlsMode"sv), dlsMode, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetAdsr)


std::string_view amuse::SoundMacro::CmdSetAdsr::DNAType() {
  return "amuse::SoundMacro::CmdSetAdsr"sv;
}


template <class Op>
void amuse::SoundMacro::CmdScaleVolume::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("scale"sv), scale, s);
  Do<Op>(athena::io::PropId("add"sv), add, s);
  Do<Op>(athena::io::PropId("table"sv), table, s);
  Do<Op>(athena::io::PropId("originalVol"sv), originalVol, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdScaleVolume)


std::string_view amuse::SoundMacro::CmdScaleVolume::DNAType() {
  return "amuse::SoundMacro::CmdScaleVolume"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPanning::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("panPosition"sv), panPosition, s);
  Do<Op>(athena::io::PropId("timeMs"sv), timeMs, s);
  Do<Op>(athena::io::PropId("width"sv), width, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPanning)


std::string_view amuse::SoundMacro::CmdPanning::DNAType() {
  return "amuse::SoundMacro::CmdPanning"sv;
}


template <class Op>
void amuse::SoundMacro::CmdEnvelope::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("scale"sv), scale, s);
  Do<Op>(athena::io::PropId("add"sv), add, s);
  Do<Op>(athena::io::PropId("table"sv), table, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdEnvelope)


std::string_view amuse::SoundMacro::CmdEnvelope::DNAType() {
  return "amuse::SoundMacro::CmdEnvelope"sv;
}


template <class Op>
void amuse::SoundMacro::CmdStartSample::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("sample"sv), sample, s);
  Do<Op>(athena::io::PropId("mode"sv), mode, s);
  Do<Op>(athena::io::PropId("offset"sv), offset, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdStartSample)


std::string_view amuse::SoundMacro::CmdStartSample::DNAType() {
  return "amuse::SoundMacro::CmdStartSample"sv;
}


template <class Op>
void amuse::SoundMacro::CmdStopSample::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdStopSample)


std::string_view amuse::SoundMacro::CmdStopSample::DNAType() {
  return "amuse::SoundMacro::CmdStopSample"sv;
}


template <class Op>
void amuse::SoundMacro::CmdKeyOff::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdKeyOff)


std::string_view amuse::SoundMacro::CmdKeyOff::DNAType() {
  return "amuse::SoundMacro::CmdKeyOff"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSplitRnd::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("rnd"sv), rnd, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSplitRnd)


std::string_view amuse::SoundMacro::CmdSplitRnd::DNAType() {
  return "amuse::SoundMacro::CmdSplitRnd"sv;
}


template <class Op>
void amuse::SoundMacro::CmdFadeIn::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("scale"sv), scale, s);
  Do<Op>(athena::io::PropId("add"sv), add, s);
  Do<Op>(athena::io::PropId("table"sv), table, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdFadeIn)


std::string_view amuse::SoundMacro::CmdFadeIn::DNAType() {
  return "amuse::SoundMacro::CmdFadeIn"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSpanning::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("spanPosition"sv), spanPosition, s);
  Do<Op>(athena::io::PropId("timeMs"sv), timeMs, s);
  Do<Op>(athena::io::PropId("width"sv), width, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSpanning)


std::string_view amuse::SoundMacro::CmdSpanning::DNAType() {
  return "amuse::SoundMacro::CmdSpanning"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetAdsrCtrl::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("attack"sv), attack, s);
  Do<Op>(athena::io::PropId("decay"sv), decay, s);
  Do<Op>(athena::io::PropId("sustain"sv), sustain, s);
  Do<Op>(athena::io::PropId("release"sv), release, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetAdsrCtrl)


std::string_view amuse::SoundMacro::CmdSetAdsrCtrl::DNAType() {
  return "amuse::SoundMacro::CmdSetAdsrCtrl"sv;
}


template <class Op>
void amuse::SoundMacro::CmdRndNote::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("noteLo"sv), noteLo, s);
  Do<Op>(athena::io::PropId("detune"sv), detune, s);
  Do<Op>(athena::io::PropId("noteHi"sv), noteHi, s);
  Do<Op>(athena::io::PropId("fixedFree"sv), fixedFree, s);
  Do<Op>(athena::io::PropId("absRel"sv), absRel, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdRndNote)


std::string_view amuse::SoundMacro::CmdRndNote::DNAType() {
  return "amuse::SoundMacro::CmdRndNote"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAddNote::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("add"sv), add, s);
  Do<Op>(athena::io::PropId("detune"sv), detune, s);
  Do<Op>(athena::io::PropId("originalKey"sv), originalKey, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAddNote)


std::string_view amuse::SoundMacro::CmdAddNote::DNAType() {
  return "amuse::SoundMacro::CmdAddNote"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetNote::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("key"sv), key, s);
  Do<Op>(athena::io::PropId("detune"sv), detune, s);
  DoSeek<Op>(2, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetNote)


std::string_view amuse::SoundMacro::CmdSetNote::DNAType() {
  return "amuse::SoundMacro::CmdSetNote"sv;
}


template <class Op>
void amuse::SoundMacro::CmdLastNote::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("add"sv), add, s);
  Do<Op>(athena::io::PropId("detune"sv), detune, s);
  DoSeek<Op>(2, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdLastNote)


std::string_view amuse::SoundMacro::CmdLastNote::DNAType() {
  return "amuse::SoundMacro::CmdLastNote"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPortamento::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("portState"sv), portState, s);
  Do<Op>(athena::io::PropId("portType"sv), portType, s);
  DoSeek<Op>(2, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPortamento)


std::string_view amuse::SoundMacro::CmdPortamento::DNAType() {
  return "amuse::SoundMacro::CmdPortamento"sv;
}


template <class Op>
void amuse::SoundMacro::CmdVibrato::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("levelNote"sv), levelNote, s);
  Do<Op>(athena::io::PropId("levelFine"sv), levelFine, s);
  Do<Op>(athena::io::PropId("modwheelFlag"sv), modwheelFlag, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdVibrato)


std::string_view amuse::SoundMacro::CmdVibrato::DNAType() {
  return "amuse::SoundMacro::CmdVibrato"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPitchSweep1::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("times"sv), times, s);
  Do<Op>(athena::io::PropId("add"sv), add, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPitchSweep1)


std::string_view amuse::SoundMacro::CmdPitchSweep1::DNAType() {
  return "amuse::SoundMacro::CmdPitchSweep1"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPitchSweep2::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("times"sv), times, s);
  Do<Op>(athena::io::PropId("add"sv), add, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("msSwitch"sv), msSwitch, s);
  Do<Op>(athena::io::PropId("ticksOrMs"sv), ticksOrMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPitchSweep2)


std::string_view amuse::SoundMacro::CmdPitchSweep2::DNAType() {
  return "amuse::SoundMacro::CmdPitchSweep2"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetPitch::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("hz"sv), hz, s);
  Do<Op>(athena::io::PropId("fine"sv), fine, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetPitch)


std::string_view amuse::SoundMacro::CmdSetPitch::DNAType() {
  return "amuse::SoundMacro::CmdSetPitch"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetPitchAdsr::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("table"sv), table, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("keys"sv), keys, s);
  Do<Op>(athena::io::PropId("cents"sv), cents, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetPitchAdsr)


std::string_view amuse::SoundMacro::CmdSetPitchAdsr::DNAType() {
  return "amuse::SoundMacro::CmdSetPitchAdsr"sv;
}


template <class Op>
void amuse::SoundMacro::CmdScaleVolumeDLS::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("scale"sv), scale, s);
  Do<Op>(athena::io::PropId("originalVol"sv), originalVol, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdScaleVolumeDLS)


std::string_view amuse::SoundMacro::CmdScaleVolumeDLS::DNAType() {
  return "amuse::SoundMacro::CmdScaleVolumeDLS"sv;
}


template <class Op>
void amuse::SoundMacro::CmdMod2Vibrange::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("keys"sv), keys, s);
  Do<Op>(athena::io::PropId("cents"sv), cents, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdMod2Vibrange)


std::string_view amuse::SoundMacro::CmdMod2Vibrange::DNAType() {
  return "amuse::SoundMacro::CmdMod2Vibrange"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetupTremolo::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("scale"sv), scale, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("modwAddScale"sv), modwAddScale, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetupTremolo)


std::string_view amuse::SoundMacro::CmdSetupTremolo::DNAType() {
  return "amuse::SoundMacro::CmdSetupTremolo"sv;
}


template <class Op>
void amuse::SoundMacro::CmdReturn::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdReturn)


std::string_view amuse::SoundMacro::CmdReturn::DNAType() {
  return "amuse::SoundMacro::CmdReturn"sv;
}


template <class Op>
void amuse::SoundMacro::CmdGoSub::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdGoSub)


std::string_view amuse::SoundMacro::CmdGoSub::DNAType() {
  return "amuse::SoundMacro::CmdGoSub"sv;
}


template <class Op>
void amuse::SoundMacro::CmdTrapEvent::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("event"sv), event, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdTrapEvent)


std::string_view amuse::SoundMacro::CmdTrapEvent::DNAType() {
  return "amuse::SoundMacro::CmdTrapEvent"sv;
}


template <class Op>
void amuse::SoundMacro::CmdUntrapEvent::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("event"sv), event, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdUntrapEvent)


std::string_view amuse::SoundMacro::CmdUntrapEvent::DNAType() {
  return "amuse::SoundMacro::CmdUntrapEvent"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSendMessage::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("voiceVar"sv), voiceVar, s);
  Do<Op>(athena::io::PropId("valueVar"sv), valueVar, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSendMessage)


std::string_view amuse::SoundMacro::CmdSendMessage::DNAType() {
  return "amuse::SoundMacro::CmdSendMessage"sv;
}


template <class Op>
void amuse::SoundMacro::CmdGetMessage::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("variable"sv), variable, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdGetMessage)


std::string_view amuse::SoundMacro::CmdGetMessage::DNAType() {
  return "amuse::SoundMacro::CmdGetMessage"sv;
}


template <class Op>
void amuse::SoundMacro::CmdGetVid::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("variable"sv), variable, s);
  Do<Op>(athena::io::PropId("playMacro"sv), playMacro, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdGetVid)


std::string_view amuse::SoundMacro::CmdGetVid::DNAType() {
  return "amuse::SoundMacro::CmdGetVid"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAddAgeCount::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("add"sv), add, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAddAgeCount)


std::string_view amuse::SoundMacro::CmdAddAgeCount::DNAType() {
  return "amuse::SoundMacro::CmdAddAgeCount"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetAgeCount::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("counter"sv), counter, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetAgeCount)


std::string_view amuse::SoundMacro::CmdSetAgeCount::DNAType() {
  return "amuse::SoundMacro::CmdSetAgeCount"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSendFlag::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("flagId"sv), flagId, s);
  Do<Op>(athena::io::PropId("value"sv), value, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSendFlag)


std::string_view amuse::SoundMacro::CmdSendFlag::DNAType() {
  return "amuse::SoundMacro::CmdSendFlag"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPitchWheelR::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("rangeUp"sv), rangeUp, s);
  Do<Op>(athena::io::PropId("rangeDown"sv), rangeDown, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPitchWheelR)


std::string_view amuse::SoundMacro::CmdPitchWheelR::DNAType() {
  return "amuse::SoundMacro::CmdPitchWheelR"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetPriority::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("prio"sv), prio, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetPriority)


std::string_view amuse::SoundMacro::CmdSetPriority::DNAType() {
  return "amuse::SoundMacro::CmdSetPriority"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAddPriority::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("prio"sv), prio, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAddPriority)


std::string_view amuse::SoundMacro::CmdAddPriority::DNAType() {
  return "amuse::SoundMacro::CmdAddPriority"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAgeCntSpeed::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  DoSeek<Op>(3, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("time"sv), time, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAgeCntSpeed)


std::string_view amuse::SoundMacro::CmdAgeCntSpeed::DNAType() {
  return "amuse::SoundMacro::CmdAgeCntSpeed"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAgeCntVel::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("ageBase"sv), ageBase, s);
  Do<Op>(athena::io::PropId("ageScale"sv), ageScale, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAgeCntVel)


std::string_view amuse::SoundMacro::CmdAgeCntVel::DNAType() {
  return "amuse::SoundMacro::CmdAgeCntVel"sv;
}


template <class Op>
void amuse::SoundMacro::CmdVolSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdVolSelect)


std::string_view amuse::SoundMacro::CmdVolSelect::DNAType() {
  return "amuse::SoundMacro::CmdVolSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPanSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPanSelect)


std::string_view amuse::SoundMacro::CmdPanSelect::DNAType() {
  return "amuse::SoundMacro::CmdPanSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPitchWheelSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPitchWheelSelect)


std::string_view amuse::SoundMacro::CmdPitchWheelSelect::DNAType() {
  return "amuse::SoundMacro::CmdPitchWheelSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdModWheelSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdModWheelSelect)


std::string_view amuse::SoundMacro::CmdModWheelSelect::DNAType() {
  return "amuse::SoundMacro::CmdModWheelSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPedalSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPedalSelect)


std::string_view amuse::SoundMacro::CmdPedalSelect::DNAType() {
  return "amuse::SoundMacro::CmdPedalSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPortamentoSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPortamentoSelect)


std::string_view amuse::SoundMacro::CmdPortamentoSelect::DNAType() {
  return "amuse::SoundMacro::CmdPortamentoSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdReverbSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdReverbSelect)


std::string_view amuse::SoundMacro::CmdReverbSelect::DNAType() {
  return "amuse::SoundMacro::CmdReverbSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSpanSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSpanSelect)


std::string_view amuse::SoundMacro::CmdSpanSelect::DNAType() {
  return "amuse::SoundMacro::CmdSpanSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdDopplerSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdDopplerSelect)


std::string_view amuse::SoundMacro::CmdDopplerSelect::DNAType() {
  return "amuse::SoundMacro::CmdDopplerSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdTremoloSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdTremoloSelect)


std::string_view amuse::SoundMacro::CmdTremoloSelect::DNAType() {
  return "amuse::SoundMacro::CmdTremoloSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPreASelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPreASelect)


std::string_view amuse::SoundMacro::CmdPreASelect::DNAType() {
  return "amuse::SoundMacro::CmdPreASelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPreBSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPreBSelect)


std::string_view amuse::SoundMacro::CmdPreBSelect::DNAType() {
  return "amuse::SoundMacro::CmdPreBSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdPostBSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdPostBSelect)


std::string_view amuse::SoundMacro::CmdPostBSelect::DNAType() {
  return "amuse::SoundMacro::CmdPostBSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAuxAFXSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
  Do<Op>(athena::io::PropId("paramIndex"sv), paramIndex, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAuxAFXSelect)


std::string_view amuse::SoundMacro::CmdAuxAFXSelect::DNAType() {
  return "amuse::SoundMacro::CmdAuxAFXSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAuxBFXSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("midiControl"sv), midiControl, s);
  Do<Op>(athena::io::PropId("scalingPercentage"sv), scalingPercentage, s);
  Do<Op>(athena::io::PropId("combine"sv), combine, s);
  Do<Op>(athena::io::PropId("isVar"sv), isVar, s);
  Do<Op>(athena::io::PropId("fineScaling"sv), fineScaling, s);
  Do<Op>(athena::io::PropId("paramIndex"sv), paramIndex, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAuxBFXSelect)


std::string_view amuse::SoundMacro::CmdAuxBFXSelect::DNAType() {
  return "amuse::SoundMacro::CmdAuxBFXSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetupLFO::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("lfoNumber"sv), lfoNumber, s);
  Do<Op>(athena::io::PropId("periodInMs"sv), periodInMs, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetupLFO)


std::string_view amuse::SoundMacro::CmdSetupLFO::DNAType() {
  return "amuse::SoundMacro::CmdSetupLFO"sv;
}


template <class Op>
void amuse::SoundMacro::CmdModeSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("dlsVol"sv), dlsVol, s);
  Do<Op>(athena::io::PropId("itd"sv), itd, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdModeSelect)


std::string_view amuse::SoundMacro::CmdModeSelect::DNAType() {
  return "amuse::SoundMacro::CmdModeSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetKeygroup::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("group"sv), group, s);
  Do<Op>(athena::io::PropId("killNow"sv), killNow, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetKeygroup)


std::string_view amuse::SoundMacro::CmdSetKeygroup::DNAType() {
  return "amuse::SoundMacro::CmdSetKeygroup"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSRCmodeSelect::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("srcType"sv), srcType, s);
  Do<Op>(athena::io::PropId("type0SrcFilter"sv), type0SrcFilter, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSRCmodeSelect)


std::string_view amuse::SoundMacro::CmdSRCmodeSelect::DNAType() {
  return "amuse::SoundMacro::CmdSRCmodeSelect"sv;
}


template <class Op>
void amuse::SoundMacro::CmdWiiUnknown::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("flag"sv), flag, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdWiiUnknown)


std::string_view amuse::SoundMacro::CmdWiiUnknown::DNAType() {
  return "amuse::SoundMacro::CmdWiiUnknown"sv;
}


template <class Op>
void amuse::SoundMacro::CmdWiiUnknown2::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("flag"sv), flag, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdWiiUnknown2)


std::string_view amuse::SoundMacro::CmdWiiUnknown2::DNAType() {
  return "amuse::SoundMacro::CmdWiiUnknown2"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAddVars::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("varCtrlA"sv), varCtrlA, s);
  Do<Op>(athena::io::PropId("a"sv), a, s);
  Do<Op>(athena::io::PropId("varCtrlB"sv), varCtrlB, s);
  Do<Op>(athena::io::PropId("b"sv), b, s);
  Do<Op>(athena::io::PropId("varCtrlC"sv), varCtrlC, s);
  Do<Op>(athena::io::PropId("c"sv), c, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAddVars)


std::string_view amuse::SoundMacro::CmdAddVars::DNAType() {
  return "amuse::SoundMacro::CmdAddVars"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSubVars::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("varCtrlA"sv), varCtrlA, s);
  Do<Op>(athena::io::PropId("a"sv), a, s);
  Do<Op>(athena::io::PropId("varCtrlB"sv), varCtrlB, s);
  Do<Op>(athena::io::PropId("b"sv), b, s);
  Do<Op>(athena::io::PropId("varCtrlC"sv), varCtrlC, s);
  Do<Op>(athena::io::PropId("c"sv), c, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSubVars)


std::string_view amuse::SoundMacro::CmdSubVars::DNAType() {
  return "amuse::SoundMacro::CmdSubVars"sv;
}


template <class Op>
void amuse::SoundMacro::CmdMulVars::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("varCtrlA"sv), varCtrlA, s);
  Do<Op>(athena::io::PropId("a"sv), a, s);
  Do<Op>(athena::io::PropId("varCtrlB"sv), varCtrlB, s);
  Do<Op>(athena::io::PropId("b"sv), b, s);
  Do<Op>(athena::io::PropId("varCtrlC"sv), varCtrlC, s);
  Do<Op>(athena::io::PropId("c"sv), c, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdMulVars)


std::string_view amuse::SoundMacro::CmdMulVars::DNAType() {
  return "amuse::SoundMacro::CmdMulVars"sv;
}


template <class Op>
void amuse::SoundMacro::CmdDivVars::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("varCtrlA"sv), varCtrlA, s);
  Do<Op>(athena::io::PropId("a"sv), a, s);
  Do<Op>(athena::io::PropId("varCtrlB"sv), varCtrlB, s);
  Do<Op>(athena::io::PropId("b"sv), b, s);
  Do<Op>(athena::io::PropId("varCtrlC"sv), varCtrlC, s);
  Do<Op>(athena::io::PropId("c"sv), c, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdDivVars)


std::string_view amuse::SoundMacro::CmdDivVars::DNAType() {
  return "amuse::SoundMacro::CmdDivVars"sv;
}


template <class Op>
void amuse::SoundMacro::CmdAddIVars::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("varCtrlA"sv), varCtrlA, s);
  Do<Op>(athena::io::PropId("a"sv), a, s);
  Do<Op>(athena::io::PropId("varCtrlB"sv), varCtrlB, s);
  Do<Op>(athena::io::PropId("b"sv), b, s);
  Do<Op>(athena::io::PropId("imm"sv), imm, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdAddIVars)


std::string_view amuse::SoundMacro::CmdAddIVars::DNAType() {
  return "amuse::SoundMacro::CmdAddIVars"sv;
}


template <class Op>
void amuse::SoundMacro::CmdSetVar::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("varCtrlA"sv), varCtrlA, s);
  Do<Op>(athena::io::PropId("a"sv), a, s);
  DoSeek<Op>(1, athena::SeekOrigin::Current, s);
  Do<Op>(athena::io::PropId("imm"sv), imm, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdSetVar)


std::string_view amuse::SoundMacro::CmdSetVar::DNAType() {
  return "amuse::SoundMacro::CmdSetVar"sv;
}


template <class Op>
void amuse::SoundMacro::CmdIfEqual::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("varCtrlA"sv), varCtrlA, s);
  Do<Op>(athena::io::PropId("a"sv), a, s);
  Do<Op>(athena::io::PropId("varCtrlB"sv), varCtrlB, s);
  Do<Op>(athena::io::PropId("b"sv), b, s);
  Do<Op>(athena::io::PropId("notEq"sv), notEq, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdIfEqual)


std::string_view amuse::SoundMacro::CmdIfEqual::DNAType() {
  return "amuse::SoundMacro::CmdIfEqual"sv;
}


template <class Op>
void amuse::SoundMacro::CmdIfLess::Enumerate(typename Op::StreamT& s) {
  amuse::SoundMacro::ICmd::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("varCtrlA"sv), varCtrlA, s);
  Do<Op>(athena::io::PropId("a"sv), a, s);
  Do<Op>(athena::io::PropId("varCtrlB"sv), varCtrlB, s);
  Do<Op>(athena::io::PropId("b"sv), b, s);
  Do<Op>(athena::io::PropId("notLt"sv), notLt, s);
  Do<Op>(athena::io::PropId("macroStep"sv), macroStep, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::SoundMacro::CmdIfLess)


std::string_view amuse::SoundMacro::CmdIfLess::DNAType() {
  return "amuse::SoundMacro::CmdIfLess"sv;
}


template <class Op>
void amuse::ITable::Enumerate(typename Op::StreamT& s) {
}

AT_SPECIALIZE_DNA_YAML(amuse::ITable)


std::string_view amuse::ITable::DNAType() {
  return "amuse::ITable"sv;
}


template <class Op>
void amuse::ADSR::Enumerate(typename Op::StreamT& s) {
  amuse::ITable::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("attack"sv), attack, s);
  Do<Op>(athena::io::PropId("decay"sv), decay, s);
  Do<Op>(athena::io::PropId("sustain"sv), sustain, s);
  Do<Op>(athena::io::PropId("release"sv), release, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::ADSR)


std::string_view amuse::ADSR::DNAType() {
  return "amuse::ADSR"sv;
}


template <class Op>
void amuse::ADSRDLS::Enumerate(typename Op::StreamT& s) {
  amuse::ITable::Enumerate<Op>(s);
  Do<Op>(athena::io::PropId("attack"sv), attack, s);
  Do<Op>(athena::io::PropId("decay"sv), decay, s);
  Do<Op>(athena::io::PropId("sustain"sv), sustain, s);
  Do<Op>(athena::io::PropId("release"sv), release, s);
  Do<Op>(athena::io::PropId("velToAttack"sv), velToAttack, s);
  Do<Op>(athena::io::PropId("keyToDecay"sv), keyToDecay, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::ADSRDLS)


std::string_view amuse::ADSRDLS::DNAType() {
  return "amuse::ADSRDLS"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::KeymapDNA<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("transpose"sv), transpose, s);
  Do<Op>(athena::io::PropId("pan"sv), pan, s);
  Do<Op>(athena::io::PropId("prioOffset"sv), prioOffset, s);
  DoSeek<Op>(3, athena::SeekOrigin::Current, s);
}

AT_SPECIALIZE_DNA(amuse::KeymapDNA<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::KeymapDNA<athena::Endian::Little>)


template <>
std::string_view amuse::KeymapDNA<athena::Endian::Big>::DNAType() {
  return "amuse::KeymapDNA<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::KeymapDNA<athena::Endian::Little>::DNAType() {
  return "amuse::KeymapDNA<athena::Endian::Little>"sv;
}


template <class Op>
void amuse::Keymap::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("transpose"sv), transpose, s);
  Do<Op>(athena::io::PropId("pan"sv), pan, s);
  Do<Op>(athena::io::PropId("prioOffset"sv), prioOffset, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::Keymap)


std::string_view amuse::Keymap::DNAType() {
  return "amuse::Keymap"sv;
}


template <athena::Endian DNAEn>
template <class Op>
void amuse::LayerMappingDNA<DNAEn>::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("keyLo"sv), keyLo, s);
  Do<Op>(athena::io::PropId("keyHi"sv), keyHi, s);
  Do<Op>(athena::io::PropId("transpose"sv), transpose, s);
  Do<Op>(athena::io::PropId("volume"sv), volume, s);
  Do<Op>(athena::io::PropId("prioOffset"sv), prioOffset, s);
  Do<Op>(athena::io::PropId("span"sv), span, s);
  Do<Op>(athena::io::PropId("pan"sv), pan, s);
  DoSeek<Op>(3, athena::SeekOrigin::Current, s);
}

AT_SPECIALIZE_DNA(amuse::LayerMappingDNA<athena::Endian::Big>)
AT_SPECIALIZE_DNA(amuse::LayerMappingDNA<athena::Endian::Little>)


template <>
std::string_view amuse::LayerMappingDNA<athena::Endian::Big>::DNAType() {
  return "amuse::LayerMappingDNA<athena::Endian::Big>"sv;
}
template <>
std::string_view amuse::LayerMappingDNA<athena::Endian::Little>::DNAType() {
  return "amuse::LayerMappingDNA<athena::Endian::Little>"sv;
}


template <class Op>
void amuse::LayerMapping::Enumerate(typename Op::StreamT& s) {
  Do<Op>(athena::io::PropId("macro"sv), macro, s);
  Do<Op>(athena::io::PropId("keyLo"sv), keyLo, s);
  Do<Op>(athena::io::PropId("keyHi"sv), keyHi, s);
  Do<Op>(athena::io::PropId("transpose"sv), transpose, s);
  Do<Op>(athena::io::PropId("volume"sv), volume, s);
  Do<Op>(athena::io::PropId("prioOffset"sv), prioOffset, s);
  Do<Op>(athena::io::PropId("span"sv), span, s);
  Do<Op>(athena::io::PropId("pan"sv), pan, s);
}

AT_SPECIALIZE_DNA_YAML(amuse::LayerMapping)


std::string_view amuse::LayerMapping::DNAType() {
  return "amuse::LayerMapping"sv;
}


