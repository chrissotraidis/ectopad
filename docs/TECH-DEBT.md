# EctoPad technical debt

Updated 2026-08-17 after the physical first-door and black-world-geometry
reproduction. Build, signing, install, PID, and container readback are evidence,
not hands-on acceptance.
Default A/B card formatting is proven at the container level. The native menu,
render scale, aspect ratio, FPS toggle, left-stick movement, and corrected
single-pass file menu are now accepted on hardware. Audio is source-instrumented and two concrete lifecycle/decoder
bugs are repaired, but gameplay listening remains open. Prime's original
controls are restored and L/R now offer an optional half-second touch latch.
Named texture defects remain open. The exact audio crash is fixed in source and
deployed, but the physical gameplay trigger still needs replay.

## Latest deploy evidence (2026-08-14, 11:13)

- `pumpAndMix()` now snapshots `m_voices` for each 5 ms chunk so recursive
  child-voice allocation cannot invalidate the active iterator. This is the
  narrow fix for two matching `+656` / `0x18` crash reports.
- The iPhoneOS build completed as arm64 UUID
  `CE8136C1-42AD-3A76-9479-E0C20B992225`; strict deep signature verification
  passed.
- The same signed EctoPad 0.1.3 (1) app was installed in place on the attached
  iPad Pro and iPhone 14. No uninstall, `--remove-existing-content`, or app-data
  container replacement was used. Both copies launched (PIDs 2542 and 11711).
- The iPad has a complete pre-install `Documents`/`Library` backup. It includes
  the 1,459,978,240-byte ISO, both 16,777,216-byte save cards, and the custom
  preferences plist; hashes are recorded in `TESTING.md`.
- The iPhone CoreDevice file-transfer service timed out, so do not claim
  pre/post byte proof for that container. Its evidence is the standard
  data-preserving in-place install plus successful launch.
- Physical gameplay replay of the voice-spawn path remains required before
  KI-026 is hardware-accepted.

## Prior deploy evidence (2026-08-13, 20:21)

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
- a later 34-minute-9-second physical session recorded zero audio queue
  underruns, zero clipped output, and no fatal, jetsam, GPU, hang, or memory
  warning. After a background interval, the overlay recovery reattached. The
  current diagnostic format did not persist the controller name, so physical
  controller use remains the user's hands-on evidence.
- four subsequent HDMI attempts produced matching `SIGABRT` reports. HDMI was
  creating another UIKit scene and SDL entered the process-wide Metaforce main
  again, causing the second `FileStoreManager` singleton construction to
  terminate. The final build guards SDL's scene entry so only the primary scene
  starts the engine. Physical HDMI replay remains open.
- the 20:21 build also replaces the inherited compiled icon and loose PNGs with
  the tracked original green ectoplasm identity from `assets/app-icon/`.
- that build was signed, strictly verified, installed in place, and launched as
  PID 2122. Pre/post ISO and Slot A/B copies remained byte-identical at the
  hashes above. No uninstall or destructive container copy was used.

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
| P0 | First beam door presentation | **Open; isolated to the special same-area `Door_Area Inside/Outside` pair.** Logic, collision, authored animation, pose advancement, frustum visibility, and renderer submission all occur. Ordinary doors using the same assets animate correctly. Do not replace the animation with disappearance or apply a generic all-door override. See the dedicated debt record below. |
| P0 | Black/distant world geometry | **Open; 2026-08-17 physical run rejected the static-array lifetime candidate.** Multiple Frigate rooms still contain large selectively black world surfaces while HUD/emissive elements remain correct. Missing textures, global fog/exposure, screenshot lifecycle, unbound-texture fallback, and same-pointer area-array reuse are not sufficient explanations. See the dedicated debt record below. |
| P1 | Injured Space Pirate pose transition | **Open; authored behavior must be separated from presentation failure.** The early Frigate encounter intentionally supports seated/injured pirates that get up when activated. Lying down at distance is not by itself a defect; snapping upright, firing while visibly prone, duplication, or a missing get-up animation is. See the dedicated debt record below. |
| P0 | Later-game Morph Ball stability | **Prior session now explained by KI-026.** The retired report shows the same audio-mixer iterator crash; the card and Morph Ball state are not implicated by that stack. The snapshot fix is deployed, and physical checkpoint replay remains open. |
| P1 | Later-game test coverage | **Mid-game USA Rev 2 state installed in Prime file slot 2, but not accepted as stable.** Existing file slot 1 is byte-preserved and the card persists across normal in-place installs. |
| P1 | Native physical controller | **Implemented; hardware acceptance open.** SDL gamepad hot-plug, platform mappings, sticks, analog triggers, D-pad, rumble, and the native mapping panel feed the existing GameCube input path. Test one real controller on this iPad before calling it supported. |
| P0 | HDMI / external display | **Root cause fixed in source and deployed; physical replay open.** SDL re-entered Metaforce main for HDMI's additional UIKit scene. The iOS guard retains the existing engine instead. Reconnect HDMI and verify visible, controllable gameplay before calling it supported. |
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

