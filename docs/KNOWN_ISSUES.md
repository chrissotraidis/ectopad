# Known Issues

Last updated: 2026-08-11

| ID | Area | Issue | Status | Evidence |
| --- | --- | --- | --- | --- |
| KI-001 | Frontend presentation | Intro/title/attract THP movies rendered clipped/garbled (band/block in a corner) instead of filling the window. | **FIXED (2026-08-11, local patch)** — root cause: the game's per-frame `BeginScene` set the viewport but **never set the scissor**, and aurora's frame-start refresh derives the draw scissor from the GX logical state (stale 640×480) — so all frontend draws clipped to a 640×480 region. Fixes: (1) `CCubeRenderer::BeginScene` now sets a full-frame scissor every frame; (2) `CGraphics::SetViewport`/`ConfigureFrameBuffer`/`ClearBackAndDepthBuffers`/`SetDepthRange` also call the standard `GXSetViewport` so the logical viewport stays in sync; (3) movie draw sets `GX_CULL_NONE` (CW-wound triangles were culled by the per-frame `CullMode::Front`). **Verified: title screen renders full-frame ("METROID PRIME" gold logo + emblem at 60 FPS); attract footage renders full-frame; in-game warp renders correctly (1,752 draw calls).** Patch: `patches/2026-08-11-metaforce-fix-frontend-viewport-scissor.patch` | Screenshots `/tmp/mf-fixed-view.jpg`, `/tmp/mf-verify3-view.jpg` (title), `/tmp/mf-warp2-view.jpg` (in-game) |
| KI-002 | Diagnostics (ImGui) | `ImGui_ImplWGPU_RenderDrawData` segfaults (null backend data, EXC_BAD_ACCESS at 0xb0) on aurora's render worker thread. Root cause: shutdown order — `imgui::shutdown()` (nulls `BackendRendererUserData`) ran before `gfx::shutdown()` drained the render worker's pending end-frame callbacks, which still call `imgui::render`. This made **every app exit segfault** (exit 139) and generated a crash dialog each time. | **FIXED (2026-08-11, local patch)** — reorder `aurora::shutdown()` to `gfx::shutdown()` before `imgui::shutdown()`, plus a `g_initialized` guard in `aurora::imgui::render`. Verified: two consecutive app quits exit code 0, no crash report generated. Patch: `patches/2026-08-11-aurora-fix-imgui-shutdown-crash.patch` | User crash report `Metaforce-2026-08-11-165907.ips` (shutdown path: `aurora_shutdown → gfx::shutdown → render_worker::synchronize` vs worker in `ImGui_ImplWGPU_RenderDrawData`) |
| KI-003 | Audio | Upstream tree has no audio output device code and the amuse game-audio engine was removed entirely (musyx vendored but unused; `CSfxManager`/`CStreamAudioManager`/`CMidiManager` amuse calls commented out). | **FIXED (2026-08-11, local patches)** — (1) SDL3 output device in aurora + restored frontend RSF music (patch `patches/2026-08-11-metaforce-enable-audio-output.patch`); (2) vendored amuse + Hecl deps (athena, logvisor, lzokay), built atdna's DNA codegen for LLVM 22 (fixes: C++20, alias-template/dependent-specialization canonical-type handling, Integral Seek args), pre-generated the three DNA sources, and wrote a software mixer backend `Runtime/Audio/SDLBackend.{hpp,cpp}` (amuse `IBackendVoiceAllocator` → stereo float mix, mirroring the boo mixer it replaces); restored `CAudioSys` engine plumbing + `CSfxManager` play/emitter/listener/aux paths (patch `patches/2026-08-11-metaforce-amuse-in-game-audio.patch`; vendor script `patches/amuse-audio-vendor/vendor-amuse-audio.sh` with pins). **Verified on macOS:** all 5 Prime audio groups parse and load into amuse (pool/proj/sdir DNA reads correct — previously out-of-bounds); in-game warp runs SFX continuously (4–5 voices, 3 submixes) at 60 FPS, 1,744 draw calls; frontend music also plays; stable pump; clean exit. **Remaining:** no resampler — voices at 24/16/12/4 kHz play at wrong pitch (soxr next); `CStreamAudioManager`/`CMidiManager` (streamed music/sequences) still to restore. | Logs `/tmp/mf_warp_amuse.log`; screenshot `/tmp/mf_amuse_in_game.png` (Chozo-ruins area, 60 FPS) |
| KI-004 | Version string | Window title shows "UNKNOWN-VERSION" — `METAFORCE_VERSION_STRING` empty (git describe fails on shallow clone). Cosmetic; build-time only. | Open — local clone artifact | 2026-08-11 |
| KI-006 | Save dialog navigation | In the "No Memory Card found in Slot A" dialog (USA), the "Create Dolphin Card" option is displayed but **not reachable** via D-pad/stick navigation — Down from "Retry" wraps to "Continue Without Saving" (the choice table only exposes 2 entries). Likely an upstream FRME/table-config quirk; "Continue Without Saving" and "Retry" both work. | Open — upstream save-UI quirk | 2026-08-11 macOS, keyboard-verified |
| KI-007 | iOS Simulator build | Building Dawn for `SIMULATORARM64` needs two local fixes vs the device build: (1) Dawn's `protoc` host tool is built for the simulator target and can't run on macOS — fix: build a host protoc from the vendored protobuf 33 and pass `WITH_PROTOC`; (2) GLFW (macOS-only, needs Carbon.h) fails — fix: `-DDAWN_USE_GLFW=OFF`. Documented in `BUILDING.md`. | Open — local build workarounds | 2026-08-11 |
| KI-005 | Shutdown | Sending SIGINT to the running app terminated with SIGSEGV (exit 139) on every quit. | **FIXED** — same root cause and fix as KI-002 (ImGui shutdown ordering) | 2026-08-11; verified exit 0 after fix |

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
