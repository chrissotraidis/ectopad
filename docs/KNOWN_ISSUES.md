# Known Issues

Last updated: 2026-08-11

| ID | Area | Issue | Status | Evidence |
| --- | --- | --- | --- | --- |
| KI-001 | Frontend presentation | Intro/title/attract THP movies render as a full-width, vertically-compressed band near the top of the window with a grid/checker artifact, instead of filling the window (FPS 60, 1-3 draw calls). The decoded frame is pixel-perfect on the CPU; the GPU shows garbage. The frontend state machine itself works: OpenCredits(10 s intro) → Title(30 s loop + press-start) → Attract cycles correctly (verified via logs). | Open — upstream WIP rendering bug in aurora's GX WebGPU texture/draw path, NOT Apple-specific | 2026-08-11 macOS run; logs + screenshots; EFB readback evidence |
| KI-002 | Diagnostics (ImGui) | `ImGui_ImplWGPU_RenderDrawData` segfaults (null backend data, EXC_BAD_ACCESS at 0xb0) on aurora's render worker thread. Root cause: shutdown order — `imgui::shutdown()` (nulls `BackendRendererUserData`) ran before `gfx::shutdown()` drained the render worker's pending end-frame callbacks, which still call `imgui::render`. This made **every app exit segfault** (exit 139) and generated a crash dialog each time. | **FIXED (2026-08-11, local patch)** — reorder `aurora::shutdown()` to `gfx::shutdown()` before `imgui::shutdown()`, plus a `g_initialized` guard in `aurora::imgui::render`. Verified: two consecutive app quits exit code 0, no crash report generated. Patch: `patches/2026-08-11-aurora-fix-imgui-shutdown-crash.patch` | User crash report `Metaforce-2026-08-11-165907.ips` (shutdown path: `aurora_shutdown → gfx::shutdown → render_worker::synchronize` vs worker in `ImGui_ImplWGPU_RenderDrawData`) |
| KI-003 | Audio | No audio output device code in the current tree (CoreAudio/SDL-audio/AudioQueue absent; musyx playback calls commented out in `CSfxManager.cpp`; `boo` voice engine commented out in `CMain.cpp`). Audio assets load without error. | Open — upstream mid-refactor gap | Tree inspection 2026-08-11 |
| KI-004 | Version string | Window title shows "UNKNOWN-VERSION" — `METAFORCE_VERSION_STRING` empty (git describe fails on shallow clone). Cosmetic; build-time only. | Open — local clone artifact | 2026-08-11 |
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
