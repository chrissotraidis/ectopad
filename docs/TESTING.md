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

### 2026-08-11 — macOS ARM64 (Gate 1)

- Build: `cmake --preset macos-default-relwithdebinfo` +
  `cmake --build --preset macos-default-relwithdebinfo` → 862/862 steps OK,
  `Binaries/Metaforce.app` (arm64 Mach-O, Metal.framework weak-linked, 25 MB).
- Boot (no dev console): `Metaforce -l <iso>` — Metal selected, 60.1 FPS,
  disc identified, assets loaded, no errors.
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

To be appended with dated entries for later phases.
