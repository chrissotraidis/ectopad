# SunPad UI Parity

Last updated: 2026-08-12

## Reference and policy

The Apple overlay reference is `ref/sunpad` at
`7d84cec8bb607c32d76499af0e7bd7435ac82d5e`. Metaforce's UIKit overlay lives
in `ref/metaforce/extern/aurora/lib/ios/`. UI, settings, touch controls, the
layout editor, and diagnostic presentation must be ported from SunPad rather
than independently redesigned.

## Source mapping

| SunPad source | Metaforce/Aurora port | Parity at 2026-08-12 |
| --- | --- | --- |
| `apple/ios/SunPadGameOverlay.h` | `lib/ios/SunPadGameOverlay.h` | Byte-identical |
| `apple/ios/SunPadGameOverlay.mm` | `lib/ios/SunPadGameOverlay.mm` | Byte-identical |
| `apple/shared/SunPadSettings.h` | `lib/ios/SunPadSettings.h` | Byte-identical |
| `apple/shared/SunPadSettings.mm` | `lib/ios/SunPadSettings.mm` | Byte-identical |
| `apple/shared/SunPadInputMixer.h` | `lib/ios/SunPadInputMixer.h` | Byte-identical |
| `apple/shared/SunPadInputMixer.mm` | `lib/ios/SunPadInputMixer.mm` | Byte-identical |
| `apple/shared/SunPadInputState.h` | `lib/ios/SunPadInputState.h` | Byte-identical |
| `apple/shared/SunPadDiagnostics.h` | `lib/ios/SunPadDiagnostics.h` | Byte-identical |
| `apple/shared/SunPadDiagnostics.mm` | `lib/ios/SunPadDiagnostics.mm` | One intentional app-name change: the persistent log root is `Metaforce/Logs` instead of `SunPad/Logs` |

`OverlayBridge.mm` is Metaforce-specific glue. It attaches the unchanged
SunPad view above SDL's UIKit/Metal view, forwards the mixer's
`SunPadInputState` into `aurora::touch::IosTouchState`, and currently presents
explicit "not available" alerts for engine actions that have not yet been
wired. It must remain minimal; new menu behavior belongs behind this bridge,
not in a restyled overlay.

## Feature parity and wiring

| Surface | UI parity | Engine wiring |
| --- | --- | --- |
| Visible GameCube controls and `•••` menu | Ported directly; Simulator rendering verified | Mixer → Aurora virtual GC pad is implemented; current host Simulator clicks are not delivering touches, so current-build interaction is not re-verified |
| Render Resolution | SunPad control present | Stub; must drive Dawn/Metaforce render scale |
| Aspect Ratio | SunPad control present | Stub; must drive game presentation |
| FPS counter | SunPad control present | Settings persist; engine FPS source/display wiring remains |
| Touch Control Settings | SunPad panel present, including opacity, global/per-control size, hide-on-controller, modern C-stick | Settings and layout state persist through SunPad code; current-build interaction awaits touch delivery/device verification |
| Layout editor | SunPad drag, per-control resize, and reset code present | Current-build interaction awaits touch delivery/device verification |
| Game Data & Saves | SunPad menu structure present | Bridge alerts only; private validation/import/atomic activation remains |
| Share Diagnostic Log | SunPad menu and snapshot implementation present | Share-sheet action remains to be wired through the bridge |
| Controller Mapping | SunPad menu item present | Bridge alert only; mapping UI/engine action remains |

## Verification record

- Earlier 2026-08-12 evidence proved the ported `•••` menu visually:
  `/tmp/ios_sunpad_overlay2.png`, `/tmp/ios_menu_open2.png`.
- KI-015 was fixed on 2026-08-12. The iOS build had compiled both
  `ios_touch_stub.cpp` and `OverlayBridge.mm` into `libaurora_core.a`; because
  the stub appeared first in archive order, the static linker satisfied
  `attach_ios_overlay()` without loading the Objective-C++ bridge. CMake now
  compiles the stub only for non-iOS targets.
- The fixed current build entered the real bridge (`[SunPad] session start`),
  rendered the controls over the live intro, reached Dawn/Metal and gameplay,
  and terminated cleanly. Evidence:
  `/tmp/ki015-overlay-fixed-2026-08-12.png` and
  `/tmp/ki015-overlay-fixed-2026-08-12.log`.
- Current host clicks in the Simulator device area still did not open the menu.
  This is recorded as a test-infrastructure limitation, not as proof that the
  app-side mixer or menu interaction is broken. Physical-device verification
  remains required.
