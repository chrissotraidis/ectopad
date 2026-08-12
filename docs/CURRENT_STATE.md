# Authoritative current state

Last audited: 2026-08-12

This is the canonical starting point for future work. When older dated evidence
describes an earlier limitation, this document and the newer dated entries in
`TESTING.md` take precedence. Historical documents are retained so conclusions
remain traceable; they are not permission to promote Simulator evidence into a
physical-device claim.

## Exact repository and environment state

- Root repository: `chrissotraidis/ectopad`, local branch `main`.
- Implementation baseline audited by this documentation pass:
  `a483fab` (`Harden SunPad menu and audio handoff`).
- Metaforce reference pin: `621ee0fed3fdd5fccbbdadbce33343f29dce071c`.
- Aurora submodule pin: `514339438178ef2bed1b14e5149d90ece0c6e0cc`.
- SunPad reference pin: `7d84cec8bb607c32d76499af0e7bd7435ac82d5e`.
- Host: Apple Silicon `arm64`, macOS 26.5 (25F71), Xcode 26.6 (17F113),
  CMake 3.27.1, Ninja 1.13.2.
- Code signing: `security find-identity -p codesigning` reports zero valid
  identities.
- Devices: `xcrun devicectl list devices` reports no connected devices.
- Physical-device Gates 2 and 3 therefore remain unexecuted, not failed.

`ref/` is ignored and contains separate working trees plus private user game
data. No `ref/` change is committed directly. Every source delta is mirrored
under `patches/` against the pinned source sequence.

## What the product is

Ectopad is native Metaforce for Metroid Prime NTSC-U Rev 2. The execution path
is Metaforce → Aurora GX compatibility → Dawn/WebGPU → Metal. It is not a
Dolphin, RetroArch, or other emulator wrapper. The app requires the user's own
legally obtained `GM8E01` revision-2 image and never packages Nintendo data.

## Build and package artifacts

Final-source builds completed successfully on 2026-08-12 for:

| Target | Artifact | Verified fact |
| --- | --- | --- |
| macOS Apple Silicon | `ref/metaforce/build/macos-default-relwithdebinfo/Binaries/Metaforce.app` | arm64 Mach-O; prior Gate 1 title/gameplay/Metal/audio/save evidence remains valid |
| iOS Simulator | `ref/metaforce/build/ios-sim/Binaries/Metaforce.app` | arm64 Simulator app; current SunPad interaction harness passed |
| iPhoneOS/iPadOS device | `ref/metaforce/build/ios-default/Binaries/Metaforce.app` | arm64 Mach-O, minimum iOS 14; unsigned and not installed on hardware |

The exact final menu/audio device app was packaged twice with identical bytes:

- IPA: `/tmp/Metaforce-unsigned-menu-audio-2026-08-12.ipa`
- SHA-256:
  `308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`
- Bundle: `com.axiodl.Metaforce`, version `0.0.1 (1)`, minimum iOS 14.0.
- Audit: ZIP structure, arm64/iPhoneOS platform, host linkage, bundle metadata,
  game/save/log/signing extensions, disc headers, credentials, private keys,
  local paths, and signing material passed.
- Known release boundary: 241 upstream Dawn CI paths remain in diagnostic
  strings. Public redistribution is not approved until GPL corresponding
  source, LGPL relink materials, and Dawn provenance remediation are complete.
- The IPA is unsigned and cannot be installed as-is on a normal physical
  device. It must not be publicly redistributed.

## Proven functionality

### Gate 1 — macOS

Proven on Apple Silicon through Dawn/Metal:

- native arm64 build and launch;
- full-frame title/frontend and attract rendering;
- New Game → intro → first-person Frigate Orpheon gameplay;
- selected later-area warp rendering;
- keyboard/mouse input and the normal SDL/Aurora gamepad software path;
- frontend music, streamed DSP/MIDI, in-game music and effects;
- save creation, clean quit, relaunch, and save reload;
- clean Aurora/ImGui shutdown after the fixed shutdown ordering.

This does not mean broad game completion, all rooms, or release-grade long-run
performance has been proven.

### Simulator-only mobile evidence

On iPad Simulator, Metaforce has reached Dawn/Metal, title, New Game, intro,
Frigate gameplay, save persistence, background/foreground, and clean shutdown.
On iPhone Simulator, the SunPad-only landscape presentation plus diagnostic
confirmation/share UI and log redaction were verified. These remain
Simulator-only claims.

