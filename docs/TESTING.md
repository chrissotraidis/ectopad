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
| iPhone Simulator | ☑ | ☐ | ☑ shell/UI | ☐ | — | ☐ | ☐ | ☐ | ☐ |
| iPad Simulator | ☑ | ◐ service | ☑ | ☐ | — | ☐ | ☑ preserved | ☑ | ☐ |
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

### 2026-08-12 — private atomic iOS game-data import

- Wired the unchanged SunPad Game Data & Saves actions to a Files picker,
  Files-visible Documents-folder import, and save-preserving removal. The
  importer validates exact raw-disc size, `GM8E01`, disc 0, revision 2, and
  GameCube magic before copying; it then validates canonical SHA-1 on a
  mode-0600 same-directory staging file before atomic `rename(2)` activation.
- Invalid 4 KiB input was rejected without changing the active image or save.
  A valid import changed the active inode and produced the canonical SHA-1; a
  normal `--autostart` restart loaded Metroid Prime USA Build v1.111 through
  Dawn/WebGPU → Metal. Removal deleted only `game.iso`, preserved the save, and
  valid reimport restored the image.
- A deliberately corrupted full-size clone passed header/size checks but was
  rejected by staged SHA-1. The prior active inode and canonical hash stayed
  intact, the save SHA-256 stayed
  `107e0d4a0494f6905d47c6bcf85b0d68ba7b2160d37ed4596564f2b10b322f5d`,
  and no staging file remained. Runtime log:
  `/tmp/metaforce-import-atomic-2026-08-12.log`.
- Final sources compile/link for iOS Simulator, macOS ARM64, and physical arm64
  iOS. The device artifact is a Mach-O arm64 `Metaforce.app`; configuration was
  hardened so cross-compiles vendor static zstd rather than link Homebrew's
  macOS dylib. Production Files-picker tapping is still unverified because host
  Simulator touch delivery is broken and no signed physical device is present.

### 2026-08-12 — holistic audit and SunPad controller mapping

- Added [COMPLETION_AUDIT.md](COMPLETION_AUDIT.md), checking every goal block
  against current evidence. The goal remains active: physical iPhone/iPad Gates
  2/3, current touch/UI interaction, diagnostics sharing, gameplay breadth,
  controlled performance/memory, and packaging remain incomplete.
- Ported `SunPadControllerMapping.{h,mm}` byte-identically. The unchanged
  SunPad menu now presents the reference five-button mapping UI; Aurora applies
  its persisted A/B/X/Y/right-shoulder permutation only to the iOS SDL gamepad
  path, leaving sticks, triggers, D-pad, Start, L, touch, and desktop unchanged.
- Simulator test returned `result=0` after verifying default mapping, persisted
  A/B swap in both directions, and Start/L passthrough, then restored the user's
  prior preference. A deterministic UI launch displayed all five default rows
  over the unchanged controls. Evidence:
  `/tmp/ios-controller-mapping-ui-2026-08-12.png` and
  `/tmp/ios-controller-mapping-2026-08-12.log`.
- The full SDL virtual-controller → Aurora assignment → `PADRead` mapping path
  was also exercised with an opt-in test-only A/B swap. A raw physical east/B
  bit (`physical=0x0200`) emerged as GameCube A (`buttons=0x0100`) and returned
  to zero on release. Evidence was captured from the live console; the guarded
  test assignment/logging is inert unless explicitly enabled.
- macOS ARM64, iOS Simulator, and physical arm64 iOS builds all completed. The
  Simulator was shut down afterward. Physical behavior is not claimed: this Mac
  currently has zero code-signing identities and `devicectl` sees no device.

### 2026-08-12 — iPhone diagnostics presentation and privacy

- Installed the current universal Simulator app on an iPhone 17 Pro Simulator
  and used test-only entry points into the unchanged SunPad overlay. Landscape
  controls, the Share Diagnostic Log confirmation, and the real UIKit activity
  sheet rendered without clipping. Evidence:
  `/tmp/iphone-diagnostics-confirmation-final-landscape-2026-08-12.png` and
  `/tmp/iphone-diagnostics-share-landscape-2026-08-12.png`.
- The shared item is a text document named `Metaforce-Diagnostic-<UTC>.log`.
  A service-level test wrote the current app home and temporary paths through
  `SunPadLog`, snapshotted the persistent log, and returned `result=0` only after
  confirming both raw paths were absent and the `<app-container>` and
  `<temporary>/` markers were present. Log:
  `/tmp/iphone-diagnostics-2026-08-12.log`.
- This is Simulator presentation/privacy evidence, not proof of tapping the menu
  or completing an export on physical hardware. The iPhone Simulator was shut
  down afterward; only one Simulator was active.

### 2026-08-12 — mobile desktop-chrome parity regression

- iPhone evidence exposed desktop ImGui chrome beneath SunPad's controls: main
  menu bar, pre-launch About UI, controller toast, and optional debug overlays.
  This violated the requirement that SunPad own everything overlaid on mobile.
- `ImGuiConsole` now retains listener/state maintenance on iOS but returns before
  all desktop presentation in `PreUpdate` and `PostDraw`. The iPhone 17 Pro and
  iPad Pro 13-inch Simulators then rendered only the unchanged SunPad controls
  over the engine/game surface. macOS still compiles the original ImGui path.
- Evidence: `/tmp/iphone-sunpad-clean-mobile-ui-landscape-2026-08-12.png` and
  `/tmp/ipad-sunpad-clean-mobile-ui-2026-08-12.png`. Both Simulators were shut
  down; the iPhone and iPad runs were sequential.

### 2026-08-12 — deterministic unsigned iOS packaging audit

- Fixed the no-tag checkout fallback so the Apple marketing/build versions are
  valid `0.0.1` and `1`, exposed validated cache overrides for real release
  versions, and rebuilt the full arm64 iPhoneOS app successfully.
- Remapped Metaforce C/C++ checkout/build roots and nod/Rust Cargo roots at
  compile time. The rebuilt executable contains no current-user checkout or
  Cargo-home paths. The prebuilt Dawn archive still contributes 241 unique
  upstream `/Users/runner/work/dawn-build/...` diagnostic paths; the audit
  reports these separately and rejects every other `/Users/` or `/Volumes/`
  path.
- Ported SunPad's deterministic `Payload/*.app` packaging/audit discipline.
  The staged copy is unsigned, stripped, xattr-free in ZIP metadata, timestamp
  normalized, license/notices complete, arm64-only, iPhoneOS-targeted, free of
  host dylinks, and scanned for disc headers, saves, logs, credentials, local
  paths, and signing material.
- Two independent packages were byte-for-byte identical at SHA-256
  `1348c52c9cbc8f3882f8f4a131fb6e0dc34c901d8ac91367c3c334d76c554835`.
  A negative archive seeded with `leaked-save.gci` was rejected. Evidence:
  `/tmp/Metaforce-unsigned-validation-final-2026-08-12.ipa` and
  `/tmp/metaforce-negative-audit.log`.
- This proves local unsigned validation packaging only. Public redistribution
  remains blocked on GPL/LGPL release materials and scrubbed Dawn provenance;
  physical install remains blocked on signing identity and hardware.

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
