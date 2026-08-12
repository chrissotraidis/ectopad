# Performance

Last updated: 2026-08-12

## Intent

- Measure frame timing, internal/drawable resolution, render scale, and adapter
  info on real Apple hardware (macOS, physical iPhone, physical iPad).
- Track memory use in Frigate Orpheon and representative later areas, plus
  long-session behavior (thermals, memory pressure).
- Implement SunPad-style live 1×/2×/3×/4× render-scale controls where Metaforce
  supports them, with original aspect presentation as the safe default.

## Baseline

No controlled performance baseline has been recorded yet. Existing 60 FPS
gameplay observations are functional evidence, not benchmark results.

## Measurement discipline

- Before a performance run, close unrelated CPU/GPU-heavy applications and
  verify no competing render, build, encoding, or Simulator process is active.
- Record the target, scene, render scale/aspect, elapsed sample window, FPS or
  frame-time series, thermal state, and a process/system-load snapshot with the
  result.
- Treat a run with material competing load as invalid for performance
  comparison, while retaining any independent functional evidence it provides.
- The 2026-08-12 iPad Simulator display-wiring screenshots showed 20.4 FPS with
  another heavy app running and 59.9 FPS after it was closed. This demonstrates
  the confounder and the live FPS display, but neither number is a baseline or a
  physical-device performance claim.
- A loaded native audio-reserve run conducted while unrelated renderer load was
  high held 5,292–5,294 output-ready frames and logged no underrun. That is valid
  mechanical audio-buffer evidence, but its FPS and timing are excluded from
  performance baselines for the same reason.
