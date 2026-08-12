# Known Issues

Last updated: 2026-08-11

| ID | Area | Issue | Status | Evidence |
| --- | --- | --- | --- | --- |
| KI-001 | Frontend presentation | Intro/title/attract THP movies rendered clipped/garbled (band/block in a corner) instead of filling the window. | **FIXED (2026-08-11, local patch)** — root cause: the game's per-frame `BeginScene` set the viewport but **never set the scissor**, and aurora's frame-start refresh derives the draw scissor from the GX logical state (stale 640×480) — so all frontend draws clipped to a 640×480 region. Fixes: (1) `CCubeRenderer::BeginScene` now sets a full-frame scissor every frame; (2) `CGraphics::SetViewport`/`ConfigureFrameBuffer`/`ClearBackAndDepthBuffers`/`SetDepthRange` also call the standard `GXSetViewport` so the logical viewport stays in sync; (3) movie draw sets `GX_CULL_NONE` (CW-wound triangles were culled by the per-frame `CullMode::Front`). **Verified: title screen renders full-frame ("METROID PRIME" gold logo + emblem at 60 FPS); attract footage renders full-frame; in-game warp renders correctly (1,752 draw calls).** Patch: `patches/2026-08-11-metaforce-fix-frontend-viewport-scissor.patch` | Screenshots `/tmp/mf-fixed-view.jpg`, `/tmp/mf-verify3-view.jpg` (title), `/tmp/mf-warp2-view.jpg` (in-game) |
| KI-002 | Diagnostics (ImGui) | `ImGui_ImplWGPU_RenderDrawData` segfaults (null backend data, EXC_BAD_ACCESS at 0xb0) on aurora's render worker thread. Root cause: shutdown order — `imgui::shutdown()` (nulls `BackendRendererUserData`) ran before `gfx::shutdown()` drained the render worker's pending end-frame callbacks, which still call `imgui::render`. This made **every app exit segfault** (exit 139) and generated a crash dialog each time. | **FIXED (2026-08-11, local patch)** — reorder `aurora::shutdown()` to `gfx::shutdown()` before `imgui::shutdown()`, plus a `g_initialized` guard in `aurora::imgui::render`. Verified: two consecutive app quits exit code 0, no crash report generated. Patch: `patches/2026-08-11-aurora-fix-imgui-shutdown-crash.patch` | User crash report `Metaforce-2026-08-11-165907.ips` (shutdown path: `aurora_shutdown → gfx::shutdown → render_worker::synchronize` vs worker in `ImGui_ImplWGPU_RenderDrawData`) |
| KI-003 | Audio | Upstream tree has no audio output device code and the amuse game-audio engine was removed entirely (musyx vendored but unused; `CSfxManager`/`CStreamAudioManager`/`CMidiManager` amuse calls commented out). | **FIXED (2026-08-11, local patches)** — (1) SDL3 output device in aurora + restored frontend RSF music (`patches/2026-08-11-metaforce-enable-audio-output.patch`); (2) vendored amuse + Hecl deps (athena, logvisor, lzokay), fixed atdna's DNA codegen for LLVM 22 (C++20, alias-template/dependent-specialization canonical-type handling, Integral Seek args), pre-generated the three DNA sources; (3) software mixer backend `Runtime/Audio/SDLBackend.{hpp,cpp}` (amuse `IBackendVoiceAllocator` → stereo float mix mirroring the boo mixer) with a **soxr voice resampler** (vendored `extern/soxr`, LGPL; input-rate → 32 kHz mix at SOXR_20_BITQ, VR for dynamic pitch); restored `CAudioSys` engine plumbing + `CSfxManager` play/emitter/listener/aux paths (`patches/2026-08-11-metaforce-amuse-in-game-audio.patch`; vendor script `patches/amuse-audio-vendor/vendor-amuse-audio.sh`); (4) **streamed DSP audio + MIDI sequencer restored** — `CStreamAudioManager` stream voice supply (DSPADPCM decode via amuse `DSPDecompressFrame`/`DSPDecompressFrameRanged`), stream voices through the `SDLBackendVoice` supply callback with per-stream-file `ResetSampleRate`, and `CMidiManager` `seqPlay`/`stopSong`/`setVolume` via the amuse sequencer; in-game voice count 6–7 (area music + SFX), 60 FPS, no resampler warnings. **Verified on macOS:** all 28 Prime audio groups parse and load into amuse; frontend music plays; stable pump; clean exit. **Verified on iPad Pro 13-inch (M5) Simulator (2026-08-11):** the full audio stack builds for the sim SDK; amuse initializes (SDL backend, 32 kHz), audio groups load, frontend RSF music plays (2 voices, 3 submixes, stable pump), title screen renders. | Logs `/tmp/mf_soxr.log`, `/tmp/mf_midi.log`, `/tmp/mf_ios_sim_audio.log`; screenshots `/tmp/mf_soxr_game.png` (Chozo-ruins area, 60 FPS), `/tmp/ios_audio_check2.png`, `/tmp/ios_audio_final.png` (iPad Simulator title) |
| KI-004 | Version string | Window title shows "UNKNOWN-VERSION" — `METAFORCE_VERSION_STRING` empty (git describe fails on shallow clone). Cosmetic; build-time only. | Open — local clone artifact | 2026-08-11 |
| KI-006 | Save dialog navigation | In the "No Memory Card found in Slot A" dialog (USA), the "Create Dolphin Card" option is displayed but **not reachable** via D-pad/stick navigation — Down from "Retry" wraps to "Continue Without Saving" (the choice table only exposes 2 entries). Likely an upstream FRME/table-config quirk; "Continue Without Saving" and "Retry" both work. | Open — upstream save-UI quirk | 2026-08-11 macOS, keyboard-verified |
| KI-007 | iOS Simulator build | Building Dawn for `SIMULATORARM64` needs two local fixes vs the device build: (1) Dawn's `protoc` host tool is built for the simulator target and can't run on macOS — fix: build a host protoc from the vendored protobuf 33 and pass `WITH_PROTOC`; (2) GLFW (macOS-only, needs Carbon.h) fails — fix: `-DDAWN_USE_GLFW=OFF`. Documented in `BUILDING.md`. | Open — local build workarounds | 2026-08-11 |
| KI-005 | Shutdown | Sending SIGINT to the running app terminated with SIGSEGV (exit 139) on every quit. | **FIXED** — same root cause and fix as KI-002 (ImGui shutdown ordering) | 2026-08-11; verified exit 0 after fix |
| KI-008 | iOS input (minor) | Simulator logs "You need UIApplicationSupportsIndirectInputEvents in your Info.plist for mouse support" (SDL/uikit). Mouse/hover input not yet declared for iOS; no impact on touch or the current title-screen flow. Relevant to future touch/mouse input work. | Open — cosmetic/minor | 2026-08-11 iPad Simulator |
| KI-009 | Save/load + New Game (critical) | `CInputStream::ReadBits` used `#if METAFORCE_TARGET_BYTE_ORDER == __LITTLE_ENDIAN` — **missing the trailing underscores** vs `__ORDER_LITTLE_ENDIAN__` used everywhere else. On AppleClang/macOS `__LITTLE_ENDIAN` (no underscores) is **not defined**, so the comparison was `1234 == 0` → the reader never byte-swapped while the writer did → every multi-byte game-state read was misaligned. This corrupted the deserialized player state (beam=6, enabledItems=0x400000, HP garbage) and **crashed every New Game** in `CPlayerGun::AcceptScriptMsg` (`x760_selectableBeams[6]` → null deref at 0x2), blocking all save/load and the whole new-game path. Works on Linux CI because glibc defines `__LITTLE_ENDIAN` transitively. | **FIXED (2026-08-11, 1 line)** — changed to `__ORDER_LITTLE_ENDIAN__`. **Verified:** New Game now plays the full intro cinematic and reaches actual Frigate Orpheon gameplay at 60 FPS (1000+ draw calls); the game-state round trip decodes correctly. Patch: `patches/2026-08-11-metaforce-fix-cinputstream-byte-order.patch` | Crash reports `Metaforce-2026-08-11-215214.ips`, `-223133.ips`, `-224535.ips`; lldb state dumps; log `/tmp/mf_fixed2.log` |
| KI-010 | Audio (new-game transition) | `CStaticAudioPlayer` had **no destructor calling `StopMixing()`**, so when the frontend was destroyed at the game transition the global `s_activePlayer` dangled; the next audio pump decoded freed (zeroed) memory where `loopEndCur == loopStartCur == 0` → `remTillLoop` stayed 0 forever → **infinite loop in the audio pump on the main thread**, freezing the game at a black screen with 100% CPU (sample: main thread 100% in `CStaticAudioPlayer::DecodeMonoAndMix`). | **FIXED (2026-08-11)** — added `~CStaticAudioPlayer() { StopMixing(); }` plus defensive guards in `Decode`/`DecodeMonoAndMix` for degenerate loop ranges. Patch: `patches/2026-08-11-metaforce-fix-staticplayer-dangling-mix.patch` | `sample` output `/tmp/mf_sample.txt`; lldb register/memory dumps |
| KI-011 | Saves (critical) | The raw card file was never updated on disk, so saves did not survive a relaunch. **Root cause:** `Card::open()` calls `resizeQueue(5)` (only the 5 system blocks), but file-data reads/writes pass the **block number as the AIO queue index** (`Card::asyncWrite` → `m_fileHandle.asyncWrite(curBlock, ...)`, blocks 5+) — out-of-bounds on the 5-slot queue (`m_queue[qIdx]` unchecked), silently losing the save data. Additionally `Card::commit()` queued header/dir/BAT aio writes without waiting, and `AsyncIO::~AsyncIO` **cancelled** pending ops (`aio_cancel`) on close, dropping them at unmount/shutdown. | **FIXED (2026-08-11)** — (1) `AsyncIO::asyncRead/asyncWrite` grow the queue when `qIdx >= size`; (2) `Card::commit()` calls `waitForCompletion()` so saves are durable before returning; (3) `AsyncIO::~AsyncIO` flushes pending ops instead of cancelling. **Verified:** new game writes `GM8E01`/`MetroidPrime B` to the card directory block on disk; after quit + relaunch the file select shows the save and pressing A **loads the saved game** (intro narration + gameplay at 60 FPS). Patch: `patches/2026-08-11-kabufuda-fix-card-queue-oob-and-commit-flush.patch` | Card hexdump `~/Library/Application Support/dolphin-emu/GC/MemoryCardA.USA.raw`; screenshots `/tmp/mf_reload_a2.png`, `/tmp/mf_reload_game.png`; logs `/tmp/mf_savefix.log`, `/tmp/mf_reload.log` |
| KI-006 (updated) | Save dialog navigation | Earlier finding claimed "Create Dolphin Card" was unreachable via D-pad. **Re-tested 2026-08-11 with the keyboard mapping: all three options are reachable** (Down×2 selects "Create Dolphin Card"), and selecting it creates + formats a 128 KB raw card and enters the file-select menu. | **Resolved** (was a test artifact of the earlier input setup) | 2026-08-11 keyboard-verified |
| KI-012 | iOS Simulator input (test infra) | Host keyboard events were not delivered to the Metaforce app inside the iPad Simulator (per-device `ConnectHardwareKeyboard` in `com.apple.iphonesimulator` was 0; toggling it had no effect; SDL received no key events). This blocks driving the frontend menus on the Simulator via CGEventPost. | **Workaround (2026-08-11)** — added an `--autostart` test hook (`CMain` flag + `CFrontEndUI` timer driver) that programmatically: ensures a Dolphin card, enters FileSelect, and confirms a New Game. Verified: full frontend flow → intro → Frigate Orpheon gameplay on the Simulator at 60 FPS, save persisted. Patch: `patches/2026-08-11-metaforce-autostart-test-hook.patch`. Real touch input (for device use) is a separate unimplemented item. | Open — keyboard forwarding remains unavailable; hook is the automation path | 2026-08-11 Simulator |
| KI-013 | App lifecycle on iOS (background/foreground) | SDL3's iOS backend reports backgrounding via `SDL_EVENT_DID_ENTER_BACKGROUND` / `WILL_ENTER_FOREGROUND` — **not** `WINDOW_MINIMIZED`/`RESTORED` (that mapping only exists on Android/desktop). Aurora's `lifecycle_event_watch` only mapped the minimized/restored pair, so `g_backgrounded` (and `window::is_presentable()`) never fired on iOS: the engine had no explicit pause/skip path on Apple mobile and kept attempting frames while backgrounded. | **FIXED (2026-08-12)** — mapped `DID_ENTER_BACKGROUND`/`WILL_ENTER_FOREGROUND` to `g_backgrounded` in aurora's event watch (Cocoa never emits these events, so macOS behavior is unchanged). Verified on the iPad Pro 13-inch (M5) Simulator from inside Frigate Orpheon gameplay: background → home screen, process alive, frame loop halts (simulation + present skipped); foreground → clean resume, HUD/state intact, CPU back to gameplay levels, RSS stable across 2 cycles (no leak). Patch: `patches/2026-08-12-aurora-fix-ios-background-events.patch`. Note: backgrounded CPU ~16% is the main loop's event-poll spin (no sleep); on physical iOS the OS suspends the process regardless. | Screenshots `/tmp/ios_lc_re3.png` (pre-bg), `/tmp/ios_lc_re_bg.png` (home), `/tmp/ios_lc_re_fg.png`, `/tmp/ios_lc_re_fg2.png` (resumes); `ps` CPU/RSS readings 2026-08-12 | 2026-08-12 Simulator |
| KI-014 | iOS shutdown crash (SIGABRT on exit) | Every `simctl terminate` on the iPad Simulator produced a crash report: the main thread hung in `SDL_CloseAudioDevice` → `COREAUDIO_CloseDevice` → `SDL_WaitThread`, so aurora's normal teardown (which clears the gx texture cache before Dawn/Metal teardown) never ran; the process was then force-killed and the gx texture-cache static destructor destroyed Dawn Metal textures via an XPC message after the Metal-simulator connection was invalidated → `abort()` on the backboard HID thread. Root cause of the hang: `aurora_audio_close` destroyed the SDL audio **stream before** closing the audio **device**, while the SDL CoreAudio thread was still draining from the stream during close. | **FIXED (2026-08-12)** — reorder `aurora_audio_close` to close the device first, then destroy the stream. Verified 3× on the iPad Pro 13-inch (M5) Simulator: `simctl terminate` returns 0 and no new crash report is generated (clean process teardown). Patch: `patches/2026-08-12-aurora-fix-ios-audio-close-hang.patch` | User crash report `Metaforce-2026-08-12-032038.ips`; `simctl terminate` exit 0 + DiagnosticReports count unchanged after fix | 2026-08-12 Simulator |
| KI-015 | iOS touch overlay attach regression | The ported SunPad UIKit overlay (`lib/ios/`, SunPad code nearly verbatim) renders identically to SunPad (verified 2026-08-12: green A, red B, gray X/Y, purple Z, gold C-stick, D-pad, L/R shoulders, START, ••• menu), but after a cleanup pass the attach now fails: "Failed to attach the UIKit touch overlay to the window". Hypothesis: `SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER` is null at the attach point in `aurora.cpp::initialize()` (right after `show_window()`); the overlay attached earlier in the session with the same code path, so timing/lifecycle is suspect — try attaching after the first frame / after the Metal surface exists, and log which check fails in `OverlayBridge.mm`. | **OPEN** — next-agent priority #1; see `docs/AGENT_GOAL_LOOP.md` §3 | Screenshots `/tmp/ios_sunpad_overlay2.png`, `/tmp/ios_menu_open2.png` (overlay rendering before the regression); warning logged by `aurora` at launch after regression | 2026-08-12 Simulator |

