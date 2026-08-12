# Handoff

Last updated: 2026-08-12

## Where things stand

- Gate 1 macOS is proven. iPad Simulator reaches Dawn/WebGPU → Metal, the full
  New Game flow, Frigate gameplay, audio, saves, and lifecycle handling.
- KI-015 is fixed: the SunPad overlay bridge had been shadowed by a non-iOS stub
  linked first from Aurora's static archive. The stub now compiles only on
  non-iOS targets. Both macOS and iOS Simulator presets rebuild successfully;
  current iPad Simulator evidence is
  `/tmp/ki015-overlay-fixed-2026-08-12.{png,log}`.
- The overlay/settings/input sources are audited against SunPad `7d84cec`; see
  [SUNPAD_PARITY.md](SUNPAD_PARITY.md). Current Simulator host clicks still do
  not deliver touch, so current-build menu/editor interaction remains blocked
  by test infrastructure rather than claimed as verified.
- The virtual GameController test path is now verified end-to-end. After fixing
  SDL3 invalid-ID handling and descriptor initialization in the opt-in hook,
  Start, left-stick Y, and A reached the real Metroid frontend through normal
  SDL → Aurora → `PADRead` handling. This is software-path evidence only;
  physical Apple GameController behavior remains untested.
- Next unblocked work: wire the SunPad menu actions and implement private
  validated/atomic game-data import. Gate 2/3 physical-device work still
  requires signing identity and hardware.

## How to resume

1. Read [STATUS.md](STATUS.md) first (status legend included).
2. Read [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) for the plan and gates.
3. Follow [BUILDING.md](BUILDING.md) to configure/build.
4. Append dated evidence to [TESTING.md](TESTING.md) and [PERFORMANCE.md](PERFORMANCE.md).

## Workspace invariants

- `ref/` and all game-derived data are git-ignored and must stay local.
- Before any commit, inspect staged files for game data, generated copyrighted
  files, saves, logs with sensitive data, provisioning profiles, certificates,
  or signing material.
- Preserve unrelated user changes; do not rewrite history unnecessarily.