### P0 debt record: first tutorial door has state but no visible animation

#### User-visible failure

This is a day-one release blocker because it is the first door the player is
taught to shoot. The shot/open sound plays and the collision becomes passable,
but the door panels remain visibly closed and do not play the authored opening
motion. Every ordinary door tested immediately afterward opens normally. The
defect is therefore not permission to change generic beam-door behavior.

An earlier workaround hid the model after the logical Open state. Hardware
showed an abrupt disappearance rather than the authored motion, so that
workaround was rejected and removed. Never restore it as a release fix.

#### Physical evidence from 2026-08-17

The relevant session is `EctoPad-Diagnostic-20260817-103041.log`, captured from
the iPad build running at 2x/original 4:3. At `10:27:34Z` the problematic object
is identified as a special pair rather than an ordinary area-transition door:

- UID 410: `Door_Area Inside`, area 0, dock 1023;
- UID 409: `Door_Area Outside`, area 0, dock 1023;
- both are activated through source UID 412 and then open each other;
- both use character `Node1`, model `d3d3ab81`, skin `b8fc0457`, layout
  `81e65611`, open primitive `newmetroiddoor_open`, animation resource
  `6bc42b3e`, and animation DB index 0;
- the authored duration is 0.833333 seconds and the configured door length is
  0.5 seconds.

For both objects, type 0 is selected, `IsAnimating()` becomes true, the pose is
cached and built, `outOfFrustum=false`, and renderer submissions advance from
0 to 7, 19, and 30. The remaining animation time reaches zero. The pose hashes
also change at the expected samples:

| Sample | Frame | Remaining | Pose hash |
| --- | ---: | ---: | --- |
| start | 1 | 0.805555 | `411a4e88614f06fa` |
| early | 8 | 0.611111 | `f9d432a50be20aee` |
| late | 20 | 0.277778 | `be89cc8ae41e116a` |
| complete | 30 | 0.000000 | `0e4290952e6a617d` |

The next ordinary door provides a controlled comparison in the same session.
UIDs 204/827 open at `10:28:00Z` after a direct projectile hit, span areas 0/1
with real docks 401/819, use the exact same model/skin/layout/open-animation
assets, produce the same pose-hash sequence, and visibly work. Subsequent pairs
across areas 1/2, 2/4, 4/6, 6/8, and 8/10 repeat the working pattern. This rules
out a missing animation asset, wrong animation index, zero-duration clip,
general door playback defect, general door skin-rule count defect, and general
Aurora inability to draw that door model.

All logged root translation deltas are zero and root rotation remains identity
for both the broken special pair and working ordinary pairs. Root motion is not
the panel animation mechanism and must not be invented as the fix.

The iPhone trace `EctoPad-Diagnostic-20260817-120058.log` independently repeats
this comparator after a real foreground resume. The latest cumulative session
runs EctoPad 0.1.3 (1) on iOS 26.6 at `renderScale=2`, `aspectMode=1`; it resumes
at `11:54:31Z`, then opens the visibly broken UIDs 410/409 at `11:58:09Z`.
Those actors again complete the same four pose hashes with 30 renderer
submissions and a constant sampled workspace hash. UIDs 204/827 open nine
seconds later with the same assets and pose hashes while their sampled
workspace hashes change. The user's physical observation supplies the visible
broken/working distinction; the log itself supplies the repeated state trace.
This makes the first-door symptom cross-device and shows it survives foreground
resume, but it does not remove the shared-workspace limitation described below.

#### What is established and what is not

Established:

- gameplay state is correct: Open is requested and collision/material state is
  removed, which is why Samus can walk through;
- the authored animation tree and CPU pose advance to completion;
- the special pair is visible to the frustum and reaches `AddToRenderer`;
- the defect is specific to the same-area `Inside/Outside` presentation path or
  something unique to its script wiring;
- ordinary doors prove that changing `CScriptDoor` globally would be dangerous.

