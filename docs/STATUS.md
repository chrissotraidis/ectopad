# Status

Last updated: 2026-08-13

For the concise authoritative handoff, artifact identity, and exact remaining
checklist, start with [CURRENT_STATE.md](CURRENT_STATE.md). Dated observations
below are retained as evidence; newer entries supersede older limitations.

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
| macOS ARM64 EctoPad build | **Proven** | `scripts/build-macos.sh` produces a native arm64 `EctoPad.app`, generates its bundle and runtime icon from the tracked EctoPad master, and passes five controller slot/reconnect checks; rebuilt 2026-08-17. |
| EctoPad launch on macOS (Metal, frame, title flow) | **Proven and user-accepted** | Dawn initialized Metal on an Apple M3 Max; the branded app reached the complete title/file-select flow, loaded the preserved 59% Chozo Ruins save, rendered gameplay with audio, and exited cleanly with status 0 on 2026-08-17. |
| Input on macOS (keyboard/mouse/controller) | **Keyboard/mouse accepted; controller software path proven** | The accepted desktop layout uses WASD movement, relative mouse look, left-click fire, right-click free-look, Shift lock-on, Space jump, F missiles, C Morph Ball, 1-4 beams, Q/E/Z/X visors, Tab map, arrow-key menu navigation, and Enter/Escape frontend actions. Scan and Combat visor switching were live-verified in Chozo Ruins. SDL controller presence takes player one and disconnect restores keyboard/mouse; five slot/reconnect tests pass. Physical Mac controller models and rumble remain untested. |
| Audio on macOS | **Proven (frontend + in-game, local patches)** | SDL3 device (44100 Hz stereo) + amuse engine with a software mixer backend + **soxr voice resampler** + **streamed DSP audio & MIDI sequencer restored** (`CStreamAudioManager` + `CMidiManager`): all 28 Prime audio groups load into amuse; in-game warp plays area music + SFX continuously (6–7 voices, 3 submixes) at 60 FPS; non-32 kHz voices (24/16/12/4 kHz) resample to correct pitch; frontend RSF music plays; stable pump, clean exit; see KNOWN_ISSUES KI-003 |
| HECL/game-data extraction from supplied ISO | **Proven** | Disc identified and all assets loaded from ISO at runtime ("Metroid Prime USA (Build v1.111 3/10/2003 17:56:21)"); raw ISO, no conversion required |
| iOS/iPadOS ARM64 device build | **Proven** | Final-source ARM64 iOS build succeeded at `build/ios-default/Binaries/Metaforce.app` (platform 2/iPhoneOS, min iOS 14.0) after fixing host-zstd leakage; unsigned packaging SHA-256 `308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d` |
| iOS Simulator (iPad) execution | **Proven** | iOS Simulator build succeeded (after fixing Dawn cross-compile host-tool issues: host protoc + GLFW disabled); **rebuilt 2026-08-11 23:20 with the vendored audio stack + CInputStream byte-order fix + kabufuda card-persistence fix**; installed and launched on iPad Pro 13-inch (M5) Simulator; loaded the user's ISO; Dawn/WebGPU reached Metal ("Apple iOS simulator GPU"); amuse audio initialized (32 kHz SDL backend); rendered the **Metroid Prime title screen**, and — via the `--autostart` test hook — ran the **full New Game flow: intro cinematic → first-person Frigate Orpheon gameplay** (visor HUD, ENERGY 99, arm cannon, radar) at 60 FPS, and **persisted a `GM8E01`/`MetroidPrime B` save to the sim's card** |
| Audio on Apple targets | **macOS proven; physical iPad transition defect open** | The iPad producer has restored DSP stream mixing, AVAudioSession, a direct Amuse pull path, final limiting, and bounded diagnostics. The attempted SoXR path and 10 ms handoff ramp were physically rejected and removed. Current logs show Speaker at 48 kHz with zero queue underruns, but a frontend-to-cutscene aberration remains audible. |
| Dawn/WebGPU reaching Metal on iOS device | **Launch proven; visual acceptance open** | The final development-signed build launched on the attached iPad. Native menu and core movement are accepted, but black/distant geometry, first-door pose presentation, and later-game stability remain open; see [TECH-DEBT.md](TECH-DEBT.md). |
| Touch controls (iPhone/iPad layouts) | **Partially proven (Simulator)** | Direct SunPad UIKit port renders on iPad/iPhone Simulator and its source parity is audited. The final current-build harness found all 14 controls, exercised settings/editor/A mixer behavior, restored preferences, and passed `result=0`. Physical finger/multitouch ergonomics remain unverified. See [SUNPAD_PARITY.md](SUNPAD_PARITY.md). |
| GameController support | **Partially proven (software path)** | `AURORA_VIRTUAL_GAMEPAD=1` attaches an SDL3 standard gamepad and accepts FIFO commands only for test. After correcting the hook's SDL3 joystick-ID and descriptor initialization, Metaforce logged `Aurora Virtual Gamepad`, Start advanced the title to the main menu, analog Y displaced the controller overlay, and A began the game. This proves SDL events → Aurora controller assignment → `PADRead`; physical Apple GCController hardware, hot-plug/reconnect, rumble, and touch/controller handoff remain untested. Evidence: `/tmp/virtual-gamepad-a-select-2026-08-12.png`. |
| Touch controls / menu (iPad) | **Core menu and movement physically accepted** | SunPad's native `UIMenu`, render/aspect/FPS actions, readable single-pass Prime file menu, and left-stick movement are accepted. Original Prime mapping is restored and 0.5 s L/R latch is deployed; full control/lifecycle regression acceptance remains. See KI-017–021 and [TECH-DEBT.md](TECH-DEBT.md). |
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
- **Gamepad input (2026-08-12):** an opt-in SDL3 virtual-gamepad test hook
  exercised the normal SDL gamepad-added/event → Aurora player assignment →
  `PADRead` path without keyboard, mouse, touch, or frontend automation. Start
  advanced the title to the main menu, left-stick Y moved the live controller
  overlay, and A began the game. The hook initially could not attach because
  SDL3 uses joystick ID `0` as invalid and requires `SDL_INIT_INTERFACE` for
  `SDL_VirtualJoystickDesc`; both errors are fixed and mirrored in
  `patches/2026-08-12-aurora-virtual-gamepad-test-hook.patch`. This is software
  path evidence, not a physical Apple GameController claim. Keyboard/mouse is
  also wired locally (see above). Evidence:
  `/tmp/virtual-gamepad-a-select-2026-08-12.png`.