## Upstream notes

- Metaforce README: project in alpha; "builds currently unavailable while the
  project undergoes large changes." CI contradicts the "unavailable" claim;
  verify each platform empirically.
- SunPad (Apple reference) targets a different runtime (Dolphin static
  recompilation) — do not port its engine architecture.

## KI-001 investigation trail (2026-08-11)

Deep-dived the movie rendering bug. All of the following were verified correct:

- THP decode: dumped the YUV planes from the running app — the decoded frame is a
  perfect "Nintendo® and Retro Studios™ present" title card (correct 640x480 YUV420).
- Movie draw state: full-framebuffer quad (viewport 2560x1920, scissor 2560x1920,
  video 640x480, scale 4, playMode Playing — logged).
- Render pass: EFB target 2560x1920, observable, correct color view.
- Present: surface 2560x1920, source 2560x1920, viewport (0,0,2560x1920),
  fullscreen-triangle copy pipeline with correct UVs.
- 2D convention: the "identity" view matrix is the Retro y/z axis swap
  (1,0,0,0 / 0,0,1,0 / 0,-1,0,0) and is uploaded correctly (logged);
  quad (x, 0, z) → view (x, z, 0) → ortho → full screen, correct depth.
- TEV formula in aurora (`mix(a,b,c)+d`) matches the GC hardware equation
  (TevOut = A*(1-C) + B*C + D) — verified against the published equation.

