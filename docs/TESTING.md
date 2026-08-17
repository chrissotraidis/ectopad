# Testing

Last updated: 2026-08-17

### 2026-08-17 — first-door comparator trace and black-geometry physical rejection

- The iPad session `EctoPad-Diagnostic-20260817-103041.log` ran with
  `renderScale=2`, original 4:3 presentation, and retained the normal overlay.
  No Dawn validation error, Metal device loss, renderer assertion, GPU reset, or
  fatal event was recorded.
- The first visibly broken door is not an ordinary area-transition pair. UIDs
  410/409 are named `Door_Area Inside/Outside`, both remain in area 0 with dock
  1023, and are driven through source UID 412. Open logic and collision succeed.
  Both objects select the authored type-0 clip and reach zero remaining time;
  pose hashes advance through `411a4e88614f06fa`, `f9d432a50be20aee`,
  `be89cc8ae41e116a`, and `0e4290952e6a617d`; they are in-frustum and reach 30
  renderer submissions. The physical model nevertheless remains visibly
  closed.
- The next ordinary door (UIDs 204/827, areas 0/1, docks 401/819) records a
  direct projectile hit, uses the exact same character/model/skin/layout/open
  primitive and pose-hash sequence, and visibly works. Later pairs across areas
  1/2, 2/4, 4/6, 6/8, and 8/10 also work. This is controlled evidence against a
  generic `CScriptDoor`, missing-asset, animation-index, timing, or root-motion
  fix.
- The diagnostic workspace hash is not instance-local: identical doors obtain
  their `CSkinnedModel` from `CCharacterFactory`'s cache and therefore share its
  mutable workspace. A hash sampled later from `Think()` can reflect a different
  door. The next instrumentation must sample immediately after skinning and at
  the actual actor-keyed draw boundary.
- Four physical screenshots at 12:28:24, 12:29:13, 12:29:46, and 12:30:19 show
  selective black world geometry across consecutive Frigate rooms. HUD,
  arm-cannon, emissive lamps/screens, and door shields remain visible while
  individual world surfaces become black. The corresponding lifecycle resign
  notifications occur one second after each image, proving the defect predates
  screenshot/background transitions.
- Fog summaries remain valid through normal mask `0x1` and authored mode-2 mask
  `0x5`; no single global fog/exposure value explains the images. The exact area
  reached with macOS `--warp 1 6` renders normally through Dawn/Metal.
- The 2026-08-17 candidate that clears cached GX arrays when static area geometry
  is removed passed four focused `GXSetArray` tests, including deterministic
  same-address clear/rebind coverage. The iPhoneOS target linked, signed,
  installed in place, and launched, but the physical screenshots reject the
  candidate as a black-geometry fix. Aurora already clears array upload ranges
  at `end_frame`, so allocator-address reuse within one frame is not a sufficient
  explanation for the persistent multi-room failure.
- No source correction followed the final log/screenshot review. The same
  signed EctoPad 0.1.3 (1) device build was subsequently installed in place on
  the paired physical iPhone 14 and launched successfully as
  `com.axiodl.Metaforce`. The install did not uninstall the app, replace its
  data container, or copy an ISO. This proves build/sign/install/launch only;
  full root-cause and physical visual acceptance requirements remain recorded
  under KI-022/KI-023 in `TECH-DEBT.md`.

### 2026-08-14 — audio voice-iteration fix and dual-device update

- Two independent iPad crash reports, built from different app UUIDs, have the
  same main-thread fault: `SDLBackendVoiceAllocator::pumpAndMix + 656`,
  `EXC_BAD_ACCESS` at `0x18`, `x0 = 0`, and identical faulting instruction
  bytes. Disassembly maps the fault to reading the current voice after the
  `m_voices` iterator was invalidated by synchronous child-voice allocation.
- The targeted fix snapshots the voice-pointer registry once per 5 ms audio
  chunk. Recursive allocations join the next chunk; voice ownership and the
  existing end-of-cycle destruction path are unchanged.
- `cmake --preset ios-default` plus target `metaforce` completed successfully.
  The arm64 binary UUID is `CE8136C1-42AD-3A76-9479-E0C20B992225`; strict deep
  signature verification passed with the local Apple Development identity.
- The exact same signed EctoPad 0.1.3 (1) app was installed in place—without
  uninstall or container replacement—on the attached iPad Pro and iPhone 14.
  Both launched successfully as PIDs 2542 and 11711 respectively.
- The iPad pre-install backup contains the 1,459,978,240-byte ISO, both
  16,777,216-byte memory cards, and the preferences plist. SHA-256 values are
  `952972a0ddb122536d2f48c20d9e119278b13f848626afc72f034ce5a1022901`,
  `6856f47f86fed65605d35d5751147309991fd95dc1350b5092b1f0e81be72eb7`,
  `b12bde0a9d4dcbbca19363706c0de1eb8d6bd8a4f11387270c1468da39544418`,
  and `8d9d79b7abae1d810b171142bd1b5f3a12cfe76206d2ce656fff82170947a64f`.
- The iPhone's read-only CoreDevice container transfer timed out, so its
  preservation evidence is the non-destructive in-place install and successful
  launch, not a pre/post hash claim. Replaying the gameplay voice-spawn trigger
  on hardware remains the crash-fix acceptance gate.

### 2026-08-13 — controller session and HDMI crash diagnosis

- Pulled EctoPad's rotating logs and the iPad `systemCrashLogs` domain
  read-only; no app-container data was modified.
- One continuous controller play session ran from 09:30:34 to 10:04:43
  (34 minutes 9 seconds) before resigning active. It resumed at 10:20:47 and
  recovered/re-attached the overlay. The log recorded zero queue underruns,
  zero final-mix clipping, and no fatal, abort, jetsam, GPU, hang, or
  memory-warning marker during that session. The current diagnostics do not
  persist the physical controller's attach/name, so controller use is based on
  the user's hands-on report rather than a logged enumeration event.