### Blocked

- **Physical-device Gate 3:** the attached iPad launched. Full A/B card images
    are proven. Remaining acceptance in [TECH-DEBT.md](TECH-DEBT.md) is audible
    audio, corrected native-menu interaction, original Prime controls,
    save/reload, and named textures.

### Fixed this session

- **Frontend audio output enabled (KI-003):** implemented a minimal SDL3 audio
    output module in aurora and restored the `CStaticAudioPlayer` +
    `CFrontEndUI` frontend-music plumbing that upstream had commented out.
    Device opens, RSF files decode via G721, and the original per-frame
    fractional pump worked at 60 FPS. This historical implementation was later
    replaced by the frame-rate-independent 120 ms output-ready reserve. Patch:
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
    `CKeyboardMouseControllerData` (in CMain) → kbm `CFinalInput` pushed only
    when no gamepad is connected. The 2026-08-17 desktop pass replaced the
    inherited GameCube-keyboard layout with WASD plus relative mouse look,
    separated Shift lock-on from right-click free-look, added direct beam and
    visor keys, and raised mouse response 20%. The special-key encoding range
    now covers all declared keys, fixing Shift being silently discarded. The
    resulting controls and preserved-save gameplay were accepted on macOS.
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
- **SunPad UIKit touch overlay (2026-08-12, iPad Pro 13-inch M5, iOS 26.5):**
    the earlier independent ImGui overlay was superseded and is not the product
    UI. The current implementation directly ports SunPad's UIKit overlay,
    settings, mixer, input state, and diagnostics into Aurora. KI-015 was a
    static-archive collision: the non-iOS stub was compiled for iOS and linked
    before `OverlayBridge.mm`. CMake now excludes the stub on iOS. The corrected
    build logged `[SunPad] session start`, rendered the controls over the live
    intro/game, reached Dawn→Metal, and exited cleanly. Host Simulator clicks
    did not reliably deliver clicks. A later launch-only current-build UIKit
    harness supersedes that limitation: every interaction phase returned 0,
    preferences were restored, and the live bridge reported the `•••` visible.
    Evidence: `/tmp/ki015-overlay-fixed-2026-08-12.png`,
    `/tmp/ki015-overlay-fixed-2026-08-12.log`,
    `/tmp/ios_menu_open2.png`; see [SUNPAD_PARITY.md](SUNPAD_PARITY.md).
    The engine side of SunPad's display controls is additionally proven by
    persisted-setting launches: 2× Original logged a 1280×960 framebuffer,
    2× experimental 16:9 logged 1707×960, and the FPS switch displayed actual
    successful-present rate. Evidence:
    `/tmp/sunpad-menu-wiring-2x-fps-2026-08-12.png` and
    `/tmp/sunpad-menu-wiring-16x9-2026-08-12.png`. Their visible FPS samples
    are not performance baselines because a competing heavy app confounded the
    first run; see [PERFORMANCE.md](PERFORMANCE.md).
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

