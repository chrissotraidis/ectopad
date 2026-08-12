# Testing

Last updated: 2026-08-12

## Discipline

- Run only **one** iOS Simulator at a time.
- Close unrelated CPU/GPU-heavy apps before performance-sensitive runs and
  record process/system load; otherwise invalidate the performance comparison.
- Record dated, command-level evidence for every claim.
- Never classify a Simulator-only failure as a device blocker without testing on
  the device architecture.
- A title screen is not completion; a rendered room is not completion; Frigate
  Orpheon alone is not completion.

## Test matrix (tracked here as executed)

| Target | Launch | Import UI | Rendering | Touch | Controller | Gameplay | Saves | Audio | Lifecycle |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| macOS ARM64 | ☑ | — | ☑ | — | ◐ software path | ☑ | ☑ | ☑ | — |
| iPhone Simulator | ☐ | ☐ | ☐ | ☐ | — | ☐ | ☐ | ☐ | ☐ |
| iPad Simulator | ☑ | ☐ | ☑ | ☐ | — | ☐ | ☐ | ☑ | ☐ |
| Physical iPhone | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ |
| Physical iPad | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ |

## Gameplay verification scope (when reachable)

- Frigate Orpheon opening sequence (Gate 3): movement, camera, aiming, lock-on,
  Morph Ball, beam firing, missiles, jumping, scanning, menus, pause,
  save/reload, lifecycle recovery, memory headroom.
- Representative later areas: Chozo Ruins, Tallon Overworld, Phendrana Drifts,
  Magmoor Caverns, Phazon Mines.

## Evidence ledger

### 2026-08-11 — macOS ARM64 (Gate 1)

- Build: `cmake --preset macos-default-relwithdebinfo` +
  `cmake --build --preset macos-default-relwithdebinfo` → 862/862 steps OK,
  `Binaries/Metaforce.app` (arm64 Mach-O, Metal.framework weak-linked, 25 MB).
- Boot (no dev console): `Metaforce -l <iso>` — Metal selected, 60.1 FPS,
  disc identified, assets loaded, no errors.

### 2026-08-11 — iPad Pro 13-inch (M5) Simulator, audio stack

- Rebuilt `build/ios-sim` with the vendored audio stack (amuse/athena/logvisor/soxr):
  clean configure + build (2522 steps) + install; app linked for the sim SDK.
- Launched `com.axiodl.Metaforce` with the user ISO staged in the app container:
  Metal adapter "Apple iOS simulator GPU", amuse engine initialized (SDL backend,
  32 kHz), audio groups loaded into amuse, frontend RSF music
  (`frontend_1.rsf`/`frontend_2.rsf`) streaming, `SDLBackend: 2 voices (2 running),
  3 submixes`, 534-frame audio pump steady, no underruns.
- Screenshot: full-frame Metroid Prime title screen (gold logo, [PRESS START],
  4:3). Evidence: log `/tmp/mf_ios_sim_audio.log`; screenshots
  `/tmp/ios_audio_check2.png`, `/tmp/ios_audio_final.png`.

### 2026-08-11 — macOS save/reload cycle (KI-011 fix)

- New game from the file select created a save slot (`GM8E01` / `MetroidPrime B`)
  and persisted it to the raw Dolphin card
  (`~/Library/Application Support/dolphin-emu/GC/MemoryCardA.USA.raw`, directory
  entry at offset 0x2040) — previously the card was never written because
  kabufuda indexed the AIO queue with block numbers beyond the 5-slot queue
  (`Card::open` → `resizeQueue(5)`) and `AsyncIO::~AsyncIO` cancelled pending
  aio writes.
- Clean SIGINT exit → relaunch → file select showed the save → pressing A
  loaded the saved game (intro narration card → cinematic at 60 FPS, 10+ audio
  voices). Evidence: screenshots `/tmp/mf_reload_a2.png`,
  `/tmp/mf_reload_game.png`; logs `/tmp/mf_savefix.log`, `/tmp/mf_reload.log`.

### 2026-08-11 — iPad Pro 13-inch (M5) Simulator, in-game + save (Gate 2 de-risk)

- Rebuilt `build/ios-sim` with the audio stack, CInputStream byte-order fix
  (KI-009), and kabufuda card-persistence fix (KI-011); installed and launched.
- `--warp 2 2`: rendered **Tallon Overworld in-game** (rocky ruins, arm cannon,
  visor HUD with ENERGY 99, radar, minimap) through Dawn→Metal on the simulator.