The 2026-08-17 workspace hash is useful but not instance-local proof.
`CCharacterFactory::CreateCharacter()` obtains the `CSkinnedModel` through its
factory cache, so identical door characters share the model's mutable skinning
workspace. Another door render can overwrite that workspace before a later
`Think()` diagnostic samples it. The special pair retained hash
`9a931887ff44fc62` while working doors showed changing hashes, but this does not
by itself prove that UID 409/410 skipped skinning. Future diagnostics must hash
the vertices immediately after `CAnimData::SetupRender()`/`CSkinnedModel::Calculate()`
and associate the observation with the actor currently rendering.

#### Rejected and unsafe approaches

- Do not hide the door model after Open; hardware rejected the visual pop.
- Do not copy another project's door patch or substitute an ordinary door's
  script blindly. This pair has different area/dock/source topology.
- Do not force all doors to rebuild, disable culling, or use a global animation
  override. Later doors already work and are the regression controls.
- Do not treat `IsAnimating()`, a changing pose hash, or renderer-submission
  count alone as proof that skinned vertices reached the draw.
- Do not change the authored 0.833333/0.5 timing; working doors use it too.

#### Next bounded investigation

1. Dump the area-0 script objects and connections for UIDs 409, 410, and 412,
   including transforms, active state, actor parameters, model flags, draw
   flags, and every incoming/outgoing message. Compare them with UIDs 204/827.
2. Add an actor-keyed trace at the actual draw boundary, not in `Think()`. For
   UIDs 409/410 only, record entry into `CActor::RenderInternal`,
   `xe7_27_enableRender`, `xe7_29_drawEnabled`, selected model, model/workspace
   addresses, model matrix, material set, workspace counts/hash immediately
   after skinning, and the vertex arrays passed to `CModel::Draw`.
3. Determine whether both special actors draw overlapping copies, whether a
   separate static/script actor supplies the visible closed panels, and whether
   source UID 412 is expected to animate that separate presentation object.
4. Reproduce area 0 on macOS with the same game revision and compare the keyed
   trace. A platform difference belongs below the door state machine; a matching
   failure indicates missing special-case script/presentation support.
5. Fix only the first demonstrated failing seam, then run ordinary-door and
   full-suite regressions before another device build.

#### Acceptance

On physical iPhone and iPad hardware, shooting the first door must visibly play
the authored open animation, leave collision synchronized with the displayed
panels, and visibly play its close animation where authored. The immediately
following ordinary doors must retain their current open/close behavior. Repeat
after a foreground resume and at 1x, 2x, and 3x render scale. Automated
animation/hash tests are supporting proof only; the physical visual pass is
mandatory.

### P1 debt record: injured Space Pirate proximity pose

The two early large enemies must not be grouped automatically with the black
world-geometry defect. The Prime encounter intentionally contains injured
pirates, and `CSpacePirate` reads a seated flag, tracks whether the actor has sat
up, and owns an explicit GetUp state. A pirate appearing prone at distance and
getting up as Samus approaches can therefore be authored behavior rather than a
draw-distance error.

The actionable defect boundary is narrower: investigate if a pirate fires while
its visible body remains prone, snaps upright without the authored transition,
duplicates, changes pose only after crossing a visibility boundary, or has a
logical GetUp animation whose skinned vertices do not change. Record the exact
room and actor IDs, player distance, seated/sat-up flags, AI state, animation ID,
pose flags, bounds, and vertex hash, then compare the encounter with the original
game at the same location.

If the state machine requests GetUp but the visible pose is stale, test the same
animation/skinning correction as the first door. If activation itself occurs at
the wrong range or state, correct the Space Pirate state transition instead.
Do not apply a global actor LOD, culling, or animation override without this
distinction. Acceptance requires the intended injured pose, visible get-up
transition, firing state, and collision to remain synchronized at both original
and higher render scales.

### P0 debt record: selective black world geometry on physical iPhone and iPad

#### User-visible failure

Large portions of Frigate world geometry intermittently render completely or
nearly black. The affected shapes follow individual walls, beams, debris, and
other world surfaces; adjacent surfaces, the HUD, Samus's arm cannon, emissive
lamps, screens, and door shields can remain correct. Some dark surfaces reveal
their expected texture under a weapon flash or nearby light. The defect can be
subtle darkness in one view and release-blocking black silhouettes occupying
most of the screen in another. It is not practical or correct to patch textures
one by one.

The strongest current physical evidence is the 2026-08-17 set:

