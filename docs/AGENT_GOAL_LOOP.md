# Ectopad Agent Goal Loop (handoff for the next agent)

This file is the standing goal-based loop for the agent working on Ectopad
(Metroid Prime → Apple Silicon via Metaforce). It replaces the plain objective
statement: it keeps the full mission, adds a mandatory **review pass** over the
prior agent's work, and elevates **SunPad parity** (menus + touch controls that
are virtually identical to SunPad, **reusing SunPad's code directly**) to a
first-class requirement that every turn checks.

## 1. Mission (unchanged from the original objective)

Build Metroid Prime (NTSC-U Rev 2, GM8E01, CRC 61592372) natively for Apple
Silicon macOS, iOS, and iPadOS using **Metaforce** (Dawn/WebGPU → Metal), driven
agentically from research through implementation, testing, documentation, and
final validation. The engine must execute as Metaforce through Dawn/Metal — no
Dolphin/RetroArch wrapper.

Feasibility gates, in order:
- **Gate 1 (macOS ARM64):** clean build, game data validated/prepared, launch,
  Dawn selects Metal, frame renders, startup/title flow, input, audio,
  saves/config. → **PROVEN** (see §4).
- **Gate 2 (iOS/iPadOS hard go/no-go):** physical ARM64 device build installs,
  launches, Dawn/WebGPU reaches Metal, loads prepared Prime data, visible
  rendering. **Simulator is proven; physical device is the remaining unknown**
  (blocked on user: signing identity + hardware).
- **Gate 3 (real gameplay):** physical iPad through Frigate Orpheon and
  representative later areas — rendering, audio, movement, camera, aiming,
  lock-on, Morph Ball, beams, missiles, jumping, scanning, menus, pause,
  save/reload, lifecycle, memory headroom.

Full control surface: complete GameCube controls with a Prime-native touch
design; GameController support; macOS keyboard/mouse. Game data: user-owned
image validation + private staging + atomic activation, never bundling
Nintendo assets. Diagnostics: SunPad-quality Share Diagnostic Log. Keep all
docs current with dated evidence, separating proven / partially proven /
Simulator-only / device-verified / blocked / not-yet-tested. Frequent focused
commits; audit for prohibited content (game data, signing material).

Completion only when macOS/iPhone/iPad native builds work, Metaforce executes
through Dawn/Metal, user-owned data validation/preparation works, Frigate +
later areas are stable, core progression is not blocked by Apple-port bugs,
touch/controller/handoff/lifecycle work, memory and performance are measured,
no known Apple-port blockers remain, and release artifacts contain no
prohibited data or secrets.

## 2. STANDING MANDATE: SunPad parity (menus + touch controls)

The user has repeatedly and explicitly required: **the touch controls, the
settings/menu overlays, and everything overlaid must be virtually identical to
SunPad (`ref/sunpad`), and the implementation must REUSE SunPad's code
directly (copy/port it) rather than reimplementing the same idea from
scratch.** A prior agent's ImGui reimplementation was rejected as "bad" for
this reason.

Rules that bind every turn:
1. **Reference SunPad's code constantly.** Before designing or touching any
   overlay, menu, settings, touch control, editor, or diagnostic UI, read the
   corresponding SunPad file in `ref/sunpad/apple/` and port it.
2. **Port, don't reinvent.** Copy `SunPadGameOverlay.{h,mm}`,
   `SunPadSettings.{h,mm}`, `SunPadInputMixer.{h,mm}`, `SunPadInputState.h`,
   `SunPadDiagnostics.{h,mm}` into the Metaforce iOS app and keep them as
   close to upstream as possible. Only minimal glue may change (the input
   boundary → aurora's pad layer; app-name strings; wiring stubs for actions
   not yet implemented).
3. **Keep the port in sync.** If SunPad changes, update the port. Track the
   mapping in `docs/SUNPAD_PARITY.md` (create it) and keep it current.
4. **The ••• menu** (Render Resolution, Aspect Ratio, FPS counter, Touch
   Control Settings, Game Data & Saves, Share Diagnostic Log, Controller
   Mapping) and the **Touch Control Settings panel** (opacity, size,
   per-control size, hide-on-controller, modern C-stick) and the **layout
   editor** (drag to reposition, per-control resize, reset) must match
   SunPad's UI. Wire the actions to real engine behavior as features land;
   never remove or restyle the menu.
5. The UIKit overlay already ported lives in
   `ref/metaforce/extern/aurora/lib/ios/` (SunPad files nearly verbatim) with
   a thin bridge (`OverlayBridge.mm`) that attaches it to the SDL window and
   feeds `aurora::touch::consume_ios_touch_state()` → the virtual GameCube
   pad. **This is the model to follow for every future UI piece.**

## 3. Mandatory review pass (start every turn here)

The prior agent's work is **not trustworthy until re-verified**. Every turn
begins with an evidence-based audit:
1. `git -C /Users/chrissotraidis/GitHub/ectopad status` — the ectopad repo must
   stay clean except docs/ + patches/ (never commit `ref/`).
2. Inspect the aurora working tree
   (`ref/metaforce/extern/aurora`): `git status --short` and the diff vs the
   pinned `5143394` — understand every uncommitted change.