- A later gameplay session ran 17:39:43–17:42:35. The first HDMI attempt then
  produced SIGABRT. Three immediate relaunches at 17:42:57, 17:42:59, and
  17:43:10 produced the same crash before engine initialization.
- All four crash reports fault in `FileStoreManager` from a second
  `aurora_main` call made by `SDLUIKitSceneDelegate postFinishLaunch`. The
  iOS-only secondary-scene guard builds successfully. Exact HDMI/controller
  physical replay remains open and is not replaced by compile evidence.

## Discipline

- Run only **one** iOS Simulator at a time.
- Close unrelated CPU/GPU-heavy apps before performance-sensitive runs and
  record process/system load; otherwise invalidate the performance comparison.
- Record dated, command-level evidence for every claim.
- Never classify a Simulator-only failure as a device blocker without testing on
  the device architecture.
- A title screen is not completion; a rendered room is not completion; Frigate
  Orpheon alone is not completion.

## Test matrix (tracked here as executed)

| Target | Launch | Import UI | Rendering | Touch | Controller | Gameplay | Saves | Audio | Lifecycle |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| macOS ARM64 | ☑ | — | ☑ | — | ◐ software path | ☑ | ☑ | ☑ | — |
| iPhone Simulator | ☑ | ☐ | ☑ shell/UI | ☐ | — | ☐ | ☐ | ☐ | ☐ |
| iPad Simulator | ☑ | ◐ harness/service | ☑ | ◐ UIKit targets | — | ☐ | ☑ preserved | ☑ | ☐ |
| Physical iPhone | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ |
| Physical iPad | ☑ launched | ◐ imported enough to play | ☑ title/Frigate reachable | ◐ movement fixed; L/R latch pending | ☐ | ◐ reachable, not accepted | ◐ A/B images present; save cycle pending | ◐ source fixes deployed; listening pending | ☐ |

## Gameplay verification scope (when reachable)

- Frigate Orpheon opening sequence (Gate 3): movement, camera, aiming, lock-on,
  Morph Ball, beam firing, missiles, jumping, scanning, menus, pause,
  save/reload, lifecycle recovery, memory headroom.
- Representative later areas: Chozo Ruins, Tallon Overworld, Phendrana Drifts,
  Magmoor Caverns, Phazon Mines.

## Evidence ledger

### 2026-08-13 — physical interaction and source-level audio correction

- User accepted left-stick movement and the native `•••` menu's render scale,
  aspect ratio, and FPS actions. Main-menu labels remained unreadable; dynamic
  option panes now explicitly use white text with black outlines.
- L/R short touches retain ordinary button behavior. A continuous half-second
  touch latches the shoulder blue with one haptic; the next tap releases it.
- The first source-level iPad trace showed a healthy 5,760-frame queue with
  zero underruns/clipping while frontend RSF alone reached peak 1.0. Its first
  jump counter compared adjacent interleaved L/R samples and overstated the
  result; the final deployed metric tracks each channel independently.
- Root cause: the RSF decoder emitted only sample pairs while the 32-to-48 kHz
  path requests alternating 682/683-frame chunks. Odd tails were dropped while
  the cursor advanced. The decoder now consumes one ADPCM nibble per requested
  sample. The first SoXR repair was later rejected by physical gameplay logs.
- Final source app compiled, passed `codesign --verify --deep --strict`, was
  installed in place, and launched. Early corrected intervals logged zero
  queue underruns/clipping. Listening in gameplay is still required; this is
  not audio acceptance.
- Post-install container inventory retained the 1.36 GB game image and both
  16,777,216-byte memory cards.
- Subsequent user testing rejected that build: menu labels remained dark and
  gameplay became silent. The log was conclusive: streamed DSP was nonzero,
  while Amuse stayed `0.000` with 193 voices marked running; once streaming
  ended, total output was zero.
- The next build restores direct Amuse pulls and applies a render-time white
  geometry override to only the affected menu panes. It compiled, passed strict
  signing verification, was installed in place, and launched. Physical
  visual/listening acceptance remains open.
- Physical follow-up accepted general playability enough to move through
  Frigate, run cutscenes, and use the in-game menu. The log confirms nonzero
  Amuse output with 4–10 active voices after streamed audio ends.
- The supplied unreadable screen was identified as `SNewFileSelectFrame`, not
  the previously edited `SFrontEndFrame`. The latest build targets all actual
  save-slot/root/popup text pairs in that frame at render time.
- Viewing a screenshot and returning removed all touch controls and `•••`.
  The latest build logs UIKit lifecycle transitions and reattaches the overlay
  to the current root-controller view on foreground and periodic host drift.
- The first beam door failed to open. Door behavior remains unchanged; the
  latest build logs projectile intersection, Open request, load wait, and open
  animation so one reproduction can identify the cause.
- The 10:02 screenshot exposed a regression from the render-time contrast
  override: both panes of each authored text pair were equally bright and the
  table geometry no longer conveyed selection. This produced offset double
  labels and no visible cursor. The correction removes the global geometry
  override and suppresses pane B's offset pass in `FRME_NewFileSelect`, while
  pane A retains the original table selection behavior.
- The same session reported a brief audio blurb at the menu-to-cutscene handoff.
  The persisted trace recorded `amuse=2.129` and 502 clipped output samples at
  that transition. The deployed build smooth-limits only Amuse samples above
  a 0.8 knee, capped at 0.95; lower-level samples are untouched.
- The corrected source app compiled, passed strict signing verification, was
  installed in place, and launched at 10:09. Post-install readback retained the
  exact 1,459,978,240-byte game image and both 16,777,216-byte memory cards.
  Visual selection and transition listening remain physical acceptance gates.
- The extended Frigate run showed that the first tutorial door did receive
  projectile hits and later executed `Open`/`door opening` for both connected
  areas. The user then observed it open on approach. No global door behavior was
  changed.
