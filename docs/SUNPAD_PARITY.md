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
| `apple/shared/SunPadDiagnostics.h` | `lib/ios/SunPadDiagnostics.h` | App-name-only comment changes |
| `apple/shared/SunPadDiagnostics.mm` | `lib/ios/SunPadDiagnostics.mm` | Two intentional app-name changes: persistent root `Metaforce/Logs` and shared filename `Metaforce-Diagnostic-*` |

`OverlayBridge.mm` is Metaforce-specific glue. It attaches the unchanged
SunPad view above SDL's UIKit/Metal view, forwards the mixer's
`SunPadInputState` into `aurora::touch::IosTouchState`, applies SunPad's render
scale/aspect settings to Aurora, displays successful surface-present rate when
SunPad's FPS switch is enabled, and delegates game-data operations to a private
import service. Controller mapping now uses SunPad's byte-identical model and
UI. It must remain minimal; new menu
behavior belongs behind this bridge, not in a restyled overlay.

## Feature parity and wiring

| Surface | UI parity | Engine wiring |
| --- | --- | --- |
| Visible GameCube controls and `•••` menu | Ported directly; Simulator rendering verified | Current-build harness found 19 menu titles and all 14 controls, drove A through the real mixer, and reported the menu visible/attached; physical finger input remains untested |
| Render Resolution | SunPad control present | Wired live to Aurora's framebuffer scale; 2× produced 1280×960 in Simulator |
| Aspect Ratio | SunPad control present | Original 4:3, experimental 16:9, and native-surface fill drive framebuffer presentation; 16:9 produced 1707×960 at 2× in Simulator |
| FPS counter | SunPad control present | Wired to successful Dawn surface presents and shown with SunPad's label styling; Simulator rendering verified |
| Touch Control Settings | SunPad panel present, including opacity, global/per-control size, hide-on-controller, modern C-stick | Current-build harness exercised the real controls/actions and verified persisted render, opacity, global size, hide, and modern-C settings; physical touch remains |
| Layout editor | SunPad drag, per-control resize, and reset code present | Current-build harness selected A, invoked the registered per-control resize action, verified 1.25×, reset, and restored defaults; drag ergonomics still require real touch |
| Game Data & Saves | SunPad menu structure unchanged | Files picker/folder import, exact GM8E01 Rev 2 validation, private staging, atomic activation, and save-preserving removal implemented; service path Simulator-verified and current harness presents picker/delegate; physical selection/import remains |
| Share Diagnostic Log | SunPad menu, privacy confirmation, snapshot, and share sheet are ported directly | iPhone Simulator presented confirmation and real UIKit share sheet; privacy test proved app-container/temp redaction and Metaforce filename; physical interaction remains |
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
- Current host clicks in the Simulator device area remain unreliable. A guarded
  launch-only harness now exercises the unchanged SunPad UIKit targets and
  current Metaforce delegates without editing any SunPad source. On 2026-08-12
  it returned `result=0`: 19 menu titles, 14 controls, A down/up through the
  real mixer, settings, per-control editor resize/reset, empty-folder alert,
  and Files picker/delegate, followed by preference restoration. The live
  bridge also logged `menu visible=1 attached=1`. Evidence:
  `/tmp/ipad-menu-audio-focused-2026-08-12.{png,log}`. This is deterministic
  current-build wiring evidence, not a substitute for finger-on-glass proof.
- SDL can update its UIKit/Metal hierarchy after initial attachment. The
  Metaforce bridge now reasserts the whole unchanged SunPad overlay at window
  front once per second on the main thread, keeping controls and `•••` above
  live gameplay without changing SunPad layout or styling.
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
- On an iPhone 17 Pro Simulator, the unchanged overlay presented SunPad's
  diagnostic confirmation and the real UIKit share sheet in landscape without
  clipping. The shared text document was named `Metaforce-Diagnostic-*`; a
  service test verified current home/temp paths were replaced with
  `<app-container>`/`<temporary>/`. Evidence:
  `/tmp/iphone-diagnostics-confirmation-final-landscape-2026-08-12.png`,
  `/tmp/iphone-diagnostics-share-landscape-2026-08-12.png`, and
  `/tmp/iphone-diagnostics-2026-08-12.log`.
- A holistic iPhone review caught Metaforce's desktop ImGui Game/Tools/Overlays/
  Help bar, pre-launch About window, and controller toast rendering underneath
  the UIKit controls. iOS now keeps ImGui's internal state maintenance but
  suppresses all desktop console presentation; macOS remains unchanged. Clean
  SunPad-only evidence:
  `/tmp/iphone-sunpad-clean-mobile-ui-landscape-2026-08-12.png` and
  `/tmp/ipad-sunpad-clean-mobile-ui-2026-08-12.png`.
