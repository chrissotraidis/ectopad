# EctoPad technical debt

Updated 2026-08-13 after continued physical-iPad correction passes. Build, signing,
install, PID, and container readback are evidence, not hands-on acceptance.
Default A/B card formatting is proven at the container level. The native menu,
render scale, aspect ratio, FPS toggle, left-stick movement, and corrected
single-pass file menu are now accepted on hardware. Audio is source-instrumented and two concrete lifecycle/decoder
bugs are repaired, but gameplay listening remains open. Prime's original
controls are restored and L/R now offer an optional half-second touch latch.
Named texture defects remain open.

## Latest deploy evidence (2026-08-13, 15:30)

The final rebuild for the day was signed and installed in place on the attached
iPad Pro (iPad14,5). This is deploy proof only.

- source `.app`: `ref/metaforce/build/ios-default/Binaries/Metaforce.app`
- identity: local Apple Development identity (not recorded)
- `codesign --verify --deep --strict` passed
- EctoPad was terminated cleanly before an explicit pre-install backup at
  `/tmp/ectopad-final-preinstall.2cyWLs`. No `--remove-existing-content`.
- latest in-place install: `com.axiodl.Metaforce`
- installed app name is **EctoPad**; bundle ID remains
  `com.axiodl.Metaforce` to preserve the existing container
- launched with `--terminate-existing`; live PID was 1940
- pre/post game-image SHA-256 matched:
  `952972a0ddb122536d2f48c20d9e119278b13f848626afc72f034ce5a1022901`
- Slot A/B files now exist at
  `Library/Application Support/dolphin-emu/GC/MemoryCard{A,B}.USA.raw`.
  Both are **16,777,216 bytes** after the iOS formatter repair. That proves
  complete images, not New Game → save → quit → relaunch → load.
- post-install inventory retained the 1.36 GB game image and both complete
  16 MB cards;
- the pre/post copies are byte-identical. Slot A SHA-256 is
  `ee2f1a892801168c4226b79988d489bae5087e5fd058054509789f53e51c9bb7`;
  Slot B SHA-256 is
  `b12bde0a9d4dcbbca19363706c0de1eb8d6bd8a4f11387270c1468da39544418`.
- runtime audio log reports Speaker, 48 kHz actual output, 10 ms I/O buffer,
  a stable 5,760-frame target queue, zero underruns, and zero clipped samples.
  Source counters now name frontend, Amuse, and DSP stream output separately.
- final narrow-pass build signed, verified, installed in place, and launched at
  11:30. Installed identity: EctoPad `com.axiodl.Metaforce`, version 0.1.3 (1).
  Post-install readback retained the 1,459,978,240-byte ISO with SHA-256
  `952972a0ddb122536d2f48c20d9e119278b13f848626afc72f034ce5a1022901`
  and both 16,777,216-byte card images. No uninstall or content replacement.
- the 15:30 build contains one isolated Aurora renderer correction, upstream
  `2bbb122`: `GXSetChanCtrl` now encodes the diffuse function correctly for
  specular attenuation. The targeted test and full 165-test GX suite pass.
  The earlier isolated `b684c0d` unbound-texture experiment was physically
  rejected because black geometry and the visible-closed door persisted, and
  was removed before this build.

Do not treat this document as permission to rewrite the overlay, invent a
new camera model, or copy SunPad's Super Mario Sunshine audio timebase
patch. Keep the existing SunPad layer. Change only the narrowest seam that
the next evidence pass justifies.

## Latest hands-on boundary (2026-08-13)

The user played the installed iPad build and reported:

- the game launches and is playable enough to reach gameplay;
- audio is "pretty messed up": no music, some sound effects work, the mix is
  clearly wrong;
- both memory-card slots start empty, so the game believes it cannot save;
  the Simulator default build has the same empty-slot behavior unless
  `--autostart` creates Slot A;
