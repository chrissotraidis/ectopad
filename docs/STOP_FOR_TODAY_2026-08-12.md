# Stop-for-today handoff — menu, touch, and audio

Last updated: 2026-08-12

This is the historical end-of-session snapshot from 2026-08-12.
[CURRENT_STATE.md](CURRENT_STATE.md) is now authoritative. The objective is broader than
today's stopping point, so this document deliberately separates what works now
from what still requires a physical device or longer acceptance testing.

## Bottom line

Metaforce is a real native Apple Silicon port, not an emulator wrapper. It runs
Metroid Prime NTSC-U Rev 2 through Aurora's Dawn/WebGPU to Metal on macOS and
the iPad Simulator. Prior runs reached the title, New Game, the intro, Frigate
Orpheon gameplay, audio, and save persistence. The unsigned physical-device
arm64 app also builds, but this Mac has no signing identity and no connected
iPhone/iPad, so physical Gates 2 and 3 are not claimed.

The mobile controls and menu are SunPad's implementation, directly ported:
`SunPadGameOverlay`, `SunPadSettings`, `SunPadInputMixer`, `SunPadInputState`,
and `SunPadControllerMapping` are byte-identical to the current SunPad
reference. Diagnostics differs only where the app name/storage name must say
Metaforce. Metaforce-specific behavior remains in `OverlayBridge.mm`; there is
no replacement ImGui touch UI on iOS.

Today fixed two immediate local-test concerns:

1. The full current-build SunPad interaction harness passed with result 0. It
   found the complete 19-title menu inventory and all 14 gameplay controls,
   drove A down/up through the real `SunPadInputMixer`, changed every touch
   setting, selected and resized A in the real layout editor, reset the layout,
   opened the empty-folder alert, opened the real Files picker with the correct
   delegate, and restored its preferences. The three-dot button was attached,
   visible, and at `{{1324, 12}, {40, 40}}` on the iPad Pro 13-inch Simulator.
2. Aurora now reasserts the whole SunPad overlay above SDL's UIKit/Metal view
   once per second from the main thread. This protects the controls and
   three-dot menu from later SDL view-hierarchy changes during live gameplay.
3. Audio production no longer assumes exactly 60 rendered frames per second.
   The previous code produced roughly 1/60 second of audio per rendered frame,
   so a 30–50 FPS interval could drain the SDL device and sound broken. The
   new pump measures output-ready audio after SDL conversion and tops up a
   bounded 120 ms reserve with fresh samples. Empty-queue recoveries are
   counted and logged; old audio is never replayed.

The audio change follows the applicable lesson from SunPad's long investigation:
protect a bounded reserve, preserve the true consumer position, and recover
with fresh/zero data rather than rewinding stale samples. SunPad's primary
Super Mario Sunshine root cause was a separate 12× guest-timebase bug in its
static recompiler, so that CPU-core patch was not copied into native Metaforce.

## What was proven today

- iOS Simulator build compiled and linked the direct SunPad overlay and initial
  120 ms audio-reserve implementation.
- Physical iPhoneOS arm64 build compiled and linked the same implementation.
- A clean, single-iPad Simulator run returned:
  - `phase=settings result=0 menu=19 controls=14`
  - `phase=editor result=0 selected=A size size=1.25`
  - `phase=folder result=0 title=Metaforce Folder`
  - `phase=files-picker result=0 class=UIDocumentPickerViewController`
  - `interaction test result=0 restored=verified`
  - `menu visible=1 attached=1 ... menuItems=8` (eight top-level menu groups;
    the recursive inventory contains 19 titles)
- The test container contained no ISO/GCM, so it could not alter or disclose
  game data. Its black game surface is expected and is not render evidence.
- Screenshot: `/tmp/ipad-menu-audio-focused-2026-08-12.png`.
- Log: `/tmp/ipad-menu-audio-focused-2026-08-12.log`.

Simulator programmatic control-path evidence is strong evidence that the menu,
settings, editor, mixer, and delegates are wired. It is not a substitute for a
finger on glass. The user's local/physical acceptance remains essential.

## Local test artifact and setup

The current build products are intentionally unsigned:

- Simulator app:
  `ref/metaforce/build/ios-sim/Binaries/Metaforce.app`
- Physical arm64 app:
  `ref/metaforce/build/ios-default/Binaries/Metaforce.app`
- Today's audited private unsigned IPA:
  `/tmp/Metaforce-unsigned-menu-audio-2026-08-12.ipa`, SHA-256
  `308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`.
  It includes today's menu/audio delta, but must be development-signed before
  physical installation and is not approved for public redistribution.

The app never bundles Nintendo data. Use the validated, legally obtained raw
GM8E01 Rev 2 ISO through the SunPad menu:

1. Tap `•••` at the top right.
2. Choose **Game Data & Saves → Change or Reimport Game Data**, then select the
   ISO/GCM in Files; or place one ISO/GCM in the app's Files-visible folder and
   choose **Import from SunPad Folder**.
3. Metaforce checks raw-disc size, `GM8E01`, disc 0, revision 2, GameCube magic,
   and the canonical SHA-1 before atomically activating a mode-0600 private
   copy. A failed import preserves the active image and saves.
4. Restart Metaforce after import when prompted.

## Five-minute acceptance checklist

Run this first on the iPad/iPhone with other heavy apps closed:

- [ ] Game reaches the title and then live gameplay through Metal.
- [ ] Touch controls remain visible over gameplay.
- [ ] `•••` is visible at the top right and opens on the first tap.
- [ ] Render Resolution and Aspect Ratio visibly apply; return to 1×/Original
      before judging performance.