- `--autostart` (new test hook): drove title → file select → new game → intro
  cinematic → **first-person Frigate Orpheon gameplay** (ENERGY 99, missiles 15,
  radar, HUD icons) at 60 FPS with no host input, and persisted the
  `GM8E01`/`MetroidPrime B` save to the sim container's card
  (`…/Library/Application Support/dolphin-emu/GC/MemoryCardA.USA.raw`, entry at
  offset 0x2040). Host keyboard forwarding to the Simulator GUI was not
  available, which motivated the hook. Evidence: `/tmp/ios_warp1.png`,
  `/tmp/ios_auto1.png`, `/tmp/ios_auto2.png`, `/tmp/ios_auto3.png`.

### 2026-08-12 — iOS Simulator touch input (foundation)

- Added SDL finger-event capture in aurora mapped to a virtual GameCube pad
  (left half = movement stick, right half = C-stick, bottom corners = A/B taps),
  fed through the existing `PADSetVirtualStatus` mechanism
  (superseded by the full touch overlay below).
- Verified on the iPad Pro 13-inch (M5) Simulator: with the game in Frigate
  Orpheon gameplay (via `--autostart`), a Simulator-GUI mouse drag on the left
  half of the screen (which the Simulator delivers as a touch) **moved Samus
  forward** — the view advanced past the doorway and the game's lock-on
  tutorial prompt ("Press and hold [L] to lock onto targets") appeared.
  Evidence: `/tmp/ios_touch1.png`, `/tmp/ios_touch_after.png`,

### 2026-08-12 — SunPad UIKit overlay port and KI-015

- The independent ImGui overlay was rejected and superseded. The current UI is
  a direct port of `SunPadGameOverlay`, `SunPadSettings`, `SunPadInputMixer`,
  `SunPadInputState`, and `SunPadDiagnostics`; see
  [SUNPAD_PARITY.md](SUNPAD_PARITY.md) for the file-by-file audit.
- Root-caused KI-015 without changing SunPad UI code. Both
  `ios_touch_stub.cpp` and `OverlayBridge.mm` defined
  `aurora::touch::attach_ios_overlay()` in `libaurora_core.a`. The stub's SDL
  platform macros were unavailable, so it emitted an iOS symbol; archive order
  caused the linker to select it and omit the bridge. CMake now includes the
  stub only for non-iOS targets.
- Rebuilt `ios-sim` successfully (2529 targets after CMake regenerated Dawn and
  SDL), installed `build/ios-sim/Binaries/Metaforce.app`, and launched with
  `--autostart` on the one booted iPad Pro 13-inch (M5) Simulator. Evidence:
  `[SunPad] session start`, Dawn adapter `Apple iOS simulator GPU`, unchanged
  SunPad controls over the live intro/game, audio active, and clean
  `simctl terminate` exit 0. Screenshot/log:
  `/tmp/ki015-overlay-fixed-2026-08-12.png`,
  `/tmp/ki015-overlay-fixed-2026-08-12.log`.
- The current Simulator host still did not forward device-area clicks to the
  app, so this run could not reopen the menu/settings. Earlier evidence
  `/tmp/ios_menu_open2.png` proves the directly ported menu rendered before the
  linker regression. Current-build menu interaction remains unverified rather
  than inferred.
- Rebuilt `macos-default-relwithdebinfo`; the app reached Dawn/WebGPU → Metal
  on Apple M2 and exited 0 after SIGINT.

### 2026-08-12 — macOS virtual GameController path

- Enabled the opt-in Aurora test hook with `AURORA_VIRTUAL_GAMEPAD=1`. It
  attaches an SDL3 standard virtual joystick and reads axis/button commands
  from `/tmp/aurora_vg_cmds`; no keyboard, mouse, touch, `--autostart`, or
  Metaforce-specific input injection participates after launch.
- Fixed two defects found before accepting the test: SDL3 joystick ID `0` is
  invalid (the unsigned ID had incorrectly used `-1` and signed comparisons),
  and `SDL_VirtualJoystickDesc` must use `SDL_INIT_INTERFACE` rather than a
  hard-coded version. The running app then logged both the virtual attachment
  and normal controller addition.
- With the validated GM8E01 Rev 2 image loaded, SDL Start (`button 6`) advanced
  `[PRESS START]` to the game's main menu; left-stick Y (`axis 1`) visibly
  displaced the live Aurora controller overlay; SDL A (`button 0`) selected
  the menu item and began the game intro. The normal window-close path returned
  without the FIFO-reader teardown hanging.
- Result: the software SDL event → Aurora controller assignment → `PADRead` →
  Metroid frontend path is proven. Physical Apple GCController discovery,
  hot-plug/reconnect, rumble, and touch/controller handoff are still untested.
  Evidence: `/tmp/virtual-gamepad-a-select-2026-08-12.png`; mirrored patch:
  `patches/2026-08-12-aurora-virtual-gamepad-test-hook.patch`.