## SunPad parity and menu/touch state

The mobile UI is a direct SunPad port. The following Metaforce/Aurora files are
byte-identical to SunPad at `7d84cec`:

- `SunPadGameOverlay.{h,mm}`
- `SunPadSettings.{h,mm}`
- `SunPadInputMixer.{h,mm}`
- `SunPadInputState.h`
- `SunPadControllerMapping.{h,mm}`

Diagnostics has only intentional Metaforce app-name/storage-name changes.
`OverlayBridge.mm` is Metaforce-specific glue; iOS desktop ImGui presentation
is suppressed so SunPad owns the mobile overlay.

Current-build deterministic UIKit wiring passed in one clean iPad Simulator
container with no ISO/GCM:

- recursive inventory of 19 menu titles;
- all 14 gameplay controls;
- A down/up through the real `SunPadInputMixer`;
- render scale, opacity, global size, hide-on-controller, and modern C-stick;
- layout editor selection, A per-control resize to 1.25×, reset, and preference
  restoration;
- empty Files-visible folder alert;
- real `UIDocumentPickerViewController` with `TouchOverlayDelegate`;
- final `interaction test result=0 restored=verified`;
- live bridge diagnostic `menu visible=1 attached=1` at the expected top-right
  frame.

Aurora now reasserts the whole SunPad overlay above SDL's UIKit/Metal hierarchy
once per second on the main thread. This specifically addresses the report that
controls were visible but the three-dot menu disappeared during gameplay.

Evidence:

- `/tmp/ipad-menu-audio-focused-2026-08-12.png`
- `/tmp/ipad-menu-audio-focused-2026-08-12.log`
- patches `2026-08-12-aurora-sunpad-interaction-menu-visibility.patch` and the
  prerequisite SunPad patch sequence.

The harness proves the current UIKit targets, state, and delegate wiring. It
does not prove finger delivery, multitouch ergonomics, drag behavior, or a
complete touch-driven Prime playthrough on physical glass.

## Menu action state

The unchanged SunPad menu contains and wires:

- Render Resolution: 1×/2×/3×/4× live framebuffer scale.
- Aspect Ratio: Original 4:3, experimental 16:9, experimental Fill.
- Show FPS Counter: successful-present rate.
- Touch Control Settings: opacity, global/per-control size,
  hide-on-controller, modern C-stick, editor, reset.
- Game Data & Saves: Files picker, Files-visible folder import, removal.
- Controller Button Mapping: persisted A/B/X/Y/Z mapping.
- Share Diagnostic Log: privacy confirmation, snapshot, UIKit share sheet.

The inherited `Experimental 60 FPS (Restart Required)` item is not claimed as
a validated GM8E01 feature. Do not advertise or expand it without a separate
Prime timing audit.

## Game-data and save state

The importer accepts only the supported raw GameCube image:

- exact size 1,459,978,240 bytes;
- `GM8E01`, disc 0, revision 2, GameCube magic;
- canonical SHA-1
  `1a737910b55b59c6ad91be9e3e3c43517fd52efb`.

It holds security-scoped access, stages a mode-0600 private copy, fsyncs,
validates staged SHA-1, atomically renames within the same directory, excludes
the image from backup, and cleans staging on failure. Removal deletes game data
without deleting saves, settings, caches, or logs. Simulator service tests
proved invalid-size rejection, corrupt full-size rejection, valid activation,
restart, removal, reimport, and save preservation. Current UIKit harness proves
that the production Files picker and empty-folder delegate paths present.
Actual physical Files selection/import remains required.

## Audio state

Metaforce uses restored amuse mixing, streamed DSP/MIDI, soxr voice resampling,
and SDL3 output. SunPad's Super Mario Sunshine audio investigation was reviewed
before the latest change:

- SunPad's main 12× guest-timebase defect belongs to its static-recompiled
  Dolphin-derived core and does not apply to native Metaforce.
- The relevant transferable lesson is to keep a bounded reserve, preserve the
  consumer's true position, and never recover by replaying stale samples.

The prior Metaforce pump produced one 60 Hz slice for every rendered frame.
When rendering fell below 60 FPS, the producer could mathematically fall behind
the device. The current pump measures output-ready audio after SDL conversion
and tops a 120 ms reserve with fresh samples, independent of current render
rate. Empty-queue recoveries are counted and logged.

