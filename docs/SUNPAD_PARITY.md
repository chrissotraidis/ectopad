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
| `apple/shared/SunPadControllerMapping.h` | `lib/ios/SunPadControllerMapping.h` | Byte-identical |
| `apple/shared/SunPadControllerMapping.mm` | `lib/ios/SunPadControllerMapping.mm` | Byte-identical |
| `apple/shared/SunPadDiagnostics.h` | `lib/ios/SunPadDiagnostics.h` | Byte-identical |
| `apple/shared/SunPadDiagnostics.mm` | `lib/ios/SunPadDiagnostics.mm` | One intentional app-name change: the persistent log root is `Metaforce/Logs` instead of `SunPad/Logs` |

`OverlayBridge.mm` is Metaforce-specific glue. It attaches the unchanged
SunPad view above SDL's UIKit/Metal view, forwards the mixer's
`SunPadInputState` into `aurora::touch::IosTouchState`, applies SunPad's render
scale/aspect settings to Aurora, displays successful surface-present rate when
SunPad's FPS switch is enabled, and delegates game-data operations to a private
import service. It presents an explicit "not available" alert for controller
mapping. It must remain minimal; new menu
behavior belongs behind this bridge, not in a restyled overlay.

## Feature parity and wiring

| Surface | UI parity | Engine wiring |
| --- | --- | --- |
| Visible GameCube controls and `•••` menu | Ported directly; Simulator rendering verified | Mixer → Aurora virtual GC pad is implemented; current host Simulator clicks are not delivering touches, so current-build interaction is not re-verified |
| Render Resolution | SunPad control present | Wired live to Aurora's framebuffer scale; 2× produced 1280×960 in Simulator |
| Aspect Ratio | SunPad control present | Original 4:3, experimental 16:9, and native-surface fill drive framebuffer presentation; 16:9 produced 1707×960 at 2× in Simulator |
| FPS counter | SunPad control present | Wired to successful Dawn surface presents and shown with SunPad's label styling; Simulator rendering verified |
| Touch Control Settings | SunPad panel present, including opacity, global/per-control size, hide-on-controller, modern C-stick | Settings and layout state persist through SunPad code; current-build interaction awaits touch delivery/device verification |
| Layout editor | SunPad drag, per-control resize, and reset code present | Current-build interaction awaits touch delivery/device verification |
| Game Data & Saves | SunPad menu structure unchanged | Files picker/folder import, exact GM8E01 Rev 2 validation, private staging, atomic activation, and save-preserving removal implemented; service path Simulator-verified, picker interaction awaits touch/device |
| Share Diagnostic Log | SunPad menu, privacy confirmation, snapshot, and share sheet are ported directly | Implementation is already self-contained in the overlay; current-build interaction still awaits working touch delivery/device verification |
| Controller Mapping | SunPad menu item and mapping model ported directly | A/B/X/Y/right-shoulder ↔ GameCube A/B/X/Y/Z permutation persists and applies in the iOS SDL→PAD path; Simulator UI and deterministic swap/passthrough test verified; physical controller remains untested |

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
- The real engine side of three display actions was verified independently of
  host touch delivery by changing the same persisted SunPad settings before
  launch. The app logged `renderScale=2 aspectMode=0` with framebuffer
  `1280x960`, then `renderScale=2 aspectMode=1` with framebuffer `1707x960`;
  both runs retained the unchanged touch overlay and showed the presented-frame
  FPS label. Evidence: `/tmp/sunpad-menu-wiring-2x-fps-2026-08-12.png` and
  `/tmp/sunpad-menu-wiring-16x9-2026-08-12.png`.
- The 20.4/59.9 values visible in those screenshots are not performance
  baselines: another GPU/CPU-heavy app materially affected the first run. See
  [PERFORMANCE.md](PERFORMANCE.md) for the required clean-run discipline.
- The unchanged Game Data & Saves actions now open a Files picker, import from
  the app's Files-visible Documents folder, or remove only stored game data.
  Invalid-size and corrupt-SHA imports left the active image and save untouched;
  valid import/reimport and removal were verified through the same service used
  by the delegate. A normal restart booted the activated image. Evidence:
  `/tmp/metaforce-import-atomic-2026-08-12.log` and [GAME_DATA.md](GAME_DATA.md).
- Controller mapping now uses SunPad's byte-identical mapping store and the same
  five-button swap UI. The Simulator displayed the dialog over the unchanged
  controls and a test restored prior defaults after verifying default mapping,
  persisted A/B swap, and Start/L passthrough. Evidence:
  `/tmp/ios-controller-mapping-ui-2026-08-12.png` and
  `/tmp/ios-controller-mapping-2026-08-12.log`.
- An SDL virtual controller then proved the runtime boundary itself: under an
  opt-in A/B swap, PAD logged raw physical B `0x0200` as mapped GameCube A
  `0x0100`, then neutral on release. Production reads an atomic cached mapping;
  `NSUserDefaults` is not consulted per frame.