- The same run logged repeated final-mix peaks of 1.2–1.6 and hundreds of
  clipped samples even though the Amuse source had already been capped. The
  limiter now runs after Amuse and DSP stream summation.
- The 10:22 screenshot proves a world-rendering failure: HUD and nearby
  geometry survive while distant/room geometry becomes black. The current build
  persists materially changed fog mode/range/color and projection near/far for
  correlation with the next screenshot. It does not import Aurora's large,
  newer FIFO/fog refactor without a controlled renderer validation.
- The file-menu correction no longer recolors both authored panes. It suppresses
  pane B's offset duplicate in `FRME_NewFileSelect`, keeps pane A's original text
  and animation, and restores the original table selection colors. L/R latch
  time is reduced from 1.0 to 0.5 seconds.
- The 11:04 app compiled, passed strict signing verification, was installed in
  place, and launched. Readback retained the exact 1,459,978,240-byte game image
  and both 16,777,216-byte cards.
- Native controller plumbing exists through SDL's GameController path and the
  current GameCube mapping/mixer, including hot-plug, both sticks, analog
  triggers, D-pad, rumble, and the native mapping panel. No physical controller
  was connected during this pass, so behavior on this iPad remains unaccepted.
- The host screen-history recorder could not be verified during this session.
  The supplied screenshots remain the visual evidence. A future diagnostic
  export needs an engine/framebuffer snapshot proven to include the Metal game
  surface; text state alone is insufficient for intermittent visual faults.

### 2026-08-12 — physical Gate 2 launch, Gate 3 failed

- Earlier the same day a scoped pass found zero identities and no device and
  correctly did not start. That snapshot is historical only
  (`/tmp/ectopad-gate2-prereq-blocked-2026-08-12.md`).
- A later session development-signed the source
  `ref/metaforce/build/ios-default/Binaries/Metaforce.app`, installed it in
  place, and launched `com.axiodl.Metaforce` on the attached iPad.
- User hands-on: the game technically runs and Frigate is reachable. Gate 3
  failed. Default slots A and B are empty so the game cannot save. Audio has
  no music and only some SFX. The `•••` menu looks unfinished and native
  1×–4× options are not present. The C-stick appears to do nothing. There are
  unspecified texture issues. The deploy/README path was not a front door.
- This is not a Simulator result and not a PID-only claim. It is also not
  acceptance. See [TECH-DEBT.md](TECH-DEBT.md), KI-003, KI-016, KI-017, KI-018.

### 2026-08-12 — current SunPad interaction/menu and audio reserve

- Added an opt-in current-build harness in Metaforce-specific
  `OverlayBridge.mm`; no SunPad source file changed. On a clean iPad Pro
  13-inch (M5) Simulator container with no ISO/GCM it found the recursive
  19-title menu inventory and all 14 controls; sent A down/up through the real
  `SunPadInputMixer`; exercised render scale, opacity, global size,
  hide-on-controller, and modern C-stick; selected/resized A to 1.25× in the
  real editor; reset; presented the empty-folder alert and real Files picker
  with `TouchOverlayDelegate`; restored defaults; and returned `result=0`.
- The live bridge logged `menu visible=1 attached=1 frame={{1324, 12}, {40, 40}}`.
  Aurora now reasserts the window-level SunPad overlay z-order once per second
  after presented frames so later SDL Metal-view changes cannot cover `•••`.
  Evidence: `/tmp/ipad-menu-audio-focused-2026-08-12.png` and `.log`.
- Audited SunPad's resolved Super Mario Sunshine audio work. Its 12× guest-
  timebase defect is architecture-specific and does not apply to native
  Metaforce, but its bounded-reserve/no-stale-replay lesson does. Metaforce's
  old pump generated one 60 Hz slice per rendered frame, so render dips could
  starve SDL. The new pump measures output-ready audio after SDL conversion,
  tops to 120 ms with fresh samples, and counts empty-queue recovery.
- Final iOS Simulator, physical iPhoneOS arm64, and macOS ARM64 targets compiled
  and linked. A native `--warp 2 2` run opened a 44.1 kHz device, held
  5,292–5,294 output frames against the 5,292-frame target across repeated
  samples, mixed 6–7 active gameplay voices/3 submixes, and logged no underrun.
  Competing Codex renderer load was 35–61%, so this is correctness/stress
  evidence, not a performance baseline. Audible physical-device acceptance is
  still required. Log: `/tmp/metaforce-audio-reserve-native-2026-08-12.log`.

### 2026-08-11 — macOS ARM64 (Gate 1)

- Build: `cmake --preset macos-default-relwithdebinfo` +
  `cmake --build --preset macos-default-relwithdebinfo` → 862/862 steps OK,
  `Binaries/Metaforce.app` (arm64 Mach-O, Metal.framework weak-linked, 25 MB).
- Boot (no dev console): `Metaforce -l <iso>` — Metal selected, 60.1 FPS,
  disc identified, assets loaded, no errors.

### 2026-08-11 — iPad Pro 13-inch (M5) Simulator, audio stack

- Rebuilt `build/ios-sim` with the vendored audio stack (amuse/athena/logvisor/soxr):
  clean configure + build (2522 steps) + install; app linked for the sim SDK.
- Launched `com.axiodl.Metaforce` with the user ISO staged in the app container:
  Metal adapter "Apple iOS simulator GPU", amuse engine initialized (SDL backend,
  32 kHz), audio groups loaded into amuse, frontend RSF music
  (`frontend_1.rsf`/`frontend_2.rsf`) streaming, `SDLBackend: 2 voices (2 running),
  3 submixes`, 534-frame audio pump steady, no underruns.
- Screenshot: full-frame Metroid Prime title screen (gold logo, [PRESS START],
  4:3). Evidence: log `/tmp/mf_ios_sim_audio.log`; screenshots
  `/tmp/ios_audio_check2.png`, `/tmp/ios_audio_final.png`.