- Rebuilt both final-source gates: `macos-default-relwithdebinfo` completed,
  and the recovered `ios-sim` Ninja journal rebuilt and linked all 2,527
  targets successfully.

### 2026-08-12 — SunPad display menu engine wiring

- Kept all audited SunPad UI/settings/mixer/input files unchanged and added the
  Metaforce-specific behavior only in `OverlayBridge.mm` and generic Aurora
  framebuffer APIs.
- Applied persisted SunPad settings before launch to test the exact menu action
  path without relying on the currently broken Simulator host-touch delivery.
  At 2× + Original 4:3 the app logged `renderScale=2 aspectMode=0` and Aurora
  created a `1280x960` framebuffer. At 2× + experimental 16:9 it logged
  `renderScale=2 aspectMode=1` and created a `1707x960` framebuffer. Both runs
  reached Dawn/WebGPU → Metal and live Metroid rendering with the unchanged
  SunPad touch overlay.
- Enabled SunPad's FPS setting and verified its label is driven by successful
  surface presents. Evidence:
  `/tmp/sunpad-menu-wiring-2x-fps-2026-08-12.png` and
  `/tmp/sunpad-menu-wiring-16x9-2026-08-12.png`.
- The visible 20.4 FPS sample was captured while another heavy app was running;
  the later image showed 59.9 after that app closed. These samples prove the
  counter changes with presented-frame rate, but neither is accepted as a
  benchmark or physical-device result. Future performance runs must follow
  [PERFORMANCE.md](PERFORMANCE.md).
- Final Aurora sources compiled and linked for iOS Simulator; the corresponding
  macOS preset also rebuilt. The full seven-patch Aurora sequence applies from
  clean pin `5143394`, passes `git diff --check`, and is byte-identical to the
  live nested source. Patch:
  `patches/2026-08-12-aurora-wire-sunpad-display-settings.patch`.

### 2026-08-11 — macOS rendering and keyboard fixes

- In-game: `Metaforce -l --warp 2 2 +debugOverlay.* <iso>` — full-screen scene +
  HUD, 1760 draw calls, 60 FPS; then ImGui segfault (KI-002).
- Artifacts: logs `/tmp/metaforce-run.log`, `/tmp/metaforce-fe.log`,
  `/tmp/metaforce-warp.log`; crash `~/Library/Logs/DiagnosticReports/
  Metaforce-2026-08-11-152126.ips`; screenshots `/tmp/metaforce-*.png`.
- Frontend investigation: frontend state machine verified (OpenCredits 10 s →
  Title 30 s → Attract cycle; per-second state logs), THP YUV decode verified
  correct by dumping decoded frames; movie rendering defect characterized as
  KI-001 (small garbled corner strip). See [KNOWN_ISSUES.md](KNOWN_ISSUES.md).
- ImGui shutdown crash (KI-002/005) root-caused from the user's crash report
  (`Metaforce-2026-08-11-165907.ips`): shutdown order bug in `aurora::shutdown`.
  Fix applied (gfx drain before imgui teardown + render guard); two consecutive
  quits verified exit 0 with no crash report.
- **KI-001 fixed:** frontend movies were clipped to a 640×480 region because the
  per-frame `BeginScene` never set the scissor (aurora refresh used the stale
  logical 640×480 scissor). Fix: full-frame scissor per frame + logical viewport
  sync + `GX_CULL_NONE` for the movie quad. Verified: title screen renders
  full-frame (gold "METROID PRIME" logo + emblem, 60 FPS), attract footage
  full-frame, in-game warp renders correctly (1,752 draw calls, HUD).
- **Keyboard input enabled (local patch):** implemented the kbm path upstream
  disabled. Verified end-to-end: holding Enter at the title screen advances the
  frontend to the save/memory-card dialog ("No Memory Card found in Slot A");
  Down arrow and S move the selection between "Continue Without Saving" and
  "Retry"; input overlay reflects key state; `config.yaml` written to the store
  root (configuration persistence). Patch:
  `patches/2026-08-11-metaforce-enable-keyboard-mouse-input.patch`.
- **iOS Simulator (iPad Pro 13-inch M5, iOS 26.5):** built the app for
  `SIMULATORARM64` after fixing Dawn cross-compile obstacles (built a host
  protoc from the vendored protobuf 33 for the codegen tool; disabled GLFW which
  is macOS-only). Installed via `simctl`, copied the user's ISO to the app
  container as `game.iso`, launched, and the app rendered the **Metroid Prime
  title screen** (gold "METROID PRIME" logo + emblem + [PRESS START], 4:3
  letterboxed). Live cycle confirmed; one simulator used, then shut down.

To be appended with dated entries for later phases.
