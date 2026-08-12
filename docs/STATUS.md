# Status

Last updated: 2026-08-12

## Status legend

- **Proven** — verified with dated evidence on the named hardware.
- **Partially proven** — works but with caveats or incomplete verification.
- **Simulator-only** — verified in an iOS Simulator, not on physical hardware.
- **Physical-device verified** — verified on a named physical Apple device.
- **Blocked** — cannot proceed for a documented reason.
- **Not yet tested** — not attempted or not yet verified.

## Current state

| Item | Status | Evidence |
| --- | --- | --- |
| ImGui shutdown crash (KI-002/005: every quit segfaulted) | **Proven (fixed)** | Reordered aurora shutdown (gfx drain before imgui teardown) + guard; app now exits code 0, no crash report; patch saved |
| Game data validated (GM8E01, Rev 2, CRC 61592372) | **Proven** | Hashes match Redump "(USA) (Rev 2)" entry exactly; see [GAME_DATA.md](GAME_DATA.md) |
| Reference repos cloned/pinned | **Proven** | metaforce @ `621ee0f`, aurora @ `5143394` (pin used by metaforce), prime @ `72e31c7`, sunpad @ `7d84cec`; see [DEPENDENCIES.md](DEPENDENCIES.md) |
| Architecture investigation | **Proven** | Current tree renders via Aurora GX-on-WebGPU (Dawn) → Metal on Apple; see [ARCHITECTURE.md](ARCHITECTURE.md) |
| macOS ARM64 build of current Metaforce | **Proven** | Clean RelWithDebInfo build, 862/862 steps, arm64 Mach-O, 2026-08-11 |
| Metaforce launch on macOS (Metal, frame, title flow) | **Proven** | Launches; Dawn initializes Metal (Apple M2 IntegratedGPU); 60 FPS; **frontend renders full-frame (title screen "METROID PRIME" logo + emblem verified)**; in-game warp renders fully (1752 draw calls, HUD) — after the KI-001 scissor/viewport fix |
| Input on macOS (keyboard/mouse/controller) | **Proven (keyboard, local patch)** | Keyboard/mouse wired locally (was disabled upstream via `#if 0`); verified: Enter=Start advances title → save dialog, arrows/S navigate menus, D-pad/stick state reflected in the input overlay; gamepad path untested (no controller connected) |
| Audio on macOS | **Proven (frontend + in-game, local patches)** | SDL3 device (44100 Hz stereo) + amuse engine with a software mixer backend + **soxr voice resampler** + **streamed DSP audio & MIDI sequencer restored** (`CStreamAudioManager` + `CMidiManager`): all 28 Prime audio groups load into amuse; in-game warp plays area music + SFX continuously (6–7 voices, 3 submixes) at 60 FPS; non-32 kHz voices (24/16/12/4 kHz) resample to correct pitch; frontend RSF music plays; stable pump, clean exit; see KNOWN_ISSUES KI-003 |
| HECL/game-data extraction from supplied ISO | **Proven** | Disc identified and all assets loaded from ISO at runtime ("Metroid Prime USA (Build v1.111 3/10/2003 17:56:21)"); raw ISO, no conversion required |
| iOS/iPadOS ARM64 device build | **Proven** | Local ARM64 iOS build succeeded (`build/install/Metaforce.app`, platform 2/iOS, minos 14.0) after fixing an upstream zstd link issue (Homebrew macOS dylib leaking into the iOS link) |
| iOS Simulator (iPad) execution | **Proven** | iOS Simulator build succeeded (after fixing Dawn cross-compile host-tool issues: host protoc + GLFW disabled); **rebuilt 2026-08-11 23:20 with the vendored audio stack + CInputStream byte-order fix + kabufuda card-persistence fix**; installed and launched on iPad Pro 13-inch (M5) Simulator; loaded the user's ISO; Dawn/WebGPU reached Metal ("Apple iOS simulator GPU"); amuse audio initialized (32 kHz SDL backend); rendered the **Metroid Prime title screen**, and — via the `--autostart` test hook — ran the **full New Game flow: intro cinematic → first-person Frigate Orpheon gameplay** (visor HUD, ENERGY 99, arm cannon, radar) at 60 FPS, and **persisted a `GM8E01`/`MetroidPrime B` save to the sim's card** |
| Audio on iPad Simulator | **Proven (Simulator-only)** | amuse engine + SDL backend initialize on the iPad Pro 13-inch (M5) Simulator (device 44100 Hz, engine 32 kHz); audio groups load into amuse (Misc/MiscSamus/UI/Weapons/ZZZ at frontend); `CStaticAudioPlayer` streams `frontend_1.rsf`/`frontend_2.rsf`; `SDLBackend: 2 voices (2 running), 3 submixes`, stable 534-frame audio pump, no underruns; see KNOWN_ISSUES KI-003 |
| Dawn/WebGPU reaching Metal on iOS device | **Not yet tested** | Principal technical unknown (Gate 2); simulator rendering (host GPU/Metal path) works — physical-device verification pending signing identity + hardware |
| Touch controls (iPhone/iPad layouts) | **Not yet tested** | |
| GameController support | **Not yet tested** | |
| Touch controls / menu (iPad) | **Port in place, attach regression (Simulator)** | SunPad's UIKit overlay is ported nearly verbatim into `ref/metaforce/extern/aurora/lib/ios/` (SunPadGameOverlay, Settings, InputMixer, InputState, Diagnostics) + `OverlayBridge.mm`, rendering identically to SunPad (verified 2026-08-12: green A, red B, gray X/Y, purple Z, gold C-stick, D-pad, L/R shoulders, START, ••• menu). **Open regression (KI-015):** attach currently fails after a cleanup pass — see KNOWN_ISSUES and `docs/AGENT_GOAL_LOOP.md` §3. |
| Save/reload behavior | **Proven (macOS)** | Full cycle verified 2026-08-11 after the kabufuda queue/commit fix (KI-011): new game → save slot `GM8E01`/`MetroidPrime B` persisted to the raw card on disk → clean quit → relaunch → file select shows the save → A loads the saved game (intro narration + gameplay at 60 FPS). In-game save-station save/reload not yet exercised (requires navigating to a save station) |
| Frigate Orpheon / later-area gameplay | **Partially proven** | New Game now reaches **actual first-person gameplay** in Frigate Orpheon after the KI-009 fix: intro cinematic (space, gunship, Samus model) renders at 60 FPS; gameplay view renders with visor HUD (energy 99), arm cannon, minimap at 60 FPS, 1000+ draw calls; keyboard movement works. Full playthrough + save station + later areas not yet verified |
| App lifecycle (background/foreground) on iPad Simulator | **Proven (Simulator-only)** | Full background → foreground cycles verified 2026-08-12 on iPad Pro 13-inch (M5) Simulator with the aurora iOS lifecycle fix: Home → app backgrounds (home screen, process alive) → relaunch foregrounds → Frigate Orpheon gameplay resumes cleanly with HUD/state intact (ENERGY 99), CPU returns to gameplay levels, RSS stable (~581 MB, no leak across 2 cycles). See detail below |