### 2026-08-11 — macOS save/reload cycle (KI-011 fix)

- New game from the file select created a save slot (`GM8E01` / `MetroidPrime B`)
  and persisted it to the raw Dolphin card
  (`~/Library/Application Support/dolphin-emu/GC/MemoryCardA.USA.raw`, directory
  entry at offset 0x2040) — previously the card was never written because
  kabufuda indexed the AIO queue with block numbers beyond the 5-slot queue
  (`Card::open` → `resizeQueue(5)`) and `AsyncIO::~AsyncIO` cancelled pending
  aio writes.
- Clean SIGINT exit → relaunch → file select showed the save → pressing A
  loaded the saved game (intro narration card → cinematic at 60 FPS, 10+ audio
  voices). Evidence: screenshots `/tmp/mf_reload_a2.png`,
  `/tmp/mf_reload_game.png`; logs `/tmp/mf_savefix.log`, `/tmp/mf_reload.log`.

### 2026-08-11 — iPad Pro 13-inch (M5) Simulator, in-game + save (Gate 2 de-risk)

- Rebuilt `build/ios-sim` with the audio stack, CInputStream byte-order fix
  (KI-009), and kabufuda card-persistence fix (KI-011); installed and launched.
- `--warp 2 2`: rendered **Tallon Overworld in-game** (rocky ruins, arm cannon,
  visor HUD with ENERGY 99, radar, minimap) through Dawn→Metal on the simulator.
- `--autostart` (new test hook): drove title → file select → new game → intro
  cinematic → **first-person Frigate Orpheon gameplay** (ENERGY 99, missiles 15,
  radar, HUD icons) at 60 FPS with no host input, and persisted the
  `GM8E01`/`MetroidPrime B` save to the sim container's card
  (`…/Library/Application Support/dolphin-emu/GC/MemoryCardA.USA.raw`, entry at
  offset 0x2040). Host keyboard forwarding to the Simulator GUI was not
  available, which motivated the hook. Evidence: `/tmp/ios_warp1.png`,
  `/tmp/ios_auto1.png`, `/tmp/ios_auto2.png`, `/tmp/ios_auto3.png`.

### 2026-08-12 — iOS Simulator touch input (foundation)

- Added SDL finger-event capture in aurora mapped to a virtual GameCube pad
  (left half = movement stick, right half = C-stick, bottom corners = A/B taps),
  fed through the existing `PADSetVirtualStatus` mechanism
  (superseded by the full touch overlay below).
- Verified on the iPad Pro 13-inch (M5) Simulator: with the game in Frigate
  Orpheon gameplay (via `--autostart`), a Simulator-GUI mouse drag on the left
  half of the screen (which the Simulator delivers as a touch) **moved Samus
  forward** — the view advanced past the doorway and the game's lock-on
  tutorial prompt ("Press and hold [L] to lock onto targets") appeared.
  Evidence: `/tmp/ios_touch1.png`, `/tmp/ios_touch_after.png`,

### 2026-08-12 — SunPad UIKit overlay port and KI-015

- The independent ImGui overlay was rejected and superseded. The current UI is
  a direct port of `SunPadGameOverlay`, `SunPadSettings`, `SunPadInputMixer`,
  `SunPadInputState`, and `SunPadDiagnostics`; see
  [SUNPAD_PARITY.md](SUNPAD_PARITY.md) for the file-by-file audit.
- Root-caused KI-015 without changing SunPad UI code. Both
  `ios_touch_stub.cpp` and `OverlayBridge.mm` defined
  `aurora::touch::attach_ios_overlay()` in `libaurora_core.a`. The stub's SDL
  platform macros were unavailable, so it emitted an iOS symbol; archive order
  caused the linker to select it and omit the bridge. CMake now includes the
  stub only for non-iOS targets.
- Rebuilt `ios-sim` successfully (2529 targets after CMake regenerated Dawn and
  SDL), installed `build/ios-sim/Binaries/Metaforce.app`, and launched with
  `--autostart` on the one booted iPad Pro 13-inch (M5) Simulator. Evidence:
  `[SunPad] session start`, Dawn adapter `Apple iOS simulator GPU`, unchanged
  SunPad controls over the live intro/game, audio active, and clean
  `simctl terminate` exit 0. Screenshot/log:
  `/tmp/ki015-overlay-fixed-2026-08-12.png`,
  `/tmp/ki015-overlay-fixed-2026-08-12.log`.
- The current Simulator host still did not forward device-area clicks to the
  app, so this run could not reopen the menu/settings. Earlier evidence
  `/tmp/ios_menu_open2.png` proves the directly ported menu rendered before the
  linker regression. At that timestamp current-build menu interaction remained
  unverified rather than inferred; the later final harness entry supersedes
  this limitation with deterministic `result=0` wiring evidence.
- Rebuilt `macos-default-relwithdebinfo`; the app reached Dawn/WebGPU → Metal
  on Apple M2 and exited 0 after SIGINT.

### 2026-08-12 — macOS virtual GameController path

- Enabled the opt-in Aurora test hook with `AURORA_VIRTUAL_GAMEPAD=1`. It
  attaches an SDL3 standard virtual joystick and reads axis/button commands
  from `/tmp/aurora_vg_cmds`; no keyboard, mouse, touch, `--autostart`, or
  Metaforce-specific input injection participates after launch.
- Fixed two defects found before accepting the test: SDL3 joystick ID `0` is
  invalid (the unsigned ID had incorrectly used `-1` and signed comparisons),
  and `SDL_VirtualJoystickDesc` must use `SDL_INIT_INTERFACE` rather than a
  hard-coded version. The running app then logged both the virtual attachment
  and normal controller addition.
