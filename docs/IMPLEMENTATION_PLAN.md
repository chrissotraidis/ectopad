# Implementation Plan

Last updated: 2026-08-11

This plan is written **before major implementation**, per the project goal. It
will be updated as gates produce evidence.

## 1. Source / repository strategy

- **Engine:** Metaforce, pinned in `ref/metaforce` at `621ee0f` (shallow clone;
  submodules pinned by gitlink SHAs — see [DEPENDENCIES.md](DEPENDENCIES.md)).
- **Reference repos:** `ref/aurora` (standalone), `ref/prime` (decomp reference),
  `ref/sunpad` (Apple shell reference). All inside git-ignored `ref/`.
- **Our work:** Apple platform glue, app shells, input/touch UI, game-data
  import flow, diagnostics, packaging, and docs live in this repository
  (`ectopad`), layered on Metaforce as a pinned submodule-free external source
  (kept out of our git history to avoid bloat; pins recorded in docs).
- Upstream changes are tracked; a current-tree regression or an upstream fix
  moves the pin deliberately, with documentation.

## 2. Supported Metroid Prime revision

- Metroid Prime (USA) (Rev 2), serial DOL-GM8E-USA, game ID `GM8E01`,
  revision byte `0x02` at header offset `0x07`.

## 3. ISO/GCM validation

- Validated 2026-08-11: size 1,459,978,240 bytes; CRC32 `61592372`; MD5
  `fdfc41b8414dd7d24834c800f567c0f8`; SHA-1
  `1a737910b55b59c6ad91be9e3e3c43517fd52efb`; matches Redump "(USA) (Rev 2)"
  byte-for-byte. Procedure in [GAME_DATA.md](GAME_DATA.md).

## 4. Compressed-image conversion

- Not needed: the supplied image is a raw ISO. If a future image is
  `.nkit.iso`/`.rvz`/`.ciso`, convert locally with `dolphin-tool convert` or NKit
  tooling to raw ISO/GCM; keep outputs ignored; record new hashes.

## 5. HECL / game-data preparation

- Investigate the current HECL/asset pipeline in the pinned tree (it now lives
  partly inside aurora/nod tooling). Determine whether on-device preparation is
  viable or whether a Mac-side preparation tool produces a private importable
  package first.
- Whichever path wins: validate → stage privately → validate result → atomic
  activation; a failed import leaves the previous installation intact; original
  data can be removed without touching saves unless the user explicitly asks.

## 6. Private staging and activation

- All generated data lives under git-ignored local paths (e.g. `ref/.private/`).
- On iOS: prepare into the app container staging dir; activate via atomic
  rename/commit; saves live in a separate location and survive reimport.

## 7. Engine architecture

- Metaforce Runtime → Aurora (core/gx/pad/si/vi/mtx) → WebGPU(Dawn) → Metal on
  Apple; SDL3 app layer; musyx audio; nod DVD; kabufuda CARD.
- See [ARCHITECTURE.md](ARCHITECTURE.md).

## 8. Dawn / WebGPU

- Use `AURORA_DAWN_PROVIDER=package` with the pinned prebuilt
  (`dawn-darwin-arm64` / `dawn-ios-arm64`) for both gates; fall back to `vendor`
  source build only if a package proves unsuitable (e.g. deployment-target or
  ABI mismatch).

## 9. Metal backend

- Aurora sets `DAWN_ENABLE_METAL ON` on APPLE; confirm empirically at runtime
  (backend/adapter diagnostics) rather than assuming.

## 10. Window / surface integration

- SDL3 provides windowing/surface on macOS; Aurora's `vi`/GX swapchain path is
  used as-is. For iOS, SDL3's UIKit backend + CAMetalLayer is the expected path;
  verify.

## 11. Audio

- musyx through SDL3 audio on macOS; on iOS, verify route handling, interrupts,
  and lifecycle suspend/resume. Test music, ambience, weapons, voice/SFX,
  transitions, extended play.

## 12. Saves

- CARD-compatible saves via kabufuda (`CMemoryCardSys*`). Keep saves separate
  from removable game data; document macOS and iOS paths; test real save/reload.

## 13. Configuration

- Engine options + app settings; persistent `•••` settings menu (SunPad-style);
  store in App Support / Application Support container on iOS.

## 14. Controllers

- aurora PAD over `SDL_Gamepad`; implement complete GameCube surface: left stick,
  C-stick, D-pad, A/B/X/Y/Z, L (analog/full), R (analog/full), Start/Pause.
- Prime-native mapping (see §20); GameController devices; seamless
  touch/controller handoff.

## 15. Keyboard/mouse on macOS

- Practical keyboard/mouse input where Metaforce's architecture supports it;
  physical controllers as well.

## 16. macOS application shell

- `Runtime/platforms/macos` Info.plist template; Metaforce.app bundle; windowed
  and fullscreen behavior; keyboard/mouse focus; SunPad-quality diagnostics.

## 17. iOS/iPadOS application shell

- `Runtime/platforms/ios` shell; leetal `ios.toolchain.cmake`; `PLATFORM=OS64`,
  deployment target 14.0; produce `Metaforce.app` (upstream CI precedent).