Round 2 (GPU-side evidence, same day):

- **EFB readback:** copied the game framebuffer (`g_frameBuffer`, 2560x1920,
  BGRA8, CopySrc) to a staging buffer via `CopyTextureToBuffer` + `MapAsync`
  (`AllowSpontaneous`). Result at movie-draw time: **100% black** — the movie
  never reaches the EFB.
- **Surface readback:** copying the swapchain texture crashed the app
  (AGX `copyTextureToBuffer` EXC_BAD_ACCESS — CAMetalLayer drawables are not
  CopySrc). One valid dump (before the crash) showed the surface = black EFB +
  the ImGui overlay — i.e. the presented content matches the black EFB.
- **On-screen analysis (clean window-only capture, no dialogs):** the visible
  content is a **full-width band ~240 px tall near the top** with a subtle
  grid/checker texture — it does **not** match the correctly-decoded frame, so
  the GPU texture content is corrupted somewhere between upload and sampling.
- The EFB pass is `observable=true` (not skipped), target 2560x1920, and the
  `GXSetCopyFilter` vfilter is a no-op in aurora (not the cause).

Conclusion: the defect is inside aurora's GX WebGPU texture upload / EFB draw
path. The CPU-side decode is perfect; the GPU-side content is garbage. Next step:
a minimal aurora repro (single R8/I8 full-frame textured quad) with a shader
debug view, or capture the EFB via Xcode GPU frame capture.