- With the validated GM8E01 Rev 2 image loaded, SDL Start (`button 6`) advanced
  `[PRESS START]` to the game's main menu; left-stick Y (`axis 1`) visibly
  displaced the live Aurora controller overlay; SDL A (`button 0`) selected
  the menu item and began the game intro. The normal window-close path returned
  without the FIFO-reader teardown hanging.
- Result: the software SDL event → Aurora controller assignment → `PADRead` →
  Metroid frontend path is proven. Physical Apple GCController discovery,
  hot-plug/reconnect, rumble, and touch/controller handoff are still untested.
  Evidence: `/tmp/virtual-gamepad-a-select-2026-08-12.png`; mirrored patch:
  `patches/2026-08-12-aurora-virtual-gamepad-test-hook.patch`.
- Rebuilt both final-source gates: `macos-default-relwithdebinfo` completed,
  and the recovered `ios-sim` Ninja journal rebuilt and linked all 2,527
  targets successfully.

### 2026-08-12 — SunPad display menu engine wiring

- Kept all audited SunPad UI/settings/mixer/input files unchanged and added the
  Metaforce-specific behavior only in `OverlayBridge.mm` and generic Aurora
  framebuffer APIs.
- Applied persisted SunPad settings before launch to test the exact menu action
  path without relying on the currently broken Simulator host-touch delivery.
  At 2× + Original 4:3 the app logged `renderScale=2 aspectMode=0` and Aurora
  created a `1280x960` framebuffer. At 2× + experimental 16:9 it logged
  `renderScale=2 aspectMode=1` and created a `1707x960` framebuffer. Both runs
  reached Dawn/WebGPU → Metal and live Metroid rendering with the unchanged
  SunPad touch overlay.
- Enabled SunPad's FPS setting and verified its label is driven by successful
  surface presents. Evidence:
  `/tmp/sunpad-menu-wiring-2x-fps-2026-08-12.png` and
  `/tmp/sunpad-menu-wiring-16x9-2026-08-12.png`.
- The visible 20.4 FPS sample was captured while another heavy app was running;
  the later image showed 59.9 after that app closed. These samples prove the
  counter changes with presented-frame rate, but neither is accepted as a
  benchmark or physical-device result. Future performance runs must follow
  [PERFORMANCE.md](PERFORMANCE.md).
- Final Aurora sources compiled and linked for iOS Simulator; the corresponding
  macOS preset also rebuilt. The full seven-patch Aurora sequence applies from
  clean pin `5143394`, passes `git diff --check`, and is byte-identical to the
  live nested source. Patch:
  `patches/2026-08-12-aurora-wire-sunpad-display-settings.patch`.

### 2026-08-12 — private atomic iOS game-data import

- Wired the unchanged SunPad Game Data & Saves actions to a Files picker,
  Files-visible Documents-folder import, and save-preserving removal. The
  importer validates exact raw-disc size, `GM8E01`, disc 0, revision 2, and
  GameCube magic before copying; it then validates canonical SHA-1 on a
  mode-0600 same-directory staging file before atomic `rename(2)` activation.
- Invalid 4 KiB input was rejected without changing the active image or save.
  A valid import changed the active inode and produced the canonical SHA-1; a
  normal `--autostart` restart loaded Metroid Prime USA Build v1.111 through
  Dawn/WebGPU → Metal. Removal deleted only `game.iso`, preserved the save, and
  valid reimport restored the image.
- A deliberately corrupted full-size clone passed header/size checks but was
  rejected by staged SHA-1. The prior active inode and canonical hash stayed
  intact, the save SHA-256 stayed
  `107e0d4a0494f6905d47c6bcf85b0d68ba7b2160d37ed4596564f2b10b322f5d`,
  and no staging file remained. Runtime log:
  `/tmp/metaforce-import-atomic-2026-08-12.log`.
- Final sources compile/link for iOS Simulator, macOS ARM64, and physical arm64
  iOS. The device artifact is a Mach-O arm64 `Metaforce.app`; configuration was
  hardened so cross-compiles vendor static zstd rather than link Homebrew's
  macOS dylib. Production Files-picker tapping is still unverified because host
  Simulator touch delivery is broken and no signed physical device is present.

### 2026-08-12 — holistic audit and SunPad controller mapping

- Added [COMPLETION_AUDIT.md](COMPLETION_AUDIT.md), checking every goal block
  against current evidence. The goal remains active: physical iPhone/iPad Gates
  2/3, current touch/UI interaction, diagnostics sharing, gameplay breadth,
  controlled performance/memory, and packaging remain incomplete.
- Ported `SunPadControllerMapping.{h,mm}` byte-identically. The unchanged
  SunPad menu now presents the reference five-button mapping UI; Aurora applies
  its persisted A/B/X/Y/right-shoulder permutation only to the iOS SDL gamepad
  path, leaving sticks, triggers, D-pad, Start, L, touch, and desktop unchanged.
- Simulator test returned `result=0` after verifying default mapping, persisted
  A/B swap in both directions, and Start/L passthrough, then restored the user's
  prior preference. A deterministic UI launch displayed all five default rows
  over the unchanged controls. Evidence:
  `/tmp/ios-controller-mapping-ui-2026-08-12.png` and
  `/tmp/ios-controller-mapping-2026-08-12.log`.
- The full SDL virtual-controller → Aurora assignment → `PADRead` mapping path
  was also exercised with an opt-in test-only A/B swap. A raw physical east/B
  bit (`physical=0x0200`) emerged as GameCube A (`buttons=0x0100`) and returned
  to zero on release. Evidence was captured from the live console; the guarded
  test assignment/logging is inert unless explicitly enabled.
- macOS ARM64, iOS Simulator, and physical arm64 iOS builds all completed. The
  Simulator was shut down afterward. Physical behavior is not claimed: this Mac
  currently has zero code-signing identities and `devicectl` sees no device.

### 2026-08-12 — iPhone diagnostics presentation and privacy