- the top-right `•••` menu looks unfinished; native 1× / 2× / 3× / 4×
  resolution options are not present on device;
- a later mapping change made the left stick look instead of move and left the
  C-stick inert; that change was wrong and has been removed;
- there are unspecified texture issues throughout the game;
- the SunPad-style menu system that should have been referenced from
  `ref/sunpad` did not feel present in the running build;
- the build/sign/install/launch path was not straightforward; README and
  current docs still read as if signing hardware and a one-command device
  path do not exist.
- the first 2026-08-13 custom action-sheet menu looked worse than SunPad,
  nested buttons did nothing, and tapping away did not dismiss it. That
  fallback was removed; it must not return.
- the restored native menu now opens and its render scale, aspect ratio, and
  FPS actions work;
- left-stick movement is fixed, and the requested L/R latch now engages after 0.5 seconds;
- game main-menu option text was too dark/indistinct to navigate;
- audio popped/clicked during frontend playback and became silent after the
  transition into gameplay.
- the single-pass `FRME_NewFileSelect` correction is now physically accepted;
- a brief audio aberration remains at the main-menu-to-asteroid/planet handoff;
- the first tutorial door becomes non-solid but its model remains visible;
- enemies can show a down/dead pose at distance while still alive and firing,
  then appear upright when approached.

`ref/sunpad` is now present locally as the UI and deploy reference. It stays
git-ignored with the rest of `ref/` and must not be committed.

Earlier documentation that said Gate 2/3 never started because this machine
had no signing identity and no attached device is stale. Hardware launch
happened. Gate 3 failed on the items below.

## Priority summary

| Priority | Issue | Status after 2026-08-13 implementation |
| --- | --- | --- |
| P0 | Default memory cards / cannot save | **Full A/B images proven on device; gameplay save acceptance open.** Both are 16,777,216 bytes. |
| P0 | Audio is messed up | **Open at Amuse source/mixer.** Queue is healthy and the final limiter prevents output clipping, but Amuse still emits >1.0 peaks and discontinuity bursts. The ineffective 10 ms boundary ramp was removed. |
| P1 | Three-dot menu / 1×–4× missing | **Hardware accepted.** Controller-hosted SunPad `UIMenu`; render, aspect, and FPS actions work. |
| P1 | Movement / aim / C-stick | **Movement accepted; latch accelerated.** Left stick moves, hold R + left stick aims, C-stick selects beams, and a 0.5-second L/R hold latches until the next tap. |
| P1 | Game main-menu readability | **Fixed and hardware accepted.** `UseSingleReadableFileMenuPane` is limited to `FRME_NewFileSelect`: pane A is white and pane B's offset duplicate is hidden, while original strings, typewriter animation, geometry, and table colors remain. |
| P0 | Touch overlay disappears after screenshot/app switch | **Lifecycle recovery deployed; physical return-from-screenshot acceptance open.** Reattach to the current root-controller view, not the stale parent. |
| P1 | `•••` dismissal focus artifact | **Non-focusable button and complete image states deployed; physical visual acceptance open.** |
| P1 | First beam door presentation | **Open; diagnostics narrowed it to pose/presentation.** Open logic and collision work; the authored 0.833333-second clip is selected and reports animating, but the closed model remains visible. Do not replace animation with disappearance. |
| P0 | Black/distant world geometry and actor pose popping | **Open; one isolated lighting correction deployed.** The unbound-texture correction was physically rejected and removed. The final build contains only upstream `2bbb122`, which passes all 165 GX tests; physical visual acceptance is deferred. |
| P0 | Later-game Morph Ball stability | **Unplayable in the current physical build.** Slot 2 loads, but entering Morph Ball and approaching a tunnel ended the session. The process stayed resident without an iOS crash/jetsam/hang artifact while runtime logging stopped. Bisect the renderer compatibility set and reproduce outside the user's live container before another device build. |
| P1 | Later-game test coverage | **Mid-game USA Rev 2 state installed in Prime file slot 2, but not accepted as stable.** Existing file slot 1 is byte-preserved and the card persists across normal in-place installs. |
| P1 | Native physical controller | **Implemented; hardware acceptance open.** SDL gamepad hot-plug, platform mappings, sticks, analog triggers, D-pad, rumble, and the native mapping panel feed the existing GameCube input path. Test one real controller on this iPad before calling it supported. |
| P1 | Visual diagnostic capture | **Open.** Runtime logs can timestamp fog/audio/door state but cannot show flickering, black geometry, or actor duplication. The host screen-history recorder was unavailable for the 10:07–10:22 session. Add a bounded in-app game-frame snapshot/export path; do not rely on UIKit hierarchy capture unless it proves the Metal surface is present. |
| P2 | Deploy / README / bootstrap | This session used the documented host `codesign` + in-place `devicectl` path. Still no one-command `deploy-ios-device.sh`. |
| P3 | Later-area, controller, quiet perf, legal package | Not the current blocker; keep them listed so they are not forgotten |

