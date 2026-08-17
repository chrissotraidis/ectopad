# Authoritative current state

Last audited: 2026-08-17 after the first-door/black-geometry physical trace

This is the canonical starting point for future work. When older dated evidence
describes an earlier limitation, this document and the newer dated entries in
`TESTING.md` take precedence. Historical documents are retained so conclusions
remain traceable; they are not permission to promote Simulator evidence into a
physical-device claim.

## Exact repository and environment state

- Root repository: `chrissotraidis/ectopad`, local branch `main`.
- Root implementation baseline before this publish pass:
  `47a928e` (`Merge pull request #1 from chrissotraidis/agent/publish-physical-prereq-audit`).
- Metaforce reference pin: `621ee0fed3fdd5fccbbdadbce33343f29dce071c`.
- Aurora submodule pin: `514339438178ef2bed1b14e5149d90ece0c6e0cc`.
- SunPad reference pin: `7d84cec8bb607c32d76499af0e7bd7435ac82d5e`.
- Host: Apple Silicon `arm64`, macOS 26.5 (25F71), Xcode 26.6 (17F113),
  CMake 3.27.1, Ninja 1.13.2.
- Code signing and hardware: the final 2026-08-13 build was development-signed,
  strictly verified, installed in place, and launched on the attached physical
  iPad as **EctoPad** (`com.axiodl.Metaforce`, 0.1.3 (1)).
- Before and after installation, `game.iso` and both 16 MiB memory cards were
  copied from the device and compared byte-for-byte. Their SHA-256 values are
  recorded in [TECH-DEBT.md](TECH-DEBT.md). No uninstall or destructive
  container replacement was used.
- The installed build reaches title and gameplay. The SunPad-native menu,
  render/aspect/FPS actions, left-stick movement, and single-pass Prime file
  menu have physical acceptance. Gate 3 remains open on transition audio,
  save/reload, lifecycle recovery, first-door animated presentation,
  black/distant geometry, and later-game stability.
- The later 20:21 in-place update adds the iOS HDMI secondary-scene guard and
  original green ectoplasm app icon. It is running as PID 2122; pre/post ISO and
  both memory-card hashes remained byte-identical. HDMI no longer has an
  unexplained crash signature, but physical accessory replay is still required.
- A 34-minute-9-second controller session before that update had zero logged
  audio underruns/clipping and no fatal, jetsam, GPU, hang, or memory-warning
  marker. The logs did not persist controller enumeration, so the controller
  itself is hands-on rather than machine-recorded evidence.
- The unsigned validation IPA remains a package-audit artifact only. Physical
  install must sign the source `.app` at
  `ref/metaforce/build/ios-default/Binaries/Metaforce.app`, not that IPA.
- The 2026-08-17 physical comparator conclusively separates the first door from
  ordinary doors: UIDs 410/409 are a same-area scripted `Inside/Outside` pair,
  while ordinary inter-area doors use the same animation assets and work. The
  logical Open/collision and CPU pose complete, but visible presentation does
  not. The instance-shared workspace diagnostic is not sufficient to locate the
  draw failure; see the KI-022 P0 record in `TECH-DEBT.md`.
- Four new physical screenshots reject the static-area vertex-array lifetime
  candidate as the black-geometry solution. The defect predates screenshot
  lifecycle events, is selective rather than global fog/exposure, and has no
  logged Dawn/Metal failure. The next renderer work is a bounded per-surface
  Mac/iPad state comparison, not another texture-by-texture correction; see
  KI-023 in `TECH-DEBT.md`.
- The current signed EctoPad 0.1.3 (1) device build was installed in place and
  launched on the paired physical iPhone 14 on 2026-08-17. The same bundle ID
  was retained and there was no uninstall, data-container replacement, or ISO
  transfer. This is the selected source/build baseline for the next IPA pass;
  it does not convert the open KI-022/KI-023 physical visual failures into
  accepted fixes.
- The subsequent iPhone playthrough physically reproduces the first-door and
  black-world-geometry symptoms at 2x after foreground resume. Its diagnostic
  trace repeats the iPad door comparator and contains no Dawn/Metal failure or
  render-setting change. Reported distance-dependent focus/sharpness changes
  remain uninstrumented and must not yet be labeled as a mip/LOD defect.

