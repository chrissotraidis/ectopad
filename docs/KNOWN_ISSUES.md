# Known Issues

Last updated: 2026-08-11

| ID | Area | Issue | Status | Evidence |
| --- | --- | --- | --- | --- |
| KI-001 | Frontend presentation | Intro/title/attract THP movies render as a small, garbled, horizontally-banded strip in the upper-left corner instead of filling the window (FPS 60, 1-2 draw calls). The frontend state machine itself works: OpenCredits(10 s intro) → Title(30 s loop + press-start) → Attract cycles correctly (verified via logs). | Open — upstream WIP rendering bug in the THP/movie path, NOT Apple-specific (platform-independent aurora GX code) | 2026-08-11 macOS run; logs `/tmp/metaforce-*.log`; screenshots |
| KI-002 | Diagnostics (ImGui) | With `+developer=1`, `ImGui_ImplWGPU_RenderDrawData` can segfault (null backend data) on aurora's render worker thread; happened during a `--warp` run after ~55 s (Frame 3291). Plain (non-developer) runs were stable for minutes. | Open — upstream bug | Crash: `Metaforce-2026-08-11-152126.ips`; EXC_BAD_ACCESS at 0xb0 |
| KI-003 | Audio | No audio output device code in the current tree (CoreAudio/SDL-audio/AudioQueue absent; musyx playback calls commented out in `CSfxManager.cpp`; `boo` voice engine commented out in `CMain.cpp`). Audio assets load without error. | Open — upstream mid-refactor gap | Tree inspection 2026-08-11 |
| KI-004 | Version string | Window title shows "UNKNOWN-VERSION" — `METAFORCE_VERSION_STRING` empty (git describe fails on shallow clone). Cosmetic; build-time only. | Open — local clone artifact | 2026-08-11 |
| KI-005 | Shutdown | Sending SIGINT to the running app terminated with SIGSEGV (exit 139) on one occasion. Unclear if shutdown-path bug or ImGui interplay. | Open — investigate | 2026-08-11 session 57908 |

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