- Installed the current universal Simulator app on an iPhone 17 Pro Simulator
  and used test-only entry points into the unchanged SunPad overlay. Landscape
  controls, the Share Diagnostic Log confirmation, and the real UIKit activity
  sheet rendered without clipping. Evidence:
  `/tmp/iphone-diagnostics-confirmation-final-landscape-2026-08-12.png` and
  `/tmp/iphone-diagnostics-share-landscape-2026-08-12.png`.
- The shared item is a text document named `Metaforce-Diagnostic-<UTC>.log`.
  A service-level test wrote the current app home and temporary paths through
  `SunPadLog`, snapshotted the persistent log, and returned `result=0` only after
  confirming both raw paths were absent and the `<app-container>` and
  `<temporary>/` markers were present. Log:
  `/tmp/iphone-diagnostics-2026-08-12.log`.
- This is Simulator presentation/privacy evidence, not proof of tapping the menu
  or completing an export on physical hardware. The iPhone Simulator was shut
  down afterward; only one Simulator was active.

### 2026-08-12 — mobile desktop-chrome parity regression

- iPhone evidence exposed desktop ImGui chrome beneath SunPad's controls: main
  menu bar, pre-launch About UI, controller toast, and optional debug overlays.
  This violated the requirement that SunPad own everything overlaid on mobile.
- `ImGuiConsole` now retains listener/state maintenance on iOS but returns before
  all desktop presentation in `PreUpdate` and `PostDraw`. The iPhone 17 Pro and
  iPad Pro 13-inch Simulators then rendered only the unchanged SunPad controls
  over the engine/game surface. macOS still compiles the original ImGui path.
- Evidence: `/tmp/iphone-sunpad-clean-mobile-ui-landscape-2026-08-12.png` and
  `/tmp/ipad-sunpad-clean-mobile-ui-2026-08-12.png`. Both Simulators were shut
  down; the iPhone and iPad runs were sequential.

### 2026-08-12 — deterministic unsigned iOS packaging audit

- Fixed the no-tag checkout fallback so the Apple marketing/build versions are
  valid `0.0.1` and `1`, exposed validated cache overrides for real release
  versions, and rebuilt the full arm64 iPhoneOS app successfully.
- Remapped Metaforce C/C++ checkout/build roots and nod/Rust Cargo roots at
  compile time. The rebuilt executable contains no current-user checkout or
  Cargo-home paths. The prebuilt Dawn archive still contributes 241 unique
  upstream `/Users/runner/work/dawn-build/...` diagnostic paths; the audit
  reports these separately and rejects every other `/Users/` or `/Volumes/`
  path.
- Ported SunPad's deterministic `Payload/*.app` packaging/audit discipline.
  The staged copy is unsigned, stripped, xattr-free in ZIP metadata, timestamp
  normalized, license/notices complete, arm64-only, iPhoneOS-targeted, free of
  host dylinks, and scanned for disc headers, saves, logs, credentials, local
  paths, and signing material.
- Two independent packages were byte-for-byte identical at SHA-256
  `1348c52c9cbc8f3882f8f4a131fb6e0dc34c901d8ac91367c3c334d76c554835`.
  A negative archive seeded with `leaked-save.gci` was rejected. Evidence:
  `/tmp/Metaforce-unsigned-validation-final-2026-08-12.ipa` and
  `/tmp/metaforce-negative-audit.log`.
- This proves local unsigned validation packaging only. Public redistribution
  remains blocked on GPL/LGPL release materials and scrubbed Dawn provenance.
  A later same-day session development-signed the source `.app` and launched it
  on the attached iPad; that launch is not Gate 3 acceptance. See
  [TECH-DEBT.md](TECH-DEBT.md).

### 2026-08-12 — final menu/audio package superseding prior package hash

- After the final menu-visibility and frame-rate-independent audio patches, the
  device app was rebuilt and packaged twice. Both archives were identical.
- Exact artifact: `/tmp/Metaforce-unsigned-menu-audio-2026-08-12.ipa`.
- SHA-256:
  `308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`.
- The same structure/platform/linkage/privacy/prohibited-content audit passed.
  This supersedes the earlier `1348c52c…` package as the final local validation
  artifact; it remains unsigned, uninstalled, and non-public.

### 2026-08-11 — macOS rendering and keyboard fixes

- In-game: `Metaforce -l --warp 2 2 +debugOverlay.* <iso>` — full-screen scene +
  HUD, 1760 draw calls, 60 FPS; then ImGui segfault (KI-002).
- Artifacts: logs `/tmp/metaforce-run.log`, `/tmp/metaforce-fe.log`,
  `/tmp/metaforce-warp.log`; crash `~/Library/Logs/DiagnosticReports/
  Metaforce-2026-08-11-152126.ips`; screenshots `/tmp/metaforce-*.png`.
- Frontend investigation: frontend state machine verified (OpenCredits 10 s →
  Title 30 s → Attract cycle; per-second state logs), THP YUV decode verified
  correct by dumping decoded frames; movie rendering defect characterized as
  KI-001 (small garbled corner strip). See [KNOWN_ISSUES.md](KNOWN_ISSUES.md).
- ImGui shutdown crash (KI-002/005) root-caused from the user's crash report
  (`Metaforce-2026-08-11-165907.ips`): shutdown order bug in `aurora::shutdown`.
  Fix applied (gfx drain before imgui teardown + render guard); two consecutive
  quits verified exit 0 with no crash report.
- **KI-001 fixed:** frontend movies were clipped to a 640×480 region because the
  per-frame `BeginScene` never set the scissor (aurora refresh used the stale
  logical 640×480 scissor). Fix: full-frame scissor per frame + logical viewport
  sync + `GX_CULL_NONE` for the movie quad. Verified: title screen renders
  full-frame (gold "METROID PRIME" logo + emblem, 60 FPS), attract footage
  full-frame, in-game warp renders correctly (1,752 draw calls, HUD).