## P0: insert durable Slot A and Slot B by default

### Report

The installed iPad build, and the Simulator default build, start as if no
memory cards are inserted. The game therefore thinks it has no place to save.
The user expects both slots to be present on a normal launch. This is the
first product blocker: a running Prime session that cannot save is not
playable for real use.

### Current evidence

- KI-011 already fixed kabufuda write/commit durability on macOS. That is a
  different bug. Once a card exists and is written, the file can persist.
  The iPad failure is that **no card is mounted by default**.
- `memcard.PathA` and `memcard.PathB` default to empty in
  `ref/metaforce/Runtime/CMemoryCardSys.cpp`.
- iOS, macOS, and Linux all compile `CMemoryCardSysNix.cpp`
  (`Runtime/CMakeLists.txt`). `CMemoryCardSysOSX.cpp` exists and is not
  compiled.
- `ResolveDolphinCardPath()` only returns a path if a Dolphin card file
  already exists. The first probe uses `SDL_GetPrefPath(nullptr, "dolphin-emu")`
  then, on Apple, `$HOME/Library/Application Support/Dolphin/GC/MemoryCard{A,B}.USA.raw`.
  A sandboxed iPad app will not have those desktop Dolphin files.
- `_CreateDolphinCard()` can create a file, and the in-game "Create Dolphin
  Card" UI can call it for Slot A, but only after the no-card dialog. Normal
  launch never creates Slot A or Slot B.
- `--autostart` creates **Slot A only**, and only for the Simulator test hook
  (`patches/2026-08-11-metaforce-autostart-test-hook.patch`). That is not a
  product default.

### Implementation (2026-08-13)

After `g_MemoryCardSys` is set in `CMain::MemoryCardInitializePump()`, iOS
now probes Slot A and Slot B and calls the existing `CreateDolphinCard()`
helper for any missing slot. Existing files are left alone. This does not
require a desktop Dolphin tree, `--autostart`, or the no-card dialog.
After the 2026-08-13 in-place launch, both
`Library/Application Support/dolphin-emu/GC/MemoryCard{A,B}.USA.raw` exist
on the iPad, but iOS POSIX AIO had completed only 24/64 KB of the format.
The formatter now uses complete synchronous positional I/O on iOS and replaces
only zero-megabit partial defaults. Post-launch readback proves both cards are
16,777,216 bytes. Patch:
`patches/2026-08-13-metaforce-ios-default-memory-cards.patch`.

Physical New Game → save → quit → relaunch → load is still required.

### Acceptance

A fresh install with no prior card files launches into a state that already
has cards in A and B. New Game can save. Quit and relaunch still shows that
save. Load restores the same session. Existing user card files, if any, are
preserved.

## P0: listen to and repair physical-iPad audio

### Report

