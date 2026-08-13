# Ectopad — Metroid Prime for Apple platforms

Native reimplementation of Metroid Prime (GameCube, NTSC-U, Rev 2) for Apple
Silicon macOS, iOS, and iPadOS, built on [Metaforce](https://github.com/AxioDL/metaforce).

## What this is

- A **native Apple port** of the Metaforce engine (a clean-room reimplementation of
  the Metroid Prime engine). The engine executes on Apple hardware; it is **not** a
  Dolphin/RetroArch-style emulator frontend.
- Rendering flows `Metaforce → Aurora (GX on WebGPU) → Dawn → Metal` on Apple
  platforms, and `Dawn → D3D12/Vulkan` elsewhere.
- Requires the user's **own, legally obtained** Metroid Prime (USA, Rev 2) disc
  image. No Nintendo game data is included in, or committed to, this repository.
- Not affiliated with or endorsed by Nintendo.

## Repository layout

- `docs/` — status, architecture, dependencies, build, game-data, testing,
  performance, known issues, legal/provenance, and handoff documentation.
- `ref/` — **git-ignored**; contains the user's game dump and public reference
  repositories (metaforce, sunpad, PrimeDecomp/prime, aurora). Nothing in `ref/`
  is ever committed.

Start with [docs/TECH-DEBT.md](docs/TECH-DEBT.md) for the current physical-iPad
defect queue. [docs/CURRENT_STATE.md](docs/CURRENT_STATE.md) is the audited
handoff, [docs/STATUS.md](docs/STATUS.md) is the evidence ledger, and
[docs/IMPLEMENTATION_PLAN.md](docs/IMPLEMENTATION_PLAN.md) is the remaining
gate plan.

The attached iPad now runs a development build named **EctoPad**. Both default
memory-card images are complete on device; the SunPad-native `•••` menu and
single-pass Prime file menu are physically accepted. Audio transition listening,
save/reload, the first-door visual fix, and named distance/texture defects remain
open. Physical install signs
`ref/metaforce/build/ios-default/Binaries/Metaforce.app` and uses
`devicectl` in place. The unsigned IPA is validation-only. See
[docs/BUILDING.md](docs/BUILDING.md) and
[docs/INSTALL_IPA.md](docs/INSTALL_IPA.md).