- **Keyboard input enabled (local patch):** implemented the kbm path upstream
  disabled. Verified end-to-end: holding Enter at the title screen advances the
  frontend to the save/memory-card dialog ("No Memory Card found in Slot A");
  Down arrow and S move the selection between "Continue Without Saving" and
  "Retry"; input overlay reflects key state; `config.yaml` written to the store
  root (configuration persistence). Patch:
  `patches/2026-08-11-metaforce-enable-keyboard-mouse-input.patch`.
- **iOS Simulator (iPad Pro 13-inch M5, iOS 26.5):** built the app for
  `SIMULATORARM64` after fixing Dawn cross-compile obstacles (built a host
  protoc from the vendored protobuf 33 for the codegen tool; disabled GLFW which
  is macOS-only). Installed via `simctl`, copied the user's ISO to the app
  container as `game.iso`, launched, and the app rendered the **Metroid Prime
  title screen** (gold "METROID PRIME" logo + emblem + [PRESS START], 4:3
  letterboxed). Live cycle confirmed; one simulator used, then shut down.

### 2026-08-13 — physical menu acceptance and final narrow correction pass

- The user physically accepted the `FRME_NewFileSelect` readability fix. The
  accepted solution keeps pane A with a white font and hides pane B's authored
  offset pass only for file/root/popup choices. Original strings, typewriter
  animation, geometry, and table colors remain. Earlier global and dual-pane
  recoloring attempts are rejected.
- A live trace plus hands-on walk-through proved the first tutorial door reaches
  Open and becomes non-solid while its static model remains visible. A later
  renderer-suppression workaround made the door disappear abruptly and was
  rejected; the original model/animation path is restored.
- A bounded 320-frame/10 ms source-boundary audio ramp did not remove the
  frontend-to-asteroid/planet aberration and was subsequently removed.
- Distance-dependent enemy pose and black/distant geometry remain open. No
  global actor-animation, LOD, or broad Aurora FIFO change was made without a
  reproducible visual capture.
- The `ios-default` target compiled and linked successfully. Signing, in-place
  install, post-install card/ISO readback, and launch succeeded at 11:30.
  `codesign --verify --deep --strict` passed; installed identity is EctoPad
  `com.axiodl.Metaforce` 0.1.3 (1). The ISO remained 1,459,978,240 bytes with
  SHA-256 `952972a0ddb122536d2f48c20d9e119278b13f848626afc72f034ce5a1022901`;
  Slot A and B each remained 16,777,216 bytes. This preservation/deploy proof
  does not substitute for physical listening or visual acceptance.

To be appended with dated entries for later phases.

### 2026-08-13 — later-game file slot and renderer research pass

- Pulled the post-play iPad diagnostic log. The audio output queue remained at
  its 5,760-frame target with zero underruns and zero post-mix clipping. Amuse
  nevertheless peaked at 2.129 during the frontend/game boundary and later
  emitted >1.0 peaks with hundreds of per-channel jumps. The rejected 10 ms
  transition ramp was removed; source/mixer behavior remains the audio target.
- Reverted the manual open-door render suppression after hardware showed an
  abrupt disappearance rather than the authored animation.
- Downloaded and validated the public RetroMaggedon mid-game save identified as
  Metroid Prime USA Rev 2. Archive SHA-256:
  `5d86dc42e57082c100bc29ffb3caac668142d59174bc607327b6152b7ee27b2d`;
  GCI SHA-256:
  `ad3bc29ebe1a4c0c06321ee93554943519529f4582c10675481295ff2dd959df`.
- Merged only its first 940-byte state into Prime internal file slot 2 on a
  cleanly terminated, freshly pulled card. The tool validated both Prime CRCs,
  reopened the complete card, proved internal file slot 1 byte-identical, and
  refused a second overwrite. Device readback exactly matched merged SHA-256
  `ee2f1a892801168c4226b79988d489bae5087e5fd058054509789f53e51c9bb7`.
- Compared pinned Aurora 5143394 with current official main. Four relevant
  fixes applied cleanly, but the combined physical build failed its first
  later-game stability gate during Morph Ball. The set was rolled back to the
  pinned renderer baseline; the later FIFO refactor remains out of scope.
- The compatibility build was signed, verified, installed in place, and
  launched on the physical iPad at 12:27. Post-install Slot A readback remained
  byte-identical at SHA-256
  `ee2f1a892801168c4226b79988d489bae5087e5fd058054509789f53e51c9bb7`,
  proving that both the user's original file slot 1 and injected mid-game file
  slot 2 survived the install. This is deployment evidence, not visual or audio
  acceptance.
- Physical file-slot-2 testing failed immediately after entering Morph Ball and
  moving toward a tunnel. The launched PID was 1812; afterward EctoPad was
  resident as PID 1826. Two pulls of `systemCrashLogs` contained no EctoPad
  crash, jetsam, GPU, or hang report. The original session log ended at
  10:49:43Z without a graceful shutdown or lifecycle notification. A later
  five-second Time Profiler capture of PID 1826 showed suspended UIKit work and
  nominal thermal state. The copied card also loads on macOS, ruling out basic
  card/CRC corruption, although its active-combat checkpoint is not a
  deterministic Morph Ball fixture. Evidence:
  `/tmp/ectopad-morph-crash.GNrkCN/runtime.log` and
  `/tmp/ectopad-morph-crash.GNrkCN/Metaforce-live.trace`.
- The user-supplied translated crash report was inspected and excluded: it is
  the local macOS process launched by Codex at 13:18 (Mac15,9, PID 23027,
  coalition `com.openai.codex`) and aborted in AppKit registration before game
  initialization. It is not an iPad or Morph Ball crash report.