- `Screenshot 2026-08-17 at 12.28.24 PM.png`;
- `Screenshot 2026-08-17 at 12.29.13 PM.png`;
- `Screenshot 2026-08-17 at 12.29.46 PM.png`;
- `Screenshot 2026-08-17 at 12.30.19 PM.png`;
- correlated log `EctoPad-Diagnostic-20260817-103041.log`.

The images span several consecutive Frigate rooms and area transitions. The
same exact game area reached through macOS warp (`--warp 1 6`) renders normally
through Dawn/Metal, so the source textures and intended room lighting are not
intrinsically black. This remains a physical-iOS/backend-state defect until a
more precise boundary is proven.

#### Cross-device iPhone evidence from 2026-08-17

The user physically observed the same black geometry on the iPhone 14 and also
reported surfaces or texture detail appearing to move in and out of focus with
distance. The corresponding file,
`EctoPad-Diagnostic-20260817-120058.log`, is a 1,842-line cumulative rotating
log. Its latest session runs from `11:07:55Z` through the share request at
`12:00:58Z`, identifies EctoPad 0.1.3 (1) and iOS 26.6 (23G71), and retains
`renderScale=2`, `aspectMode=1` throughout gameplay. Device model comes from the
physical-test report; the log format does not currently record it. The same
version/build fields are also insufficient to prove binary identity because the
log omits Git/patch revision, executable UUID, and a build fingerprint.

The app backgrounds at `11:46:44Z`, returns at `11:54:31Z`, and continues
rendering through areas 0, 1, 2, 4, 6, 8, 10, 12, and 14. The overlay remains
attached. After resume the trace contains 55 normal fog summaries, six authored
mode-2 summaries, one mixed transition summary, and five authored mode-4
summaries. There is no Dawn validation error, Metal device loss, GPU reset,
renderer assertion, fatal event, crash marker, or memory warning. Render scale
and aspect mode do not change when the symptom is reported.

This establishes a cross-device physical-iOS failure and weighs against an
iPad-only viewport, 3x-resolution, screenshot-lifecycle, or global-fog cause.
It does **not** identify the draw that became black, nor does it prove that the
distance-dependent focus symptom is mipmapping, anisotropic filtering, texture
streaming, level-of-detail selection, or dynamic resolution. None of those
states are recorded. The next capture must keep the focus/sharpness symptom
separate from black geometry until both are tied to the same draw and state.

#### Timeline and eliminated lifecycle/fog explanations

Each screenshot is followed by `UIApplicationWillResignActiveNotification`
because the user left EctoPad to inspect/capture it. The black geometry is
already visible before those notifications:

- image 12:28:24, resign event `10:28:25Z`;
- image 12:29:13, resign event `10:29:14Z`;
- image 12:29:46, resign event `10:29:47Z`;
- image 12:30:19, resign event `10:30:20Z`.

Foreground recovery succeeds and the overlay reattaches, but resume neither
creates nor clears the world defect. Screenshot/background lifecycle is
therefore not the cause.

The five-second fog summaries remain internally consistent while the defect is
visible. Black surfaces occur with the normal mode mask `0x1` and also in rooms
that legitimately use mode 2/fog mask `0x5`. The HUD and emissive surfaces are
not globally darkened. A single bad global fog color, exposure value, render
scale, or presentation copy is not a sufficient explanation. The log contains
no Dawn validation error, Metal device loss, assertion, fatal renderer event,
or GPU reset.

#### Renderer experiments and their outcomes

The following candidates are explicitly rejected or incomplete:

1. The four-change Aurora compatibility bundle—unbound texture white fallback
   (`b684c0d`), channel diffuse encoding (`2bbb122`), projection depth
   correction (`1dde08f`), and bounded partial RGBA8 edge tiles (`1a15801`)—was
   not accepted as a bundle and was rolled back.
2. `b684c0d` alone was physically rejected: the same black geometry and first
   door failure remained. Missing/unbound texture fallback is not the root
   explanation.
3. `2bbb122` corrects real GameCube channel-control encoding and passed its
   focused/full GX tests, but later physical screenshots still contain black
   world surfaces. It is not a complete visual fix.
4. A 2026-08-17 candidate cleared Metaforce's cached GX vertex-array bindings
   when static area geometry was removed, targeting allocator-address reuse.
   A deterministic FIFO regression proved clear/rebind invalidation, and the
   iPad build installed successfully, but the four screenshots above physically
   reject it as the solution. Aurora also clears each GX array's uploaded
   `cachedRange` at every `end_frame`; same-pointer area reuse within one frame
   is therefore too narrow to explain a defect that persists across frames and
   rooms.