`ref/` is ignored and contains separate working trees plus private user game
data. No `ref/` change is committed directly. Source changes are recorded under
`patches/` against the pinned source sequence; rejected experiments are labeled
explicitly and must not be replayed as accepted fixes.

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
| iPhoneOS/iPadOS device | `ref/metaforce/build/ios-default/Binaries/Metaforce.app` | arm64 Mach-O, minimum iOS 14; later development-signed, installed, and launched on the attached iPad. Launch is not Gate 3 acceptance. |

The selected EctoPad 0.1.3 (1) device app was packaged twice on 2026-08-17:

- IPA: ignored local artifact `artifacts/EctoPad-0.1.3-unsigned.ipa`;
- size: 12,427,372 bytes;
- SHA-256:
  `f90b353617d81ce3e4f6a0ebedf52f7c10a39d4b969d48a78335b35263010086`;
- both independently generated packages were byte-identical;
- ZIP integrity, arm64/iPhoneOS platform, bundle/version/minimum OS, host
  linkage, Files/indirect-input metadata, disc headers, credentials, personal
  paths, game data, saves, logs, and signing-material exclusion passed;
- 241 upstream Dawn CI source paths remain embedded as non-local provenance.

This IPA is unsigned, not installable as-is, and remains a local validation
artifact. Its successful audit does not override the corresponding-source,
LGPL relink, or Dawn provenance release boundary below.

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
  device. It must not be publicly redistributed. The later physical install
  signed the source `.app` instead of this archive.

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

Physical listening on the attached iPad failed: no music, some SFX, mix
clearly wrong. KI-003 remains implementation-complete and audible-acceptance
open. See [TECH-DEBT.md](TECH-DEBT.md). Do not copy SunPad's 12× guest-timebase
patch.

## Controller and diagnostics state

- The normal SDL → Aurora → `PADRead` software path is proven with an opt-in
  virtual controller: attach, Start, analog Y, and A reached the game.
- Aurora now reconciles its SDL3 handle map with `SDL_GamepadConnected()` and
  `SDL_GetGamepads()` on add/remove, stale reads, and foreground resume. Five
  deterministic slot tests cover a missed removal, sole-controller reclaim,
  uninterrupted two-controller preservation, next-free-slot assignment, and
  resume reconciliation; all 180 Aurora tests pass. `PADRead` still begins by
  clearing every port status, so removing a stale handle releases held input.
- SunPad's mapping store/UI is byte-identical. Deterministic tests prove default
  mapping, A/B swap, passthrough, persistence, and the mapped iOS pad boundary.
- Physical Bluetooth, wired, and natural-sleep reconnect behavior, mapping,
  rumble, and touch/controller handoff are not accepted by automated evidence.
- SunPad diagnostics persist/rotate, redact current app/temp paths in new log
  lines, confirm disclosure, snapshot, and present the share sheet. iPhone
  Simulator UI/privacy tests pass. Physical export inspection remains.

## Exact remaining work

### Hardware launch happened; acceptance did not

The attached iPad launched a development-signed `Metaforce.app`. Title,
import, and Frigate gameplay are reachable. That closes the old "no identity /
no device" blocker and satisfies the minimum Gate 2 launch question. It does
not close Gate 3.

Current product blockers are in [TECH-DEBT.md](TECH-DEBT.md):

1. New Game → save → quit → relaunch → load using the now-complete A/B cards.
2. Audible audio on the same iPad.
3. Physical SunPad-native `•••` submenu/dismissal and 1×–4× behavior.
4. Original Prime movement, hold-R aim, L lock-on, and C-stick beam selection.
5. Named texture captures.
6. A documented bootstrap + in-place `devicectl` deploy path so the next
   hardware launch does not rediscover hidden vendor steps.

### Gate 2 — physical launch/render

Launch on the attached iPad happened. Remaining Gate 2 honesty items:

- keep install/PID distinct from gameplay acceptance;
- confirm the next installed binary is the current overlay/audio sources;
- keep `•••` actually usable, not merely present as a button.

### Gate 3 — physical gameplay acceptance

Gate 3 failed on the user's hands-on. Using the production Files flow and
real touch, the remaining Frigate checklist is still:

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
