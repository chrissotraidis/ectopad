# Stop-for-today handoff — physical iPad correction pass

Last updated: 2026-08-13 after the 15:30 deployment.

## Running build

The attached iPad is running the final development-signed build for the day:

- display name: **EctoPad**
- bundle ID: `com.axiodl.Metaforce`
- version: `0.1.3 (1)`
- source app: `ref/metaforce/build/ios-default/Binaries/Metaforce.app`
- install: in place, without uninstalling or replacing the data container
- signing: `codesign --verify --deep --strict` passed
- process at handoff: PID 1940

The game image and both memory cards were copied before and after installation
and compared byte-for-byte. Their SHA-256 values are:

- `game.iso`: `952972a0ddb122536d2f48c20d9e119278b13f848626afc72f034ce5a1022901`
- Slot A: `ee2f1a892801168c4226b79988d489bae5087e5fd058054509789f53e51c9bb7`
- Slot B: `b12bde0a9d4dcbbca19363706c0de1eb8d6bd8a4f11387270c1468da39544418`

Slot A contains the preserved original file slot 1 and the injected USA Rev 2
mid-game fixture in file slot 2. Do not reinject it unless file slot 2 is
deliberately erased.

## Accepted on the physical iPad

- the app is named EctoPad;
- the SunPad-native `•••` menu opens and dismisses;
- render resolution, aspect ratio, and FPS actions apply;
- Prime's file menu is readable with one authored text pane;
- left-stick movement is restored;
- original GameCube/Prime mapping is restored, with a 0.5-second L/R touch
  latch deployed;
- gameplay can run at 60 FPS in the tested Frigate sections;
- both memory-card images exist as complete 16 MiB files.

## Final isolated renderer candidate

The four-change Aurora compatibility bundle was rejected and removed. A later
physical build containing only the unbound-texture correction (`b684c0d`) still
showed the black room and visible-closed first door, so it was also removed.

The installed build contains only the newly isolated upstream Aurora
`GXSetChanCtrl` correction (`2bbb122`) from that set. The targeted specular
channel test and the full Aurora GX test binary pass: 165/165. There was no
additional physical gameplay run after this final deployment, so this is a
candidate, not a visual-fix claim.

## Door finding

The first tutorial door's logic and collision are working. New diagnostics
prove the authored 0.833333-second clip exists, animation type 0 is selected,
and the model reports `animating=true`. The closed-looking model nevertheless
remains visible while the player can walk through it. The remaining defect is
animated-model pose/presentation, not a missing projectile hit, Open message,
collision transition, or clip.

Do not restore the rejected workaround that abruptly hides the model.

## Open gates for the next session

1. Test the same black-room/first-door path on the installed `2bbb122` build.
2. Test file slot 2 and Morph Ball stability without assuming the earlier
   session end was a confirmed crash; no iOS crash/jetsam/hang artifact existed.
3. Listen across the frontend-to-asteroid/planet transition. The queue remains
   healthy, but the audible aberration persists. The ineffective 10 ms ramp and
   SoXR starvation path are removed.
4. Exercise screenshot/background/foreground return and confirm the touch
   overlay reattaches.
5. Perform a real save-station save, terminate/relaunch, and load it.
6. Test one physical Apple controller before claiming controller support.

The full prioritized evidence remains in [TECH-DEBT.md](TECH-DEBT.md), with
dated runs in [TESTING.md](TESTING.md) and individual symptoms in
[KNOWN_ISSUES.md](KNOWN_ISSUES.md).

## Repository boundary

`ref/`, the ISO, saves, logs, recordings, profiles, signatures, and built apps
stay local and ignored. GitHub receives documentation, source-delta patch
records, and the save-slot merge helper only. Rejected experiments remain
labeled as rejected history and are not part of the accepted build recipe.