Audio is the other launch blocker. There is no music. Some sound effects
work. The user described the mix as clearly messed up. This matches the
existing KI-003 boundary: implementation exists, audible mobile acceptance
does not.

### Current evidence

- Local patches already restore amuse, SDL3 output, soxr, streamed DSP/MIDI,
  and the KI-010 static-player destructor.
- The 2026-08-12 pump tops a measured 120 ms output-ready reserve instead of
  emitting one 60 Hz slice per rendered frame
  (`patches/2026-08-12-aurora-frame-rate-independent-audio-reserve.patch`).
- A loaded native macOS run held 5,292–5,294 frames at 44.1 kHz with 6–7
  voices and no underrun log. That is mechanical evidence, not iPad speaker
  proof.
- SunPad's 12× guest-timebase patch belongs to its Dolphin/recomp core. Do
  **not** copy it. Metaforce is a native reimplementation.

### Implementation (2026-08-13)

Streamed DSP never reached the mixer: `AllocateStream()` no longer started a
boo voice, and `MasterSupplyCallback` mixed only static frontend audio plus
amuse SFX. The existing DSP decoder is restored onto the current SDL/amuse
callback. `CStreamAudioManager::SupplyCallback` now mixes active streams with
`m_leftgain * x4c_vol` / `m_rightgain * x4c_vol` after static + amuse. Sample
rate stays 32 kHz stereo float. SunPad's 12× guest-timebase patch was not
copied.
Patch: `patches/2026-08-13-metaforce-restore-dsp-stream-mix.patch`.

The iOS follow-up also activates an AVAudioSession, restores SoXR per-voice
sample-rate/pitch conversion, clamps final output, and records route/rate,
queue, underrun, peak, and clip metrics. Current device logs show Speaker at
48 kHz, 10 ms I/O, a stable queue, zero underruns, and zero clipped samples.
Patch: `patches/2026-08-13-ectopad-ios-audio-session-soxr-diagnostics.patch`.

The source-level trace then proved that the queue stayed healthy while the
frontend source itself emitted full-scale discontinuity bursts. Its decoder
handled only pairs of samples, but SDL's iPad conversion alternates 682/683
input frames; each odd request dropped a frame while advancing the cursor.
The decoder is now sample/nibble accurate. Physical gameplay then rejected the
SoXR path more decisively: the log showed 193 logically running voices with
`amuse=0.000`. Amuse can temporarily return zero while a voice waits for data,
but SoXR interpreted that as permanent EOF. The live build restores the
previously audible direct pull path. Correct pitch/sample-rate conversion
remains debt until starvation and EOF can be distinguished. Gameplay music,
SFX, and listening still decide acceptance.

The 10:02 physical trace isolated the remaining menu-to-cutscene blurb: Amuse
briefly peaked at 2.129 and produced 502 clipped output samples while the
frontend source was handing off. Limiting only Amuse was insufficient because
the combined Amuse + streamed-DSP mix later reached 1.2–1.6. The current build
therefore applies the sample-local smooth limiter above a 0.8 knee,
asymptotically capped at 0.95, after every source is summed. A bounded
320-frame/10 ms frontend handoff ramp was then tested, did not remove the
audible aberration, and was removed before the final build. Source/mixer
behavior at the transition remains open. Physical listening still decides
acceptance.

Listen on the same iPad before calling this fixed. Instrument only if the
existing pump/device logs cannot name a remaining failure.

### Acceptance

Frontend music, area music, voice, beam, and footsteps are present and
recognizable. No crackle, drop, wrong pitch, doubling, or silence across FPS
dips and one background/resume cycle. Counters can support the diagnosis;
only listening closes the item.

## P1: finish the physical `•••` menu, including 1×–4×

### Report

On the physical iPad the top-right three-dot menu looks unfinished. The
native 1× / 2× / 3× / 4× resolution options are not present. The user also
expected the SunPad menu system referenced from `ref/sunpad` to be the live
product surface.

### Current evidence

