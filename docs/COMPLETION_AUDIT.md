# Completion Audit

Last updated: 2026-08-13

This matrix preserves the full goal and distinguishes local implementation
progress from evidence that actually satisfies each release requirement.
[CURRENT_STATE.md](CURRENT_STATE.md) supplies the exact artifact identity and
ordered remaining checklist.

| Requirement | Current evidence | Verdict | Next proof required |
| --- | --- | --- | --- |
| Native Apple Silicon macOS Metaforce through Dawn/WebGPU → Metal | ARM64 build, Metal adapter, title, Frigate gameplay, audio, keyboard/mouse, software GameController path, save/reload | **Proven for Gate 1** | Controlled performance/memory and broader-area stability remain release work |
| Native arm64 iOS/iPadOS build | The final `Metaforce.app` links, is signed, strictly verified, installed in place, launched, and preserves the ISO plus both memory cards byte-for-byte | **Build and Gate 2 deployment proven; Gate 3 partial** | Close the remaining [TECH-DEBT.md](TECH-DEBT.md) physical audio, visual, lifecycle, save, and stability gates |
| Strict SunPad overlay/menu/touch parity | The live SunPad-native menu opens; nested render/aspect/FPS actions work; the separate Prime file menu uses its accepted single authored pane | **Physical menu acceptance proven** | Keep lifecycle return and dismissal-artifact regression checks open |
| Complete Prime touch-control surface | Wrong C-stick-as-look override removed; original GameCube mappings restored; left-stick movement accepted; 0.5 s L/R latch deployed | **Core touch movement accepted; full control matrix open** | Verify L lock-on/strafe, hold-R aim, C-stick beam selection, and lifecycle recovery together |
| GameController and mapping | SDL virtual controller proves add/axis/buttons; SunPad A/B/X/Y/Z mapping UI/store and iOS permutation are implemented; raw physical B `0x0200` mapped through live PAD as GameCube A `0x0100` under an opt-in swap | **Software path proven** | Physical Apple controller discovery, hot-plug/reconnect, rumble, mapping, and touch/controller handoff |
| Private user game-data handling | Exact Rev 2 validation, private staging, staged SHA-1, atomic rename, safe removal/reimport, save preservation | **Service proven in Simulator** | Production Files picker/folder interaction on working touch/device |
| Diagnostics and Share Diagnostic Log | SunPad persistent rotating log, confirmation, snapshot, and UIKit share sheet are ported; iPhone Simulator proved home/temp redaction and a Metaforce-named text snapshot | **Simulator UI/privacy proven** | Repeat share/export and inspect artifact on physical iPhone/iPad |
| Saves/configuration | macOS save/reload proven; physical A/B images now both read back as 16,777,216 bytes | **Card creation proven; physical save cycle open** | KI-016: New Game → save → relaunch → load |
| Lifecycle | iPad Simulator background/foreground and clean shutdown proven | **Simulator-only** | Physical interruption/background/foreground/termination and state recovery |
| Gameplay breadth and progression | Frigate first-person gameplay and selected warp rendering proven | **Incomplete** | Full Frigate control checklist and representative Chozo/Tallon/Phendrana/Magmoor/Phazon sessions |
| Performance, memory, thermals | FPS presentation works; lifecycle RSS spot-check exists; frame-rate-independent audio reserve held 120 ms with 6–7 voices and zero logged underruns in a loaded native run | **Incomplete** | Quiet-system baselines with load snapshots; frame times, RSS/headroom, thermal state, audible device audio, long sessions |
| iPhone layout | iPhone 17 Pro Simulator rendered the unchanged landscape controls plus diagnostic confirmation/share sheet without clipping | **Simulator presentation proven** | Exercise touch controls/editor/menu and gameplay, then repeat on physical iPhone |
| Packaging/signing | Unsigned IPA audit still stands; the final source `.app` was development-signed, strictly verified, installed in place, launched, and its data read back | **Physical in-place deploy proven; one-command path still absent** | Keep the unsigned IPA validation-only; add bootstrap/`deploy-ios-device.sh` so the next launch is a front-door path |
| Legal/prohibited-data boundary | `ref/` is ignored; packaging includes principal notices; the audit rejects game/save/log/signing extensions, disc headers, credentials, local paths, host dylibs, and a seeded `.gci` negative case | **Local artifact boundary proven; public redistribution blocked** | Assemble/review GPL corresponding source and LGPL static-relink materials; rebuild/remap upstream Dawn provenance strings; audit the final signed artifact |

## Current environment blockers

- The earlier same-day "zero identities / no device" notes are stale. A
  development-signed build launched on the attached iPad.
- Gate 3 is now blocked by product defects, not missing hardware. Default cards,
  the native menu, and core movement have progressed; transition audio,
  save/reload, lifecycle recovery, door/geometry presentation, and later-game
  stability remain. See [TECH-DEBT.md](TECH-DEBT.md).
- Simulator host touch delivery remains unreliable; deterministic test hooks
  still do not replace physical interaction proof.

The persistent goal must remain active until every row is proven at the scope
named by the objective. Simulator or build evidence must not be promoted to a
physical-device claim.