5. Raw RGBA/texture-format changes temporarily altered individual appearances
   but did not generalize. Do not resume format-by-format or texture-by-texture
   whack-a-mole without a failing draw identity.

These outcomes do not prove that all vertex state is correct. They prove only
that the specific static-area binding lifetime theory is insufficient.

#### Current hypothesis boundary

The source images are present, geometry selection continues, and only selected
world surfaces become black. The best remaining boundary is per-draw static
world state: vertex-normal/index interpretation, material/channel state, TEV
inputs, texture-stage binding, or a pipeline/bind-group state leak. Incorrect
normals or channel state would also explain why a local weapon light can reveal
an otherwise black textured surface. Large silhouettes mean vertex/index
attributes must remain in scope until a captured draw proves their positions
and indices are correct.

This is an inference, not a root-cause claim. The current diagnostic records
room fog and high-level events but does not identify the model, surface,
material, arrays, pipeline, or shader state responsible for a black pixel.

#### Next bounded investigation

1. Use one named Frigate view that is correct on macOS and reproducibly black
   on both iPhone and iPad. Keep game revision, room, camera position, visor,
   and render scale fixed.
2. Add one diagnostic header containing hardware model, OS build, GPU/adapter,
   Git/patch revision, executable UUID or source fingerprint, render size,
   render scale, aspect mode, and active experimental-patch flags.
3. Add a bounded world-surface capture for that area: area/model/surface and
   material IDs; draw bounds; primitive/index range; vertex descriptor; array
   pointers, byte sizes, strides, endian flags, and small CPU hashes; normal
   format/range; active GX channels/lights; TEV stage count and inputs; texture
   bindings plus sampler/mip/LOD state; blend/depth/cull state; and Aurora
   pipeline/bind-group identity.
4. Capture the same draw on macOS, iPhone, and iPad and compare the first
   divergent value. Do not log every draw indefinitely or dump proprietary
   texture/vertex contents; identifiers, bounded hashes, formats, and state are
   sufficient.
5. If CPU draw state matches, inspect the generated shader/uniform/storage
   ranges and Dawn buffer offsets for that pipeline. If CPU state differs, fix
   the earliest Metaforce/GX state transition that diverges.
6. Test one variable per build. Revert the rejected static-array candidate
   before forming the next production patch unless new evidence independently
   justifies keeping it.

#### Acceptance

The four named Frigate views and the earlier black-room capture must render
without black/missing world surfaces on physical iPhone and iPad hardware.
Verify before and after foreground resume, after several area transitions, and
at 1x, 2x, and 3x. Record a fixed-camera approach to one surface exhibiting the
iPhone focus/sharpness symptom and compare its texture, sampler, mip/LOD, and
draw identity frame by frame. Weapon flashes must illuminate surfaces naturally
rather than merely expose otherwise black materials. HUD, emissive surfaces,
doors, translucency, fog, and macOS rendering must not regress. Run the focused
GX tests, the complete Aurora/Metaforce suite, and sustained physical Frigate
traversals; automated tests do not replace visual acceptance on both devices.

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

## Superseded renderer intake note

The former P2 request to capture a vague "texture issue" is complete and
superseded by the P0 black-world-geometry debt record above. The defect now has
named physical screenshots, correlated lifecycle/fog logs, macOS comparison,
rejected candidates, and a bounded state-capture plan. Treat it as selective
world rendering state, not as an unspecified texture-file problem.

## P2: make physical iPad build/sign/install a front-door path

### Report

Launching the attached iPad was not as straightforward as it should have been.
The process pulled unexpected dependencies, and the source-build/sign/install
path is still not a clean front door. The public unsigned IPA has now been
removed; this remaining workflow gap is first-class process debt.

### Current evidence

- README only sends readers to `CURRENT_STATE.md`. There is no one-command
  iPad path.
- `docs/BUILDING.md` lists CMake/Ninja/Python+markupsafe/Homebrew SDL3, then
  raw cmake presets. The first device configure also needs Rust 1.97.1 plus
  Apple targets, vendored audio, and Dawn. That is easy to miss.
- `docs/INSTALL_IPA.md` now states that no public IPA exists and directs users
  to the source-build/sign path.
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

From a documented front door, a later session can bootstrap (if needed), build
`ios-default`, development-sign, install in place, and launch without
discovering hidden vendor steps. Any locally generated unsigned IPA remains
temporary validation output only and must not be published. User data survives
the install.

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