The current source already contains the full SunPad menu in
`ref/metaforce/extern/aurora/lib/ios/SunPadGameOverlay.mm`:

- `•••` is a `UIButton` with `showsMenuAsPrimaryAction`;
- `buildMenu` creates a `UIMenu` with Render Resolution
  `1× (Native)` / `2×` / `3×` / `4×`, Aspect Ratio, FPS, mapping, touch
  settings, Game Data & Saves, and Share Diagnostic Log;
- the settings panel also has a `1×` / `2×` / `3×` / `4×` segmented control;
- KI-015 fixed the stub/bridge archive-order bug, and
  `patches/2026-08-12-aurora-sunpad-interaction-menu-visibility.patch`
  reasserts the overlay above SDL once per second.

Simulator harness evidence found 19 menu titles and reported
`menu visible=1 attached=1`. That does not prove the physical tap path.

Possible later causes, none proven:

- a physical tap / `showsMenuAsPrimaryAction` / SDL view-hierarchy problem;
- the installed build is older than the menu/audio sources;
- the menu presents, but the Render Resolution submenu or actions do not;
- the user expected the in-panel segmented control rather than a `UIMenu`
  submenu.

### Implementation and rejection (2026-08-13)

The first correction replaced SunPad's native `UIMenu` with a custom
`UIAlertController` action sheet. Hardware proved that decision wrong: it was
oversized, nested actions did not open, and outside taps did not dismiss it.
That entire fallback has been deleted.

The current install again matches SunPad: `showsMenuAsPrimaryAction = YES`,
one native `UIMenu`, native Render Resolution / Aspect Ratio / Game Data
submenus, selection state, icons, haptics, and automatic outside-tap dismissal.
After that first restoration still produced no menu on hardware, the overlay
host was corrected to match SunPad too: it now belongs to SDL's active root
view controller instead of being a direct `UIWindow` sibling. A touch-down log
now distinguishes a missed hit from rejected UIKit presentation. Only EctoPad
product strings and renderer notification otherwise differ. Physical testing
now confirms that the menu opens and its render resolution, aspect ratio, and
FPS actions work. Patch:
`patches/2026-08-13-ectopad-sunpad-native-menu.patch`.

## Latest physical play findings (09:28–09:36)

The direct Amuse rollback materially improved the game: the user could move,
play through cutscenes, use the in-game menu, and continue through Frigate.
The persistent trace corroborates this with 4–10 active Amuse voices producing
nonzero output after streamed audio ended. Audio remains listening-open because
brief clipping and accumulated underrun recovery still appear in the session.

The screen previously called the “main menu” is specifically
`SNewFileSelectFrame` (`FRME_NewFileSelect`). Earlier contrast changes targeted
`SFrontEndFrame`, so they could not repair the supplied screenshot. A later
global geometry override did make the screen readable, but incorrectly forced
both panes of every authored text pair to white and flattened the table's
selected/unselected colors. Hardware showed offset double text and no visible
selection. The accepted build removes all render-level overrides. The helper
`UseSingleReadableFileMenuPane` applies only in `FRME_NewFileSelect`: it sets
pane A's font color to white and hides pane B's authored offset pass. Pane A
retains the original strings, typewriter animation, widget geometry, and table
selection colors. The user physically accepted this exact solution on
2026-08-13; do not restore either global recoloring attempt.

Leaving the app to view a screenshot removed both touch controls and `•••`.
The old once-per-second recovery only raised `g_overlay` inside whatever
superview it already had. The current build observes all UIKit active/background
notifications and reattaches the existing overlay to the current root view on
foreground/active and whenever a presented frame detects host drift. The first
launch reports the hook active with `overlayWindow=1`, `menu=1`, and
`controlsHidden=0`.