## Upstream context

- Metaforce README marks the project **alpha** and says builds are "currently
  unavailable while the project undergoes large changes," but upstream CI
  ([`.github/workflows/build.yml`](../ref/metaforce/.github/workflows/build.yml))
  actively builds macOS, iOS, tvOS, Windows, and Linux.
- The current tree moved graphics into the **Aurora** compatibility layer
  (GX on WebGPU/Dawn). Older documentation referencing a direct Dawn integration
  is stale; see [ARCHITECTURE.md](ARCHITECTURE.md).

## Proven / partially proven detail

### Proven

- **Dump integrity (2026-08-11):** `ref/Metroid Prime [GM8E01] [61592372][b].iso`
  (1,459,978,240 bytes) has CRC32 `61592372`, MD5 `fdfc41b8414dd7d24834c800f567c0f8`,
  SHA-1 `1a737910b55b59c6ad91be9e3e3c43517fd52efb`. Game ID `GM8E01`, revision byte
  `0x02` at header offset `0x07` (Rev 2), magic `0xC2339F3D`, title "Metroid Prime".
- **Repository pins (2026-08-11):** see [DEPENDENCIES.md](DEPENDENCIES.md).
- **Gate 1 — macOS ARM64 build and launch (2026-08-11):**
  - Clean RelWithDebInfo build (862/862 steps) → `Binaries/Metaforce.app` (arm64,
    25 MB), linked with Metal.framework and Homebrew SDL3/libpng/freetype/abseil.
  - Launch: `Metaforce -l <iso>` — Aurora init, WebGPU instance, "Attempting to
    initialize Metal", adapter **API: Metal / Device: Apple M2 (IntegratedGPU)**,
    surface BGRA8Unorm, framebuffer 2560x1920 scale 2, "Loading data from Metroid
    Prime USA (Build v1.111 3/10/2003 17:56:21)", thousands of assets async-built
    from the ISO. No errors in logs.
  - Frame rendering: FPS 60.1 sustained; Dawn pipeline cache + Metal shader cache
    written to the store root.
  - In-game rendering via `--warp 2 2`: full-screen first-person scene with HUD at
    60 FPS (Frame 3291, draw calls 1760, merged 14448) before an ImGui-overlay
    segfault (see KNOWN_ISSUES).
