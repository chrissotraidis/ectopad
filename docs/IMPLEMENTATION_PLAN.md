# Implementation Plan

Last updated: 2026-08-12

This originated before major implementation and is now a living gate plan.
[CURRENT_STATE.md](CURRENT_STATE.md) is authoritative for exact artifacts and
proof boundaries.

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

- **Verified (2026-08-11): the current tree does not use HECL extraction.** The
  runtime reads the user's disc image directly via nod-backed `CDvdFile` (ISO/GCM/
  RVZ/CISO supported). On iOS/tvOS the app expects `<store root>/game.iso`; desktop
  accepts a CLI path, file drop, or ImGui selection.
- Therefore the "preparation" pipeline is: **validate → stage privately → atomic
  activation of `game.iso`**. No extraction of Nintendo assets is needed for the
  base flow, which also minimizes copyrighted-data exposure. Keep the app
  architecture ready for a future extracted-assets path if upstream introduces one.
- A failed import leaves the previous `game.iso` intact; original data can be
  removed without touching saves unless the user explicitly asks.

## 6. Private staging and activation

- All generated data lives under ignored or private app-container paths.
- **Implemented 2026-08-12:** iOS copies from the security-scoped Files URL to a
  mode-0600 staging file beside private `game.iso`, fsyncs, verifies canonical
  SHA-1, activates with atomic `rename(2)`, and fsyncs the directory. The service
  is serialized; errors clean staging and preserve the active image. Saves live
  separately and survive reimport and removal. Simulator evidence covers
  invalid size, valid activation/boot, removal/reimport, and corrupt full-size
  SHA rejection.

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

- **Current state (verified 2026-08-11):** the musyx library (AxioDL fork of the
  GameCube audio engine) is vendored and complete, but the **host integration is
  missing** — the amuse/boo voice-engine calls are commented out in
  `CSfxManager.cpp`, `CStreamAudioManager.cpp`, `CMidiManager.cpp`, `CMain.cpp`,
  and there was no audio output device anywhere in the tree (KI-003). This is an
  upstream mid-refactor gap.
- **Progress (2026-08-11, macOS):** SDL3 output device in aurora; frontend
  music restored; **amuse in-game audio works** — vendored amuse + athena +
  logvisor + lzokay (pins + vendor script in `patches/amuse-audio-vendor/`),
  pre-generated DNA serialization sources, a software mixer backend
  (`Runtime/Audio/SDLBackend.*`), restored `CAudioSys` engine + `CSfxManager`.
  All 5 Prime audio groups load; in-game SFX voices play at 60 FPS.
  Patches: `patches/2026-08-11-metaforce-enable-audio-output.patch`,
  `patches/2026-08-11-metaforce-amuse-in-game-audio.patch`.
- **Implementation plan:**
  1. ~~Add a minimal audio device module (SDL3 audio — aurora already links SDL3
     with CoreAudio on macOS and the AudioToolbox/CoreAudio stack on iOS).~~ **Done**
  2. ~~Restore the DSP stream path in `CStreamAudioManager` (DSPADPCM → PCM decode
     via musyx's decoder; feed the ring buffers to the device).~~ **Done**
     (stream voices via the SDLBackendVoice supply callback with per-stream-file
     `ResetSampleRate`; verified 6–7 voices in-game, 2026-08-11)
  3. ~~Initialize the amuse engine and connect `CSfxManager`/`CAudioSys`~~ **Done**
     (group sets load into amuse; SFX verified in-game).
  4. ~~Wire the per-frame mix pump to the device callback~~ **Done**
     (amuse mix summed with the static player in the SDL supply callback).
  5. ~~Add a voice resampler (soxr) so non-32 kHz samples (24/16/12/4 kHz) play
     at correct pitch; restore `CStreamAudioManager`/`CMidiManager` streamed
     music/sequences.~~ **Done**
     (soxr vendored; streamed DSP + MIDI sequencer restored; verified on macOS
     and the iPad Simulator, 2026-08-11)
  6. ~~Decouple production from rendered-frame cadence.~~ **Done** (the output
     stream now targets a measured 120 ms converted reserve; loaded native
     evidence held 5,292–5,294 frames with no underrun log).
  7. Verify audibly on hardware: music, ambience, weapons, voice/SFX,
     transitions, route/interruption, background/resume, and sustained play.
- Gate 1 audio is proven. Physical mobile audible acceptance remains.

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
- **Build proven 2026-08-12:** the device preset links a Mach-O arm64 app. The
  cross-compile avoids host pkg-config libraries and vendors static zstd. Actual
  signing/install/launch remains a physical-device prerequisite.

## 18. Files import flow

- **Implemented behind the unchanged SunPad UI:** `••• → Game Data & Saves` →
  pick the user's ISO/GCM via Files (or the Files-visible app folder) → validate
  exact GM8E01 Rev 2 header/size → prepare privately → validate staged SHA-1 →
  activate atomically. Reimport and safe removal preserve saves. The service is
  Simulator-proven; the final harness presents the production picker and
  delegate. Actual physical selection/import remains.

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
- **Status (2026-08-12): SunPad direct port in place.** The earlier independent
  ImGui implementation was rejected and superseded. Aurora now hosts SunPad's
  UIKit overlay, settings, input mixer/state, layout editor, and diagnostics
  nearly verbatim; see [SUNPAD_PARITY.md](SUNPAD_PARITY.md). KI-015 attachment
  is fixed and the controls render over Dawn/Metal on the iPad Simulator.
  Render scale, aspect presentation, and the successful-present FPS counter are
  now wired through a thin bridge and Simulator-verified without modifying the
  SunPad sources. The final current-build harness passes menu/settings/editor/
  mixer/folder/picker phases and the live bridge reports `•••` visible and
  attached. Controller mapping and diagnostic confirmation/share are wired and
  Simulator-proven. Remaining: physical finger/multitouch/editor ergonomics,
  physical Files selection, physical iPhone/iPad layout acceptance, and touch/
  controller handoff. Experimental 60 FPS is intentionally not claimed.

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

- None proven fundamental. KI-001 is fixed locally; audio output and the
  frame-rate-independent reserve are wired locally. Watch items are Metaforce
  alpha-state churn, physical Dawn/Metal compatibility, SDL3 iOS device
  behavior, physical audible audio acceptance, and public-release provenance/
  license obligations.

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

1. Install/configure a valid Apple Development identity and provisioning
   profile; connect a physical iPad and preferably iPhone.
2. Sign the exact final arm64 device app, install it, and perform a post-sign
   privacy/platform audit without modifying it.
3. Execute Gate 2: physical Dawn→Metal launch, visible rendering, persistent
   SunPad controls/`•••`, and diagnostic capture.
4. Execute focused Gate 3 physical acceptance: production Files import, real
   touch/multitouch/editor, Frigate controls, save/reload, lifecycle, and
   audible audio across routes/interruptions and sustained play.
5. Only then collect controlled performance/memory/thermal data and later-area
   evidence with competing applications closed.