The first beam door trace and the user's ability to walk through it isolate the
failure to presentation: projectile, Open messages, animation request, and
collision/material removal all occur. The stronger trace additionally proves
that the model owns an authored 0.833333-second clip, animation type 0 is set,
and `IsAnimating()` is true. Hiding the model once open made the door disappear
without its authored animation and was physically rejected. The final build
retains the original render/animation path plus diagnostics only.

The separate enemy-at-distance report remains open. Enemies can appear down or
dead while still firing and return upright when approached. That may be stale
skeletal pose, actor visibility, or renderer distance state; it is not safely
explained by the door fix. No global actor-animation/LOD change is justified
without a reproducible frame capture.

The 12:04 physical screenshot and later QuickTime capture upgrade the renderer issue from vague darkness
to direct evidence of large black and wrongly translucent world polygons. A
comparison against current official Aurora found four later fixes that apply
cleanly to the pinned renderer without importing its subsequent FIFO rewrite:
unbound texture stages sample white (b684c0d), channel diffuse encoding is
corrected (2bbb122), depth correction lives in the projection matrix (1dde08f),
and partial RGBA8 edge tiles are bounded (1a15801). The combined compatibility
set failed its first later-game stability gate and has been rolled back from
the hardware build. A later one-change physical run of `b684c0d` still showed
the same black geometry and visible-closed first door, so that correction was
also rejected alone and removed. The final build instead isolates `2bbb122`,
the GX channel diffuse-function correction. Its targeted test and all 165 GX
tests pass, but it has not received physical visual acceptance. The rejected
four-change bundle remains documented in
`patches/2026-08-13-aurora-renderer-compatibility.patch`; the deployed change is
recorded in `patches/2026-08-13-aurora-gx-channel-lighting.patch`.

The former fog trace was itself defective instrumentation: normal draw code
switches room fog on and off per material, producing nearly 79,000 lines. It now
records a five-second count/mode summary plus the last parameters.

## P1: persistent later-game test state

A public Dolphin save explicitly marked compatible with Metroid Prime USA Rev
2 was validated locally. The mid-game state starts in Chozo Ruins with multiple
beams, Spider Ball, Gravity Suit, 11 energy tanks, and 120 missiles. It is a
better renderer/control test than replaying Frigate or jumping directly to the
final boss.

`scripts/merge-prime-save-slot.cpp` merges only the donor's 940-byte game-state
record into Prime's internal file slot 2. It validates GM8E/01, one-block GCI
layout, donor/current Prime CRCs, full card structure, and an empty destination;
then reopens the result and proves file slot 1 remains byte-identical. A second
merge correctly refuses to overwrite the occupied slot.

The current device card was cleanly backed up to
`/tmp/ectopad-slot2-device-backup.jwnXnm`. Original Slot A SHA-256 was
`f6a80dfda737edba53fac2e4a7561730bbfc6d1db846724213cc152de761fd03`;
the merged/read-back card is
`ee2f1a892801168c4226b79988d489bae5087e5fd058054509789f53e51c9bb7`.
The upload and device readback match exactly. Slot B was not modified.
The subsequent renderer-compatibility build was installed in place and its
post-install Slot A readback remained byte-identical at the same SHA-256.
After its failed stability run, the compatibility set was removed and the
renderer-baseline rollback installed in place at 14:10. Its post-install card
readback again matched the same SHA-256 exactly.

This is Prime's on-screen **file slot 2**, not GameCube hardware Slot B. It
persists in `MemoryCardA.USA.raw` inside the app data container across normal
in-place installs. Re-inject only after deliberate container replacement or if
file slot 2 is erased; never overwrite a newly occupied user slot silently.

The first physical use of this state is a failed stability gate: entering Morph
Ball and moving toward a tunnel ended the play session. The launched PID was
1812; afterward EctoPad was resident as PID 1826. iOS emitted no crash,
jetsam, GPU, or hang report, and the original runtime log ended without a
lifecycle notification. Treat file slot 2 as a
reproduction fixture, not as evidence that later-game play is supported.

### Acceptance