Round 3 (same day):

- **Cull experiment:** added `GXSetCullMode(GX_CULL_NONE)` to the movie draw
  setup (`MyTHPGXYuv2RgbSetup`). This **changed the on-screen garbage into real
  movie content** (a circular instrument/gauge from the attract footage became
  visible) — so the per-frame `CullMode::Front` was partially culling the movie's
  CW-wound triangles. Saved as
  `patches/2026-08-11-metaforce-movie-cull-experiment.patch`; kept in the working
  tree as a candidate fix, still not a full fix (geometry remains wrong).
- **EFB readback (validated, no crash):** re-ran the readback with the normal
  present flow intact (no early return). At frame 600 the EFB is **still 100%
  black** while the window shows movie content — the visible content is not in
  `g_frameBuffer`. Combined with the surface dump (black + ImGui), the presented
  frames are black, yet the window shows garbage — pointing to an inconsistency
  in the aurora GX WebGPU present/draw path that needs Xcode GPU frame capture to
  pin down. This is a current-tree upstream defect affecting all platforms.

Round 4 — ROOT CAUSE FOUND AND FIXED (same day):

- The frame-start refresh in aurora (`gfx/common.cpp`, `begin_frame`) sets the
  cached draw scissor from the GX **logical** scissor. The game's per-frame
  `CCubeRenderer::BeginScene` set only the viewport, never the scissor — so the
  logical scissor (stale 640×480, the GX default) was used for every frontend
  draw, clipping everything to a 640×480 region. Same for the logical viewport
  (metaforce used only the aurora `GXSetViewportRender` override, which the
  refresh then clobbered).