3. Re-verify KI-015 remains fixed. Its root cause was duplicate static-library
   implementations: `ios_touch_stub.cpp` was compiled for iOS and linked before
   `OverlayBridge.mm`, so the real bridge was never loaded. CMake now includes
   the stub only for non-iOS builds. Current evidence:
   `/tmp/ki015-overlay-fixed-2026-08-12.{png,log}`.
4. Rebuild both presets (`macos-default-relwithdebinfo`, `ios-sim`) and run
   the app to verify each claimed result before relying on it. Verify with
   dated screenshots + logs in `/tmp`.
5. Re-check every "Proven" claim in `docs/STATUS.md` before building on it.

## 4. Current state (2026-08-12, from prior agent)

Proven (with dated evidence in `/tmp`):
- Gate 1 macOS: build, Metal, title, full New Game → Frigate Orpheon gameplay
  60 FPS, keyboard/mouse input, audio (amuse + soxr + streamed DSP/MIDI),
  save/reload round trip. `docs/STATUS.md` has the detail.
- iPad Simulator (only ONE simulator at a time; UDID
  `D80E9862-C29A-4D69-B8E5-D81D396C17D5`): title, `--autostart` New Game flow,
  Frigate gameplay, saves persisted, background/foreground lifecycle (after the
  KI-013 aurora `DID_ENTER_BACKGROUND` fix), Dawn→Metal on "Apple iOS
  simulator GPU".
- **SunPad UIKit touch overlay ported** into
  `ref/metaforce/extern/aurora/lib/ios/` (SunPadGameOverlay.mm + Settings +
  InputMixer + InputState + Diagnostics, nearly verbatim) + `OverlayBridge.mm`
  + wiring (cmake, aurora.cpp attach, pad.cpp consume, touch.hpp
  `IosTouchState`). It **rendered identically to SunPad** (green A, red B,
  gray X/Y, purple Z, gold C-stick, D-pad, L/R shoulders, START, ••• menu).
- **iOS shutdown crash fixed** (crash report the user attached): exit hung in
  `SDL_CloseAudioDevice` (audio stream destroyed before the device close
  stalled the SDL audio thread), which prevented the normal teardown that
  clears the gx texture cache → an XPC abort during static destruction. Fix:
  reorder `aurora_audio_close` to close the device before destroying the
  stream. Verified: `simctl terminate` returns 0 with no new crash report,
  3×.

Known open items:
- **Simulator host touch forwarding is currently broken** (mouse clicks in the
  device area stopped producing touches mid-session; the Simulator's
  "Send Pointer to Device" state is stuck; clicks on the Simulator's own UI
  work). This is a host/test issue, not app code — document and retest on a
  fresh Simulator/device; the app-side input path (mixer → pad → game) is
  SunPad's own code.
- Virtual gamepad test hook (env `AURORA_VIRTUAL_GAMEPAD=1`, FIFO
  `/tmp/aurora_vg_cmds` in `lib/input.cpp`) — verified 2026-08-12 after fixing
  SDL3 invalid-ID handling and descriptor initialization. Start, analog Y, and
  A reached the real frontend through SDL → Aurora → `PADRead`. Physical Apple
  GameController hardware, hot-plug, rumble, and handoff remain untested.
- Files import/validation + atomic staging, Share Diagnostic Log wiring (menu
  present, actions stubbed), physical-device Gate 2/3 (needs user: signing
  identity + hardware), memory/performance measurements, later-area
  playthroughs.

## 5. Working process (keep)

- Mirror every `ref/metaforce`/aurora edit to `patches/` before committing
  (convention: `patches/YYYY-MM-DD-<repo>-<change>.patch`, diffs against the
  pinned commit; the aurora pin is `5143394`, metaforce `621ee0f`). Validate
  patches apply in sequence on a clean worktree.
- Keep `docs/` current with dated evidence: STATUS, IMPLEMENTATION_PLAN,
  ARCHITECTURE, DEPENDENCIES, BUILDING, GAME_DATA, TESTING, PERFORMANCE,
  KNOWN_ISSUES, LEGAL_AND_PROVENANCE, HANDOFF.
- Frequent focused commits; audit staged files for prohibited content (game
  data, saves, signing).
- Run only ONE Simulator at a time; prefer incremental builds
  (`cmake --build build/ios-sim -j8`); the iOS install step is
  `simctl install build/ios-sim/Binaries/Metaforce.app` (the `install` target
  compresses a huge dSYM — skip it).

## 6. Next priorities (in order)

1. Restore Simulator host touch delivery or verify on a physical device, then
   re-test the `•••` menu, settings panel, editor, and mixer path on the current
   KI-015-fixed build.
2. Wire the ••• menu actions to real behavior (render scale/aspect/FPS to the
   engine; game data & saves to the import flow; Share Diagnostic Log to the
   ported diagnostics).
3. Physical GameController verification (Apple GCController via SDL): the
   no-hardware SDL/Aurora/PAD path is proven; hardware discovery, reconnect,
   rumble, and touch/controller handoff still require a controller/device.
4. Files import/validation + atomic staging (SunPad reference), save/data
   separation.
5. Physical-device Gate 2/3 (needs user prerequisites).
6. Memory/performance measurements; later-area playthroughs.

See `docs/STATUS.md`, `docs/KNOWN_ISSUES.md`, `docs/IMPLEMENTATION_PLAN.md`,
and `docs/HANDOFF.md` for the full evidence trail.
