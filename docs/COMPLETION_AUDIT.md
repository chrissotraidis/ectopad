# Completion Audit

Last updated: 2026-08-12

This matrix preserves the full goal and distinguishes local implementation
progress from evidence that actually satisfies each release requirement.

| Requirement | Current evidence | Verdict | Next proof required |
| --- | --- | --- | --- |
| Native Apple Silicon macOS Metaforce through Dawn/WebGPU → Metal | ARM64 build, Metal adapter, title, Frigate gameplay, audio, keyboard/mouse, software GameController path, save/reload | **Proven for Gate 1** | Controlled performance/memory and broader-area stability remain release work |
| Native arm64 iOS/iPadOS build | Unsigned arm64 `Metaforce.app` links successfully, including SunPad bridge/import/mapping | **Build proven** | Sign, install, launch, reach device Metal, and render on physical iPhone/iPad |
| Strict SunPad overlay/menu/touch parity | Overlay/settings/mixer/input/mapping sources byte-identical; diagnostics has app-name-only changes; desktop ImGui chrome is suppressed on iOS; clean SunPad-only iPhone/iPad screenshots exist | **Source/presentation parity proven; interaction partial** | Current-build touch/menu/editor tests on iPhone + iPad and physical devices |
| Complete Prime touch-control surface | All SunPad GameCube controls and layout editor are present; earlier touch moved Samus | **Partially proven** | Exercise movement, camera/aim, lock-on, Morph Ball, beams, missiles, jump, scan, menus, pause, and layout persistence |
| GameController and mapping | SDL virtual controller proves add/axis/buttons; SunPad A/B/X/Y/Z mapping UI/store and iOS permutation are implemented; raw physical B `0x0200` mapped through live PAD as GameCube A `0x0100` under an opt-in swap | **Software path proven** | Physical Apple controller discovery, hot-plug/reconnect, rumble, mapping, and touch/controller handoff |
| Private user game-data handling | Exact Rev 2 validation, private staging, staged SHA-1, atomic rename, safe removal/reimport, save preservation | **Service proven in Simulator** | Production Files picker/folder interaction on working touch/device |
| Diagnostics and Share Diagnostic Log | SunPad persistent rotating log, confirmation, snapshot, and UIKit share sheet are ported; iPhone Simulator proved home/temp redaction and a Metaforce-named text snapshot | **Simulator UI/privacy proven** | Repeat share/export and inspect artifact on physical iPhone/iPad |
| Saves/configuration | macOS save/reload and Simulator save preservation/import separation proven | **Partially proven** | In-game save station plus physical iOS/iPadOS save/reload and upgrade persistence |
| Lifecycle | iPad Simulator background/foreground and clean shutdown proven | **Simulator-only** | Physical interruption/background/foreground/termination and state recovery |
| Gameplay breadth and progression | Frigate first-person gameplay and selected warp rendering proven | **Incomplete** | Full Frigate control checklist and representative Chozo/Tallon/Phendrana/Magmoor/Phazon sessions |
| Performance, memory, thermals | FPS presentation works; lifecycle RSS spot-check exists | **Incomplete** | Quiet-system baselines with load snapshots; frame times, RSS/headroom, thermal state, long sessions |
| iPhone layout | iPhone 17 Pro Simulator rendered the unchanged landscape controls plus diagnostic confirmation/share sheet without clipping | **Simulator presentation proven** | Exercise touch controls/editor/menu and gameplay, then repeat on physical iPhone |
| Packaging/signing | Unsigned app bundle exists; no signing identity or device is available | **Incomplete; external prerequisite for device gates** | Development signing/install first, then reproducible audited IPA/app packaging |
| Legal/prohibited-data boundary | `ref/` is ignored; root commits contain patches/docs only | **Ongoing invariant** | Audit every final staged tree and distributable artifact for game data, saves, secrets, and signing material |

## Current environment blockers

- `security find-identity -p codesigning` reports zero valid identities.
- `xcrun devicectl list devices` reports no connected physical devices.
- Simulator host touch delivery is unreliable; deterministic test hooks may
  prove service/UI presentation but do not replace physical interaction proof.

The persistent goal must remain active until every row is proven at the scope
named by the objective. Simulator or build evidence must not be promoted to a
physical-device claim.
