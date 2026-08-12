# Handoff

Last updated: 2026-08-12

## Where things stand

- [CURRENT_STATE.md](CURRENT_STATE.md) is the authoritative 2026-08-12 state.
  It records the exact pins, artifacts, hash, proof boundaries, blockers, and
  remaining checklist. Older limitations below are superseded where that file
  cites newer evidence.
- 2026-08-12 physical attempt: the exact prerequisites were re-verified and are
  still missing — no valid Apple Development identity, no provisioning-profile
  directory, no connected iPad/iPhone. The scoped Gate 2/3 pass therefore did
  not start; no engine/UI change was made (blocked, not failed). Evidence:
  `/tmp/ectopad-gate2-prereq-blocked-2026-08-12.md`.

- Gate 1 macOS is proven. iPad Simulator reaches Dawn/WebGPU → Metal, the full
  New Game flow, Frigate gameplay, audio, saves, and lifecycle handling.
- KI-015 is fixed: the SunPad overlay bridge had been shadowed by a non-iOS stub
  linked first from Aurora's static archive. The stub now compiles only on
  non-iOS targets. Both macOS and iOS Simulator presets rebuild successfully;
  current iPad Simulator evidence is
  `/tmp/ki015-overlay-fixed-2026-08-12.{png,log}`.
- The overlay/settings/input sources are audited against SunPad `7d84cec`; see
  [SUNPAD_PARITY.md](SUNPAD_PARITY.md). A launch-only UIKit harness now passes
  all menu/settings/editor/mixer/picker phases (`result=0`) and the live bridge
  reports the `•••` visible and attached. This is deterministic current-build
  wiring evidence; finger and multitouch behavior still require hardware.
- The virtual GameController test path is now verified end-to-end. After fixing
  SDL3 invalid-ID handling and descriptor initialization in the opt-in hook,
  Start, left-stick Y, and A reached the real Metroid frontend through normal
  SDL → Aurora → `PADRead` handling. This is software-path evidence only;
  physical Apple GameController behavior remains untested.
- SunPad's real render-scale, aspect-presentation, and FPS actions are now wired
  behind the unchanged menu source. Simulator evidence proves 2× 4:3
  (`1280x960`), 2× experimental 16:9 (`1707x960`), and the live
  presented-frame label. The Experimental 60 FPS switch remains intentionally
  unwired for GM8E01. Controller Mapping now uses SunPad's byte-identical store
  and reference five-button UI; Simulator persistence/permutation/UI evidence is
  complete, while physical-controller behavior remains untested.
- Game Data & Saves is now real behind the unchanged SunPad UI: Files picker and
  Documents-folder imports validate exact GM8E01 Rev 2, stage privately, verify
  canonical SHA-1, and activate by same-directory atomic rename. Simulator
  service tests proved valid import/restart, save-preserving removal/reimport,
  invalid-size rejection, and full-size corrupt-SHA rejection without damage to
  the active image or save. The production picker and delegate now present in
  the current harness; actual physical Files selection/import remains.
- A physical arm64 iOS `Metaforce.app` now compiles and links. Aurora no longer
  consults host pkg-config for zstd while cross-compiling, which had injected a
  Homebrew macOS dylib into the iOS link. Install/launch remains blocked on
  user signing identity and hardware.
- Performance runs must close competing CPU/GPU-heavy apps and record load.
  The 20.4/59.9 Simulator samples were confounded and are functional FPS-label
  evidence only, not baselines.
- The requirement-level status is explicit in
  [COMPLETION_AUDIT.md](COMPLETION_AUDIT.md). Simulator UI wiring work is
  complete for this handoff. The next meaningful work is outside Simulator:
  development signing, physical install/Metal launch, then touch/audio/import/
  lifecycle acceptance. Gate 2/3 requires a signing identity and hardware.
- Strict mobile presentation is cleaner after the audit: iOS no longer renders
  Metaforce's desktop ImGui bar/windows/toasts/debug overlays under SunPad. Clean
  iPhone and iPad Simulator screenshots show the unchanged SunPad layer alone;
  macOS ImGui behavior is unchanged.
- The historical session detail remains in
  [STOP_FOR_TODAY_2026-08-12.md](STOP_FOR_TODAY_2026-08-12.md); use
  [CURRENT_STATE.md](CURRENT_STATE.md) for resumption. The current-build
  interaction harness passes all phases (`result=0`) and the
  bridge reasserts the SunPad layer over SDL once per second so `•••` remains
  visible. Audio production now targets a measured 120 ms output-ready reserve
  rather than assuming a 60 FPS render cadence; a loaded native run held
  5,292–5,294 output frames with 6–7 active voices and no underrun log.

## How to resume

1. Read [CURRENT_STATE.md](CURRENT_STATE.md), then [STATUS.md](STATUS.md)
   (status legend included). Use the stop-for-today file only as a historical
   session snapshot.
2. Read [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) for the plan and gates.
3. Follow [INSTALL_IPA.md](INSTALL_IPA.md) and [BUILDING.md](BUILDING.md) only
   after a valid development identity and physical device are available.
4. Append dated evidence to [TESTING.md](TESTING.md) and [PERFORMANCE.md](PERFORMANCE.md).
5. Before any FPS comparison, close unrelated CPU/GPU-heavy apps and verify no
   build, renderer, or extra Simulator process is competing; record load.
6. Do not repeat Simulator menu automation unless a physical result exposes a
   reproducible product defect.

## Workspace invariants

- `ref/` and all game-derived data are git-ignored and must stay local.
- Before any commit, inspect staged files for game data, generated copyrighted
  files, saves, logs with sensitive data, provisioning profiles, certificates,
  or signing material.
- Preserve unrelated user changes; do not rewrite history unnecessarily.
