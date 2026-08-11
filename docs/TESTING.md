# Testing

Last updated: 2026-08-11

## Discipline

- Run only **one** iOS Simulator at a time.
- Record dated, command-level evidence for every claim.
- Never classify a Simulator-only failure as a device blocker without testing on
  the device architecture.
- A title screen is not completion; a rendered room is not completion; Frigate
  Orpheon alone is not completion.

## Test matrix (tracked here as executed)

| Target | Launch | Import UI | Rendering | Touch | Controller | Gameplay | Saves | Audio | Lifecycle |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| macOS ARM64 | ☐ | — | ☐ | — | ☐ | ☐ | ☐ | ☐ | ☐ |
| iPhone Simulator | ☐ | ☐ | ☐ | ☐ | — | ☐ | ☐ | ☐ | ☐ |
| iPad Simulator | ☐ | ☐ | ☐ | ☐ | — | ☐ | ☐ | ☐ | ☐ |
| Physical iPhone | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ |
| Physical iPad | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ | ☐ |

## Gameplay verification scope (when reachable)

- Frigate Orpheon opening sequence (Gate 3): movement, camera, aiming, lock-on,
  Morph Ball, beam firing, missiles, jumping, scanning, menus, pause,
  save/reload, lifecycle recovery, memory headroom.
- Representative later areas: Chozo Ruins, Tallon Overworld, Phendrana Drifts,
  Magmoor Caverns, Phazon Mines.

## Evidence ledger

To be appended with dated entries (commands, logs, screenshots, video).

