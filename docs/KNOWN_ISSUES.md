# Known Issues

Last updated: 2026-08-11

| ID | Area | Issue | Status | Evidence |
| --- | --- | --- | --- | --- |
| KI-001 | Frontend presentation | Intro/title THP movie presents as a small fragment in the top-left instead of filling the window (1-2 draw calls, 60 FPS). Frontend state machine runs (OpenCredits → Title). | Open — believed upstream WIP (commit `621ee0f` "impl visors, warp & water (WIP)") | 2026-08-11 macOS run; screenshots + logs |
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