## Automation check 2026-08-12 08:10 (macOS)

- Build: OK (incremental, current — binary from 07:46, no rebuild needed).
- macOS render: OK — `--warp 2 2` full-frame Tallon Overworld gameplay
  (HUD, ENERGY 99, radar, minimap, arm cannon); FPS 55.1, draw calls 1782,
  merged 14768, done pipelines 250; no FATAL/error lines. Evidence:
  `/tmp/mf_auto_check.png`, log `/tmp/mf_auto_check4.log`.
- Audio: OK — 6 voices (3 submixes) steady audio pump.
- iPad Simulator (opportunistic): sim booted + app installed + process alive,
  but the running instance is the agent's `--metaforce-import-test` harness and
  captured a **black frame** (no gameplay render at capture time) — not treated
  as a regression; evidence `/tmp/ios_auto_check.png`.
- Note: exec-session teardown kills backgrounded Metaforce processes; keep
  launch + capture inside one command.

## Automation check 2026-08-12 12:05 (macOS)

- Build: OK — incremental (75/75, current tree at `c7bd09c`).
- macOS render: OK — `--warp 2 2` full-frame Tallon Overworld gameplay
  (HUD, ENERGY 99, radar, minimap, arm cannon); **FPS 60.0, draw calls 1807,
  merged 14814**, done pipelines 250; no FATAL/error lines. Evidence:
  `/tmp/mf_auto_check.png`, log `/tmp/mf_auto_check.log`.
- Audio: OK — 6 voices (3 submixes) steady pump.
- iPad Simulator (opportunistic): **skipped (sim not booted)**.

## Physical Gate 2/3 attempt 2026-08-12 (prerequisites later satisfied)

- Earlier the same day, a scoped pass found zero identities and no device and
  correctly stopped. That snapshot is historical only.
- A later session development-signed
  `ref/metaforce/build/ios-default/Binaries/Metaforce.app`, installed it in
  place, and launched `com.axiodl.Metaforce` on the attached iPad.
- Gate 2 launch happened. Gate 3 failed on the user's hands-on: no default
  cards, messed-up audio, unfinished `•••` without 1×–4×, inert C-stick, and
  texture issues. See [TECH-DEBT.md](TECH-DEBT.md).

## Final state reconciliation 2026-08-12

- Current menu/touch harness: `phase=settings/editor/folder/files-picker`
  all returned 0; recursive menu inventory 19, gameplay controls 14,
  `interaction test result=0 restored=verified`, and live
  `menu visible=1 attached=1`. Evidence:
  `/tmp/ipad-menu-audio-focused-2026-08-12.{png,log}`.
- Current audio producer: converted output-ready depth drives a bounded 120 ms
  reserve; loaded native evidence held 5,292–5,294 frames against a 5,292 target
  with 6–7 voices and no underrun log. This is mechanical, not audible-device
  proof. Evidence: `/tmp/metaforce-audio-reserve-native-2026-08-12.log`.
- Final unsigned IPA:
  `/tmp/Metaforce-unsigned-menu-audio-2026-08-12.ipa`, SHA-256
  `308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`.
  Two packages were identical and the package audit passed. It is not signed,
  installable as-is, or approved for public redistribution.
- The current 0.1.3 packaging pass supersedes that validation artifact. The
  ignored local file is `artifacts/EctoPad-0.1.3-unsigned.ipa`, 12,427,372
  bytes, SHA-256
  `f90b353617d81ce3e4f6a0ebedf52f7c10a39d4b969d48a78335b35263010086`.
  Two 2026-08-17 packages were byte-identical and passed the same game-data,
  save, log, signing-material, architecture, platform, linkage, path, and
  credential audit. It remains unsigned/local-only under the current release
  boundary.
- Next work is physical acceptance of the current build: audible audio,
  `•••` submenu/dismissal and 1×/2× changes, original Prime controls, and a
  save/relaunch/load cycle; then named textures and deploy automation.