- **New Game path (2026-08-11, after KI-009 fix):** title → file select →
  new game → intro cinematic (space starfield, planet, gunship, Samus model,
  frame ~5772, 60 FPS) → **first-person gameplay inside Frigate Orpheon**
  (viso HUD, energy 99, arm cannon, minimap; frame ~10635, 60 FPS, ~1000 draw
  calls; W moves the player). Audio: 10–15 amuse voices during the intro.
  Evidence: `/tmp/mf_f2.png`, `/tmp/mf_gx4.png`, `/tmp/mf_gx5.png`,
  `/tmp/mf_gx6.png`, `/tmp/mf_play1.png`, logs `/tmp/mf_fixed2.log`,
  `/tmp/mf_savetest.log`.
- **Save/reload round trip (2026-08-11, after KI-011 fix):** new game wrote
  `GM8E01`/`MetroidPrime B` into the card directory block on disk (previously
  never persisted); after a clean SIGINT exit and relaunch, the file select
  loaded the save and pressing A started the saved game (intro narration text
  card → cinematic at 60 FPS, 10+ audio voices). Evidence:
  `/tmp/mf_reload_a2.png`, `/tmp/mf_reload_game.png`, card hexdump at
  `~/Library/Application Support/dolphin-emu/GC/MemoryCardA.USA.raw` (entry at
  offset 0x2040), logs `/tmp/mf_savefix.log`, `/tmp/mf_reload.log`.
- **iOS Simulator in-game (2026-08-11):** rebuilt the sim app with all recent
  fixes and verified in-game rendering through Dawn→Metal: `--warp 2 2`
  rendered Tallon Overworld (ruins, full HUD); `--autostart` drove the full
  frontend flow (title → file select → new game → intro cinematic → first-person
  Frigate Orpheon gameplay with ENERGY 99 / missiles 15 / arm cannon / radar at
  60 FPS) with no host input; the new-game save
  (`GM8E01`/`MetroidPrime B`) was persisted to the sim container's card. The
  `--autostart` hook (patch:
  `patches/2026-08-11-metaforce-autostart-test-hook.patch`) exists because
  host keyboard forwarding to the Simulator was unavailable
  (`ConnectHardwareKeyboard` ignored) — it is also the foundation for future
  device automation. Evidence: `/tmp/ios_warp1.png`, `/tmp/ios_auto1.png`,
  `/tmp/ios_auto2.png`, `/tmp/ios_auto3.png`.

### Partially proven
- **Gamepad input:** wired via aurora PAD → SDL_Gamepad (untested — no controller
    connected). Keyboard/mouse is now wired locally (see above).

### Blocked

- **Physical-device Gate 2/3:** requires a code-signing identity and physical
    iPhone/iPad (user-side prerequisite; no signing identity installed, no
    device connected as of 2026-08-11). Not an engine blocker.

### Fixed this session

- **Frontend audio output enabled (KI-003):** implemented a minimal SDL3 audio
    output module in aurora and restored the `CStaticAudioPlayer` +
    `CFrontEndUI` frontend-music plumbing that upstream had commented out.
    Device opens, RSF files decode via G721, per-frame pump keeps the stream
    steady (fractional 533/534-frame accumulator), clean shutdown. Patch:
    `patches/2026-08-11-metaforce-enable-audio-output.patch`.
- **ImGui shutdown crash (KI-002/005):** every app exit segfaulted in
    `ImGui_ImplWGPU_RenderDrawData` because `imgui::shutdown()` nulled the backend
    before `gfx::shutdown()` drained the render worker. Fixed locally by
    reordering shutdown and guarding `imgui::render`; verified clean exits.
- **Frontend movie rendering (KI-001):** root cause was the game never setting
    the per-frame scissor, so aurora's frame-start refresh used the stale 640×480
    logical scissor, clipping every frontend draw. Fixed by setting a full-frame
    scissor per frame (`CCubeRenderer::BeginScene`), keeping the logical viewport
    in sync (`CGraphics::SetViewport` also calls `GXSetViewport`), and disabling
    culling for the movie draw. Title screen and attract movies now render
    full-frame; in-game rendering verified unaffected.
- **Keyboard/mouse input enabled:** upstream disabled the kbm path
    (`#if 0` in `CInputGenerator::Update` and an unfinished kbm `CFinalInput`
    constructor). Implemented a working path: SDL key/mouse/text events →
    `CKeyboardMouseControllerData` (in CMain) → kbm `CFinalInput` (WASD left
    stick, IJKL right stick, J/K/I/U/H/Q/E = A/B/X/Y/Z/L/R, Enter = Start,
    arrows = D-pad) pushed when no gamepad is connected. Verified: Enter at the
    title opens the save/memory-card dialog; arrows/S navigate menus.