Final macOS, Simulator, and device targets compile this logic. A loaded native
`--warp 2 2` run opened 44.1 kHz output, mixed 6–7 active voices/3 submixes, and
held 5,292–5,294 output frames against a 5,292-frame target with no underrun
messages, even while unrelated renderer load was high. This is mechanical
correctness/stress evidence, not an audible-quality or performance benchmark.

Evidence: `/tmp/metaforce-audio-reserve-native-2026-08-12.log` and patch
`2026-08-12-aurora-frame-rate-independent-audio-reserve.patch`.

Still required: listen on physical hardware across frontend, cinematics,
gameplay, overlapping effects, route/interruption changes, background/resume,
and sustained sessions.

## Controller and diagnostics state

- The normal SDL → Aurora → `PADRead` software path is proven with an opt-in
  virtual controller: attach, Start, analog Y, and A reached the game.
- SunPad's mapping store/UI is byte-identical. Deterministic tests prove default
  mapping, A/B swap, passthrough, persistence, and the mapped iOS pad boundary.
- Physical Apple controller discovery, reconnect, mapping, rumble, and
  touch/controller handoff are not tested.
- SunPad diagnostics persist/rotate, redact current app/temp paths in new log
  lines, confirm disclosure, snapshot, and present the share sheet. iPhone
  Simulator UI/privacy tests pass. Physical export inspection remains.

## Exact remaining work

### Blocked only by external prerequisites

1. Install a valid Apple Development certificate/provisioning profile.
2. Connect a physical iPad and preferably a physical iPhone.
3. Sign the exact final device app, install it, and audit the post-sign artifact.

### Gate 2 — physical launch/render

On hardware, prove and capture:

- installed executable is arm64/iPhoneOS;
- launch does not crash;
- Dawn selects Metal and creates a visible surface;
- title/game surface renders;
- SunPad controls and `•••` stay visible above gameplay;
- diagnostic log contains no fatal Metal, import, or audio initialization error.

### Gate 3 — physical gameplay acceptance

Using the production Files flow and real touch, exercise Frigate Orpheon:

- movement, camera/aim, lock-on;
- beam, missile, jump, scan;
- Morph Ball;
- Start/pause and game menus;
- grouped D-pad where applicable;
- simultaneous/multitouch behavior;
- editor drag, per-control resize, reset, persistence;
- save/reload;
- background/foreground, audio interruption, and relaunch;
- sustained audible music/voice/effects without crackle, loss, wrong pitch,
  doubling, or silence;
- memory headroom and thermal state.

### Later confidence work

- representative sustained sessions in Chozo Ruins, Tallon Overworld,
  Phendrana Drifts, Magmoor Caverns, and Phazon Mines;
- physical GameController discovery/reconnect/rumble/mapping/handoff;
- in-world save station and upgrade persistence;
- quiet-system frame-time, RSS, thermal, and audio-underrun measurements;
- release-source/relink/provenance work and final signed-package audit.

## Work that should not be repeated next

- Do not spend another session on Simulator menu automation unless a physical
  result exposes a reproducible product defect.
- Do not redesign or restyle SunPad UI.
- Do not replace SunPad controls with ImGui.
- Do not copy SunPad's static-recompiler timebase patch into Metaforce.
- Do not quote the loaded Codex/Simulator FPS samples as benchmarks.
- Do not bundle, upload, or commit game data, saves, logs, provisioning, keys,
  certificates, or signed private artifacts.
- Do not mark the persistent goal complete from builds or Simulator evidence.

## Documentation audit map

The 2026-08-12 final audit reconciled all state-bearing documentation:

- authoritative status: this file and `STATUS.md`;
- goal/gates: `AGENT_GOAL_LOOP.md`, `IMPLEMENTATION_PLAN.md`,
  `COMPLETION_AUDIT.md`;
- UI: `SUNPAD_PARITY.md`;
- evidence: `TESTING.md`, `PERFORMANCE.md`, `KNOWN_ISSUES.md`;
- execution/handoff: `HANDOFF.md`, `BUILDING.md`, `INSTALL_IPA.md`;
- boundaries: `GAME_DATA.md`, `LEGAL_AND_PROVENANCE.md`,
  `DEPENDENCIES.md`, `THIRD_PARTY_NOTICES.md`;
- architecture: `ARCHITECTURE.md`;
- historical end-of-session snapshot: `STOP_FOR_TODAY_2026-08-12.md`.

`THIRD_PARTY_NOTICES.md` remains accurate and unchanged. The old snapshot is
retained but points here for the current state.