- Fixes applied in the working tree (see table above). Verified on macOS:
  frontend movies full-frame, title screen correct, in-game warp correct.
- Earlier EFB-black readbacks were reading the correct texture but at moments
  where the movie was clipped away by the 640×480 scissor; the "garbage" on
  screen was the tiny clipped fragment. All rounds of evidence are consistent
  with the scissor/viewport root cause.

Experimental changes that altered behavior but did not fix geometry:

- `GX_TF_R8_PC` → `GX_TF_I8` for the Y/U/V movie textures (the game's own code in
  PrimeDecomp/prime uses `GX_TF_I8`; the decomp's movie draw is still a stub).
  Result: ~10x brighter, full-range colors (p99 255 vs 52) but the small-corner
  geometry and garbling persisted. Kept as an experiment, reverted with the tree.
- Debug instrumentation saved as `patches/2026-08-11-frontend-movie-debug.patch`
  and `patches/2026-08-11-aurora-frontend-debug.patch` for resuming the hunt.

Conclusion: every game-side and present-side quantity inspected is correct, yet
the output is a small garbled strip. The defect is inside aurora's GX WebGPU
implementation (likely the EFB pass texture handling or the 2D draw path) and
needs GPU-side capture (framebuffer readback) to pin down. Documented as an
upstream issue; does not block the Apple-port gates (in-game rendering is proven;
see STATUS). Next step: reproduce with a minimal 2D-quad shader test in aurora.