- **Streamed DSP audio + MIDI sequencer restored (KI-003):** restored
    `CStreamAudioManager` stream voice supply (DSPADPCM decode via amuse
    `DSPDecompressFrame`/`DSPDecompressFrameRanged`), stream voices through the
    `SDLBackendVoice` custom supply callback with per-stream-file
    `ResetSampleRate`, and `CMidiManager` `seqPlay`/`stopSong`/`setVolume` via
    the amuse sequencer. In-game voice count rose to 6–7 (area music + SFX) at
    60 FPS with no resampler warnings. Patch:
    `patches/2026-08-11-metaforce-amuse-in-game-audio.patch` +
    `patches/amuse-audio-vendor/soxr.patch`.

### Simulator-only

- **Audio on iPad Simulator (2026-08-11, iPad Pro 13-inch M5, iOS 26.5):** the
    full vendored audio stack builds for the sim SDK and runs — Metal adapter
    "Apple iOS simulator GPU", amuse engine initialized (SDL backend, 32 kHz),
    audio groups load into amuse, frontend RSF music streams, `SDLBackend: 2
    voices (2 running), 3 submixes` with a stable 534-frame audio pump. Evidence:
    log `/tmp/mf_ios_sim_audio.log`, screenshots `/tmp/ios_audio_check2.png`
    (title screen) and `/tmp/ios_audio_final.png`.
- **Prime-native touch overlay (2026-08-12, iPad Pro 13-inch M5, iOS 26.5):**
    replaced the invisible zone layout with a rendered, customizable overlay in
    aurora (`lib/touch.cpp`, drawn in the ImGui pass above the game): left stick
    (move), right stick (look), D-pad (beam left/right, visor up/down — the
    original Prime mapping), A/B/X/Y face buttons, Z (jump), L (fire/charge),
    R (lock-on), START (pause), a gear (edit mode) and RESET. Touch input
    merges with physical controllers via the existing virtual-status path (no
    mode switch = seamless handoff). Edit mode (gear) enables drag-repositioning
    of any control; positions persist normalized per orientation in
    `<userPath>/touch_overlay.ini` (`[landscape]`/`[portrait]`) and reload on
    launch; RESET restores defaults. Verified in Frigate Orpheon gameplay via
    Simulator GUI touches: stick drags moved/turned Samus; START opened the
    pause/inventory screen; B closed it; dragging the A button moved it to
    (0.964, 0.842), the layout saved and rendered at the new position after a
    relaunch, then was reset to defaults. The iPad Simulator always reports a
    virtual Apple gamepad (vid 05ac, pid 0004), so controller presence cannot
    auto-hide the overlay on the sim — input simply merges. Patch:
    `patches/2026-08-12-aurora-touch-overlay.patch` (replaces the earlier
    `2026-08-12-aurora-touch-input-virtual-pad.patch`). Evidence:
    `/tmp/ios_overlay_play1.png`, `/tmp/ios_start3_after.png`,
    `/tmp/ios_editmode_confirm.png`, `/tmp/ios_persist_check.png`,
    `/tmp/touch_overlay_test_layout.ini`.
- **Lifecycle on iPad Simulator (2026-08-12, iPad Pro 13-inch M5, iOS 26.5):**
    verified background → foreground behavior twice from inside Frigate Orpheon
    first-person gameplay. **Root-cause finding:** SDL3's iOS backend reports
    backgrounding via `SDL_EVENT_DID_ENTER_BACKGROUND`/`WILL_ENTER_FOREGROUND`,
    not `WINDOW_MINIMIZED`/`RESTORED`, so aurora's `g_backgrounded` flag (and
    therefore `window::is_presentable()`) never fired on iOS — the engine had no
    explicit pause path on Apple mobile. Fixed in aurora's
    `lifecycle_event_watch` by mapping those events to `g_backgrounded`
    (`patches/2026-08-12-aurora-fix-ios-background-events.patch`). With the fix:
    Home → app backgrounds (home screen shown, process alive) and the frame loop
    halts simulation + rendering (`begin_frame()` returns false, no present
    attempts); foreground via `simctl launch` → Frigate Orpheon resumes cleanly
    with HUD/state intact (ENERGY 99, missiles 15), CPU 33→38% (gameplay), RSS
    stable 594.6 → 595.2 MB across the two cycles (no leak). Backgrounded CPU
    ~16% is the event-poll spin of the main loop (no sleep); on physical iOS the
    OS suspends the process regardless. Evidence: `/tmp/ios_lc_re1.png`
    (intro text), `/tmp/ios_lc_re3.png` (gameplay pre-background),
    `/tmp/ios_lc_re_bg.png` (home screen), `/tmp/ios_lc_re_fg.png` and
    `/tmp/ios_lc_re_fg2.png` (clean resumes).

### Physical-device verified

- Nothing yet.

### Not yet tested

- Everything in the "Current state" table marked **Not yet tested**.