- The four Aurora compatibility changes were removed source-by-source and the
  pinned renderer baseline rebuilt successfully. Low-frequency runtime events
  now write synchronously, and every Morph Ball state transition records its
  prior/next state, area, and position. The rollback build was signed, strictly
  verified, installed in place, and launched at 14:10. Post-install Slot A
  readback remained byte-identical at SHA-256
  `ee2f1a892801168c4226b79988d489bae5087e5fd058054509789f53e51c9bb7`.
  Morph Ball remains a physical acceptance gate; deployment is not a fix claim.

### 2026-08-13 — final isolated renderer build and device handoff

- QuickTime capture reproduced the black-room geometry and the first door's
  visible-closed model in the same run. The video is
  `/tmp/ectopad-door-black-2026-08-13.mov`; the correlated runtime log is
  `/tmp/ectopad-live-bug-log.MaC1Hw/runtime.log`.
- An iPad build containing only Aurora `b684c0d` was physically rejected: both
  symptoms remained. That source change was removed.
- Door-specific instrumentation then proved Open is requested, animation type
  0 is selected, its authored duration is 0.833333 seconds, and the model
  reports `animating=true`. This rules out absent door logic or missing clip
  data; model pose/presentation remains open.
- The final build contains only upstream Aurora `2bbb122`, correcting
  `GXSetChanCtrl` diffuse-function encoding for specular attenuation. The
  targeted `GXFifoTest.ChanCtrl_Color1_SpecularLighting` test passed, followed
  by the complete Aurora GX test binary: 165/165 passed.
- The iPhoneOS app compiled, was development-signed, passed
  `codesign --verify --deep --strict`, installed in place, and launched at
  15:30 as PID 1940. No recording or additional hands-on game run was requested.
- EctoPad was cleanly terminated before backup. Pre-install and post-install
  copies of `game.iso`, `MemoryCardA.USA.raw`, and `MemoryCardB.USA.raw` are
  byte-identical. SHA-256 values are respectively
  `952972a0ddb122536d2f48c20d9e119278b13f848626afc72f034ce5a1022901`,
  `ee2f1a892801168c4226b79988d489bae5087e5fd058054509789f53e51c9bb7`,
  and `b12bde0a9d4dcbbca19363706c0de1eb8d6bd8a4f11387270c1468da39544418`.
  Deployment and preservation are proven; the black geometry, door animation,
  transition audio, and later-game stability remain physical acceptance gates.

### 2026-08-13 — HDMI guard, original icon, and final in-place update

- The exact iPhoneOS target rebuilt successfully with the iOS-only
  `EctoPadSceneGuard.mm`. The arm64 executable contains the persistent
  `external display scene connected; retained existing engine instance`
  breadcrumb, and the linked SDL archive exposes the guarded
  `SDLUIKitSceneDelegate postFinishLaunch` selector. Compile/link evidence does
  not replace reconnecting HDMI on the physical iPad.
- The original green ectoplasm icon was generated as project artwork, converted
  to an opaque sRGB 1024×1024 master, and compiled with Xcode 26.6 `actool`.
  `assetutil` reports opaque `AppIcon` renditions, and the produced app's
  `Assets.car` SHA-256 exactly matches the synced Metaforce resource:
  `4037ae7ebacbed882bf9b0af8ab5e60da6d68af409daa4465ac4619801c65f46`.
- The unsigned validation package passed the repository audit with SHA-256
  `5d38f6d837c501e3a24c347e864033ec58aaad9f61638f871dc32ae2f6c8d28e`.
  It remains a local, non-public validation artifact.
- The device app was development-signed, passed
  `codesign --verify --deep --strict`, installed in place, and launched at
  20:21 as PID 2122. Bundle metadata remains EctoPad
  `com.axiodl.Metaforce` 0.1.3 (1).
- Pre/post install copies of the ISO and both card images are byte-identical.
  SHA-256 values remain
  `952972a0ddb122536d2f48c20d9e119278b13f848626afc72f034ce5a1022901`,
  `ee2f1a892801168c4226b79988d489bae5087e5fd058054509789f53e51c9bb7`,
  and `b12bde0a9d4dcbbca19363706c0de1eb8d6bd8a4f11387270c1468da39544418`.

### 2026-08-16 — SDL3 controller reconciliation

- Backend ownership was verified before changing code: Aurora/Metaforce uses
  SDL3 gamepads for runtime input. The native Apple GameController framework is
  consulted by the iOS mapping UI, but it does not own gameplay slots.
- The focused `controller_slots_tests` binary passed 5/5 cases: missed removal
  plus sole-controller reclaim, two-controller preservation, preservation of a
  remaining valid player, next-free assignment for an additional controller,
  and foreground reconciliation.
- The complete Aurora CTest set passed 180/180. The full macOS
  `macos-default-relwithdebinfo` build linked the app, and the iPhoneOS
  `ios-default` `metaforce` target linked successfully.
- Merge commit `88adf04` was pushed to `main`. The iPhoneOS app was rebuilt,
  development-signed under the existing `com.axiodl.Metaforce` identity,
  strictly verified, and installed in place on the iPad without uninstalling
  the prior app or requesting container-content removal. It launched as PID
  4588. A device screenshot at 20:44 shows the Metroid Prime title screen and
  touch overlay, proving that the installed build can still open the existing
  ISO and reach the game frontend. Evidence:
  `/tmp/ectopad-controller-deploy.gVhYpu/post/EctoPad-after.png` (SHA-256
  `e2eae1cd0430f75441d95e61cfbc289270f9581cefdf1b14537c3d143c2cecd5`).
- Current pre/post byte readback of preferences and memory-card files was not
  available: both CoreDevice CLI copies and Xcode's app-container download
  stalled. A prior complete container backup remains available as recovery
  material, but it is not proof that today's save bytes are unchanged. No
  uninstall, container replacement, preference reset, or ISO copy was used.
  Physical Bluetooth, wired, and natural-sleep disconnect/reconnect remain
  acceptance gates, including held-input release, foreground recovery,
  two-controller behavior, rumble, and touch/controller handoff on real
  accessories.
