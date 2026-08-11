# Status

Last updated: 2026-08-11

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
| Audio on macOS | **Blocked (upstream)** | No audio output device code in the current upstream tree; playback plumbing commented out mid-refactor; see KNOWN_ISSUES |
| HECL/game-data extraction from supplied ISO | **Proven** | Disc identified and all assets loaded from ISO at runtime ("Metroid Prime USA (Build v1.111 3/10/2003 17:56:21)"); raw ISO, no conversion required |
| iOS/iPadOS ARM64 device build | **Not yet tested** | Upstream CI builds iOS `.app`; not yet reproduced locally |
| Dawn/WebGPU reaching Metal on iOS device | **Not yet tested** | Principal technical unknown (Gate 2) |
| Touch controls (iPhone/iPad layouts) | **Not yet tested** | |
| GameController support | **Not yet tested** | |
| Save/reload behavior | **Not yet tested** | |
| Frigate Orpheon / later-area gameplay | **Not yet tested** | |

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

### Partially proven
- **Gamepad input:** wired via aurora PAD → SDL_Gamepad (untested — no controller
    connected). Keyboard/mouse is now wired locally (see above).

### Blocked

- **Audio output (macOS):** the current upstream tree has no audio output device
    code (no CoreAudio/SDL-audio/AudioQueue anywhere; musyx playback calls in
    CSfxManager are commented out). This is an upstream mid-refactor gap, not an
    Apple-port problem. Audio assets load without error.

### Fixed this session

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

### Simulator-only

- Nothing yet.

### Physical-device verified

- Nothing yet.

### Not yet tested

- Everything in the "Current state" table marked **Not yet tested**.