## 18. Files import flow

- `••• → Game Data & Saves` → pick user's supported Prime image via Files →
  validate → prepare privately → validate → activate atomically. Reimport must
  not destroy saves; safe removal preserves saves.

## 19. Mobile data-storage design

- Separate containers: game data (removable/importable), saves, configuration,
  logs, temp/staging. App updates preserve data per Apple container semantics.

## 20. Lifecycle / background / foreground

- Handle backgrounding (pause audio, suspend rendering loop), foreground
  restoration (recreate surface, continue), interruptions, and memory warnings.

## 21. Touch controls

- Prime-native design (not a generic dual-stick port): movement, camera/aiming,
  lock-on, firing, charging, jumping, missiles, Morph Ball, beam/visor selection,
  scanning, menus, pause. Research the original control scheme before assigning
  hit targets. Keep the original control model available; any optional enhanced
  mode must be clean and documented.

## 22. iPhone layout

- Compact layout; draggable/resizable controls; normalized stored positions;
  reset-to-default.

## 23. iPad layout

- Large-screen layout with the same customization primitives; primary gameplay
  target.

## 24. Settings

- Persistent `•••` menu: game data & saves, controls, render scale (1×/2×/3×/4×
  where supported), aspect presentation (original-aspect safe default;
  experimental modes clearly marked), audio, diagnostics.

## 25. Diagnostics / logging

- SunPad-quality: startup, game-data validation, preparation, graphics init,
  Dawn backend selection, Metal/device problems, asset loads, audio init,
  controller issues, memory pressure, lifecycle events, crashes. Share Diagnostic
  Log (privacy-reviewed; no game assets/saves; redact container prefixes).

## 26. Performance instrumentation

- Frame timing, internal/drawable resolution, render scale, adapter/device info,
  major init failures; live render-scale controls where safe.

## 27. Memory instrumentation

- Track usage in Frigate Orpheon and representative later areas; memory-pressure
  handling; headroom evidence on real hardware.

## 28. Simulator strategy

- iPhone and iPad Simulators (one at a time) for UI, import flow, settings,
  storage paths, touch layout, diagnostics, and engine execution where the
  runtime supports it. Simulator-only results are labeled as such.

## 29. Physical-device strategy

- Primary target: iPad (and iPhone). Install via Xcode/developer signing only;
  no signing material in git. Verify Metal, data ingestion, rendering, audio,
  touch, GameController, lifecycle, saves, memory, thermals, extended play.

## 30. Packaging

- Metaforce.app (macOS), .ipa (iOS/iPadOS). Reuse SunPad packaging patterns where
  appropriate (upstream `ci/build-dmg.sh`, `ci/build-ipa.sh` exist).

## 31. Signing

- Local development signing only; provisioning profiles/certificates never
  committed. Document the device-install procedure (SunPad `ios-provision*`
  scripts as reference).

## 32. Distribution boundaries

- Source + legally redistributable deps only; no Nintendo data; audit every
  artifact; language: "native reimplementation; requires user-supplied legally
  obtained game data; no Nintendo data included; not affiliated with/endorsed by
  Nintendo."

## 33. Testing

- Continuous; one Simulator at a time; dated evidence per platform in
  [TESTING.md](TESTING.md); macOS areas list, iPhone/iPad Simulator lists,
  physical device lists per the goal.

## 34. Legal / provenance boundaries

- Per [LEGAL_AND_PROVENANCE.md](LEGAL_AND_PROVENANCE.md); elevated DMCA
  exposure assumed; no piracy content.

## 35. Known upstream blockers

- None proven yet. Candidate watch items: metaforce alpha-state churn; Dawn
  prebuilt package deployment-target compatibility; SDL3 iOS backend maturity.

## 36. Go / no-go criteria

- **Gate 1 (macOS ARM64):** clean build; game data prepared from supplied image;
  launch; Dawn selects Metal; frame renders; startup/title flow; input; audio
  init; saves/config paths understood. Fix current-tree build problems where
  reasonably possible rather than assuming docs are current.
- **Gate 2 (iOS/iPadOS device):** real ARM64 device build installs and launches;
  graphics device/surface created; Dawn/WebGPU reaches Metal; prepared Prime data
  loads; visible game rendering (ideally title screen). This is the principal
  technical unknown. If genuinely impossible after serious root-cause
  investigation, document the exact failure, logs, failing subsystem,
  experiments, upstream limits, code paths, smallest next step, and whether the
  blocker is fundamental or unfinished — then stop rather than fake progress.
- **Gate 3 (physical iPad gameplay):** Frigate Orpheon opening playable (stable
  rendering, audio, movement, camera, aiming, lock-on, Morph Ball, beams,
  missiles, jumping, scanning, menus, pause, save/reload, lifecycle recovery,
  memory headroom), then representative later areas.
- **Completion:** the goal's full completion condition list, with every claim
  evidenced and release artifacts audited.

## Immediate next steps

1. Configure and build macOS ARM64 (Gate 1).
2. Fix current-tree build problems as found.
3. Extract/validate game data; launch; verify Metal/frame/title/input/audio/saves.
4. Reproduce the iOS arm64 build locally; then device install (Gate 2).