Tap `•••` during title and Frigate. Every documented item appears, including
native 1× / 2× / 3× / 4×. Switching 1× and 2× visibly changes the
framebuffer. The overlay stays above gameplay.

## P1: restore original Prime movement, aim, lock-on, and beam selection

### Report

The first change incorrectly made a deflected logical look input count as
LookHold. Because Prime reads look/aim from the left Control Stick while R is
held, ordinary left-stick input entered look mode and movement stopped. The
yellow C-stick remained ineffective as a camera because it is not Prime's
camera control.

### Current evidence

The touch path is wired:

- overlay writes `cStickX` / `cStickY`;
- mixer forwards them;
- `PADUpdateTouchVirtualPad` in
  `ref/metaforce/extern/aurora/lib/dolphin/pad/pad.cpp` sets
  `substickX` / `substickY`;
- `PADRead` consumes that virtual pad.

The original Prime mapping is: left Control Stick moves; hold R and use that
same stick to look/aim; L locks on and enables target-relative movement;
C-stick directions select beams; D-pad directions select visors.

### Implementation (2026-08-13)

The iOS LookHold override is removed. R is now an ordinary static touch button
that holds digital R and full trigger pressure until release. The yellow stick
remains wired to GameCube substick/beam selection, and the Sunshine-specific
Modern C-stick toggle is removed from EctoPad. Patch:
`patches/2026-08-13-ectopad-restore-prime-touch-controls.patch`.

### Acceptance

Left stick walks normally. Holding R while moving the left stick aims/looks;
releasing R restores movement. Holding L locks onto a target and allows
target-relative movement. C-stick directions change beams when those beams are
available. Morph Ball and D-pad visor selection remain intact.

## P2: capture the texture issues before touching GX

### Report

There are texture issues throughout the game. The report does not yet name
rooms, assets, or a screenshot set.

### Current evidence

KI-001 fixed frontend movie scissor/viewport clipping on macOS. That is not
proven to be the iPad texture report. Do not reopen GX/texture work from a
generic "textures look wrong" note.

### Next investigation

Capture screenshots and room names on the same iPad. Separate frontend
movies, HUD, world textures, and water/visor effects. Only then decide
whether this is a recurrence of KI-001, a scale/filter issue, or a new GX
path.

### Acceptance

Each named defect has a screenshot, a room/asset, and either a verified fix
or a documented source-asset limitation.

## P2: make physical iPad build/sign/install a front-door path

### Report

Launching the attached iPad was not as straightforward as it should have
been. The process pulled unexpected dependencies, and the README still
points at status docs that described an unsigned IPA and a missing signing
identity. This is first-class process debt, not a footnote.

### Current evidence

- README only sends readers to `CURRENT_STATE.md`. There is no one-command
  iPad path.
- `docs/BUILDING.md` lists CMake/Ninja/Python+markupsafe/Homebrew SDL3, then
  raw cmake presets. The first device configure also needs Rust 1.97.1 plus
  Apple targets, vendored audio, and Dawn. That is easy to miss.
- `docs/INSTALL_IPA.md` still described an unsigned validation IPA and said
  this machine had no identity and no device.
- EctoPad has only `scripts/package-ios.sh` and `scripts/audit-ios-package.sh`.
  There is no bootstrap for ignored `ref/` clones/pins/patches, and no
  `deploy-ios-device.sh`.
- `ref/sunpad` now has the missing reference pieces:
  `bootstrap-dependencies.sh`, `ios-build-core-device.sh`,
  `ios-provision-device.sh`, and `deploy-ios-device.sh`.

Physical install must sign the **source**
`ref/metaforce/build/ios-default/Binaries/Metaforce.app`, verify with
`codesign --verify --deep --strict`, then
`xcrun devicectl device install app` in place and launch with
`--terminate-existing`. Bundle ID is `com.axiodl.Metaforce`. The unsigned
IPA at `/tmp/Metaforce-unsigned-menu-audio-2026-08-12.ipa` (SHA-256
`308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`) is
validation-only. Do not install it.