- [ ] FPS counter toggles and remains above the game surface.
- [ ] Touch Control Settings changes opacity and all-control size.
- [ ] Move controls enters the yellow editor; selecting a control turns it
      blue, its per-control slider works, Done exits, and Reset restores it.
- [ ] Movement stick, C-stick, A/B/X/Y/Z, Start, L/R, and grouped D-pad respond.
- [ ] Frontend music, voice/cinematic audio, area music, beam fire, footsteps,
      UI sounds, and overlapping effects are complete and correctly pitched.
- [ ] Audio does not crackle, drop, speed up, or lose channels when FPS briefly
      dips or a new room/pipeline loads.
- [ ] Background for five seconds and return: rendering, touch, and audio all
      resume once, without doubled audio or silence.
- [ ] Save, quit, relaunch, and reload the save.

If audio is wrong, immediately note whether the visual FPS dipped and use
**••• → Share Diagnostic Log…**. Relevant messages include
`Audio device opened`, `targetBufferMs=120`, `Audio pump`, and
`Audio queue underrun detected`. Do not publish a diagnostic log without
reviewing it; it can contain device/app metadata and the source image filename,
though current app-container and temporary paths are redacted.

## Honest completion estimate

The core engineering needed for local testing is roughly 80–85% complete. The
full persistent objective is roughly 60–65% complete because the remaining
work is dominated by hardware-only acceptance and broad gameplay coverage, not
by another large architecture port.

What is already in place: native Metal execution, game-data validation/import,
SunPad controls/menu/settings/editor, display actions, diagnostic sharing,
controller mapping, audio engine/output, save persistence, iOS packaging, and
deterministic service tests.

What prevents completion: physical signing/install/render proof, comprehensive
touch gameplay, physical GameController behavior, audible physical-device
audio/lifecycle acceptance, later-area playthroughs, quiet-system performance/
memory/thermal measurements, and final redistribution/legal materials.

## Remaining checklist, in priority order

### P0 — user local acceptance

- [ ] Confirm the three-dot button stays visible and taps open during title,
      cinematics, and Frigate gameplay on the user's installation.
- [ ] Confirm every menu action and layout-editor gesture with real touch.
- [ ] Audibly evaluate the 120 ms audio reserve on frontend, cinematics, and
      gameplay. Simulator logs cannot prove speaker output quality.
- [ ] Capture the shared diagnostic log if the symptom persists.

### P1 — physical Gates 2 and 3

- [ ] Add a valid Apple development signing identity and provisioning profile.
- [ ] Sign without mutating the audited unsigned source artifact unexpectedly.
- [ ] Install on physical iPad and iPhone; prove native arm64, Dawn/Metal, and
      visible rendering (Gate 2).
- [ ] Complete Frigate Orpheon with movement, camera/aim, lock-on, Morph Ball,
      beam, missiles, jump, scan, menus, pause, save/reload, background/resume,
      memory, and sustained audio (Gate 3).
- [ ] Verify physical controller discovery, hot-plug/reconnect, mapping,
      rumble, and touch auto-hide/handoff.

### P2 — breadth and release confidence

- [ ] Representative sustained sessions in Chozo Ruins, Tallon Overworld,
      Phendrana Drifts, Magmoor Caverns, and Phazon Mines.
- [ ] Quiet-system performance baselines at 1× and optional higher scales.
      Record process load first; shut down builds, extra Simulators, log
      streams, and unrelated CPU/GPU-heavy apps. The earlier 20.4/59.9 contrast
      is functional FPS evidence only, not a benchmark.
- [ ] Measure frame-time distribution, RSS/headroom, thermal state, audio
      underrun count, and lifecycle recovery during long sessions.
- [ ] Exercise an in-world save station and upgrade/relaunch compatibility.

### P3 — distribution

- [x] Repackage the final device build twice and rerun deterministic/prohibited-
      content audit (identical SHA-256 `308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`).
- [ ] Post-sign audit on the exact artifact installed on hardware.
- [ ] Prepare/review GPL corresponding source and LGPL static-relink materials.
- [ ] Remove or rebuild upstream Dawn diagnostic provenance paths for a public
      release artifact.
- [ ] Final legal, privacy, game-data, saves, logs, credentials, provisioning,
      and signing-material audit.

## Important boundaries and known risks

- No physical-device result is proven in this environment: there are zero
  valid code-signing identities and no connected device.
- Host Simulator touch has been unreliable. The deterministic harness invokes
  the existing UIKit controls/actions and proves wiring, but cannot prove real
  gesture delivery or multitouch ergonomics.
- Today's audio diagnosis is structurally strong: the former producer rate was
  mathematically tied to render FPS. The new bounded-reserve logic still needs
  audible device acceptance, interruption-route testing, and long-session
  telemetry before KI-003 can be called fully closed on mobile.
- The experimental 60 FPS menu item is intentionally not claimed as wired for
  GM8E01. Native Metaforce normally targets 60 presented frames; this inherited
  SunPad label remains restart-required/reference UI and must not be marketed
  as a validated Prime mode.
- Do not commit anything under `ref/`; source deltas are mirrored as patches.
- Preserve the unrelated automation addition currently present in
  `docs/STATUS.md` unless its owner explicitly asks to change it.

## Resume order

1. Read `CURRENT_STATE.md`, then `COMPLETION_AUDIT.md`, `SUNPAD_PARITY.md`, and
   `KNOWN_ISSUES.md`.
2. Inspect root and nested worktree status; never assume prior evidence.
3. Confirm today's two focused patches apply after their documented
   prerequisites and match the live nested sources.
4. Start with physical signing/install and Gate 2. Do not repeat Simulator menu
   automation unless physical evidence exposes a concrete regression.
5. Keep the persistent goal active until physical Gates 2/3 and every audit
   row are actually proven.