Preserve the live container. Back up `Documents` and `Library` separately.
Never use `--remove-existing-content` against the app container without
explicit authorization and a tested restore. After deploy, distinguish
install/PID from audio, save, menu, and control acceptance.

The 2026-08-13 P0/P1 rebuild used that exact host path: sign the source
`.app`, verify, back up `Documents` then `Library`, install in place,
launch with `--terminate-existing`, and recopy the live container. The final
handoff PID was 1940. It is still not a one-command script, and it is
still not gameplay acceptance.

### Next implementation, once approved

Add a small EctoPad bootstrap for the ignored `ref/` pins and patches, plus
a thin `scripts/deploy-ios-device.sh` that rediscovers the attached iPad,
signs the source `.app`, verifies, installs in place, and launches. Update
README so the physical path is visible without reading the whole evidence
ledger. Follow the existing iOS physical-device deploy skill: discover the
UDID, sign the source `.app`, verify, install in place, launch with
`--terminate-existing`, and keep install/PID distinct from gameplay
acceptance.

### Acceptance

From a documented front door, a later session can bootstrap (if needed),
build `ios-default`, development-sign, install in place, and launch without
discovering hidden vendor steps. The unsigned IPA remains a validation
artifact only. User data survives the install.

## P3: keep the later gates visible

These are not the current morning blockers:

- representative Chozo / Tallon / Phendrana / Magmoor / Phazon sessions;
- physical Apple GameController discovery, reconnect, mapping, rumble, and
  touch handoff;
- quiet-system frame-time, RSS, thermal, and audio-underrun measurements;
- GPL corresponding source, LGPL relink materials, and Dawn provenance
  before any public package.

## Goal-based execution plan

### Goal

Make the already-launched iPad build actually playable: default saves, listenable
audio, a complete `•••` menu, and original Prime movement/aim controls. Then make the
next hardware launch boring.

### Working rules

- One issue at a time, P0 first.
- No overlay restyle. No ImGui mobile shell. No SunPad 12× audio port.
- No Simulator-only closure for audio, saves, menu tap, or controls.
- Preserve `ref/`, saves, settings, and the live app container.
- Stage reviewed paths only. Do not commit game data or signing material.

### Phase 0: freeze the current hardware baseline

Keep the installed container. Export Share Diagnostic Log. Note the
installed app version if it is still present. Do not wipe the device to
"start clean" unless the user asks.

### Phase 1: default cards

Create and mount Slot A and Slot B in the iOS sandbox on first launch.
Accept only with New Game → save → quit → relaunch → load on the same iPad.

### Phase 2: audible audio

Listen first. Instrument only if the existing pump/device logs cannot name
the failure. Accept only by ear on the same iPad.

### Phase 3: physical `•••` and 1×–4×

Prove the installed overlay is the current SunPad menu, then fix the
narrowest presentation/hit-test/wiring gap. Accept with a visible 1×/2×
framebuffer change.

### Phase 4: original Prime controls

Accept normal left-stick movement, hold-R plus left-stick aim, L lock-on and
strafe, and C-stick beam selection on the physical iPad.

### Phase 5: textures, then deploy docs/scripts

Capture the texture set before GX work. Add bootstrap/deploy only after the
playability P0/P1 items have an implementation plan, unless a later session
is specifically about the install path.

## Do not

- Treat build, signing, install, or a live PID as Gate 3 acceptance.
- Copy SunPad's 12× guest-timebase audio patch.
- Redesign or replace the SunPad overlay.
- Invent a new camera model before measuring LookHold.
- Reopen KI-001 as the iPad texture cause without screenshots.
- Use `--remove-existing-content` on the live container without backup and
  explicit authorization.
- Commit `ref/`, ISOs, saves, logs with container paths, or signing material.
