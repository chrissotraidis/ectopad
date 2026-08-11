# Dependencies

Last updated: 2026-08-11

## Reference repositories (cloned into `ref/`)

`ref/` is git-ignored; these are separate clones used as source pins and research
references.

| Repository | URL | Pinned revision | Purpose |
| --- | --- | --- | --- |
| metaforce | https://github.com/AxioDL/metaforce | `621ee0f` ("Update aurora; impl visors, warp & water (WIP)") | Native Metroid Prime engine (primary) |
| aurora (submodule pin) | https://github.com/encounter/aurora | `514339438178ef2bed1b14e5149d90ece0c6e0cc` | GX compatibility layer on WebGPU/Dawn; app/input/DVD/CARD layers (SDL3) |
| aurora (standalone tip) | https://github.com/encounter/aurora | `59c2b97` ("Refactor gfx/common into modules") | Standalone clone; note tip differs from metaforce's pin |
| prime | https://github.com/PrimeDecomp/prime | `72e31c7` ("CTryclops") | Matching decompilation — research/accuracy reference only, **not** a build dependency |
| sunpad | https://github.com/chrissotraidis/sunpad | `7d84cec` | Apple application-shell reference (UX, packaging, input, diagnostics, testing discipline) |

## metaforce submodules (gitlink pins at `621ee0f`)

| Submodule | Pinned SHA | Notes |
| --- | --- | --- |
| `extern/aurora` | `514339438178ef2bed1b14e5149d90ece0c6e0cc` | Encounter/aurora; graphics + app layer |
| `extern/fixNES` | `156fcaca9f4cd9c23d423737994c45cfb05d16ca` | NES emulation component |
| `extern/jbus` | `7932b0a9b99e3461489b29ba3433f4c4256eb0f6` | CPU/JIT? (verify) |
| `extern/kabufuda` | `9493afc8ff961ee04193038046308411904b8137` | Memory-card (CARD) support |
| `extern/libjpeg-turbo` | `3d2da99c69d000c6351f6d2390b694f800e5ef4d` | JPEG decode (`heads/thp` branch) |
| `extern/musyx` | `37e8ecd4e6503e90b97ea81cd7d669357861d501` | GameCube audio library reimplementation |
| `extern/nod` | `0ab72ade73a3aacf967741daee0bd088271f2075` | Disc image reading (encounter/nod) |
| `extern/spdlog` | `48bcf39a661a13be22666ac64db8a7f886f2637e` | Logging |
| `extern/zeus` | `d64e9f502a07c808596c5112bfd46bf0a0d8138e` | SIMD math library (AxioDL/zeus) |

## aurora dependency versions (pinned in `AuroraDependencyVersions.cmake`)

| Dependency | Pinned version/ref | Provider options |
| --- | --- | --- |
| Dawn (WebGPU) | operative pin in metaforce's aurora submodule (`5143394`): version `v20260603.191052`, prebuilt from `encounter/dawn-build` releases. (Newer standalone aurora defaults to `v20260807.225922` from `encounter/dawn`.) | auto / vendor / system / package |
| SDL3 | `3.4.10` (`refs/tags/release-3.4.10`) | auto / vendor / system / package |
| nod | `v2.0.0-alpha.10` | (bundled) |

Prebuilt Dawn packages exist for: `windows-{amd64,arm64}`, `linux-{x86_64,aarch64}`,
`darwin-{arm64,x86_64}`, `ios-arm64`, `android-aarch64` — including the two we need
for Gate 1 (macOS ARM64) and Gate 2 (iOS device).

Build-time note (2026-08-11, macOS): `AURORA_DAWN_PROVIDER=package` resolved to the
prebuilt Dawn package; `AURORA_SDL3_PROVIDER=system` resolved to Homebrew SDL3 3.4.12
and is linked **shared** (aurora warns that static SDL3 is unavailable via Homebrew).

## Build environment (this machine, 2026-08-11)

- Apple M2 (8 cores), 24 GB RAM, macOS 26.5 (Build 25F71)
- Xcode 26.6 (Build 17F113); SDKs: macosx 26.5, iphoneos 26.5, iphonesimulator 26.5
- CMake 3.27.1, Ninja 1.13.2, AppleClang 21.0.0, git 2.41.0
- Python 3.8.10 (+ markupsafe 2.1.5)
- Rust 1.97.1 (aarch64-apple-darwin installed; iOS target not yet added)
- Homebrew SDL3 3.4.12 (used via aurora `system` provider)

## Licenses

| Component | License |
| --- | --- |
| metaforce | See `ref/metaforce/LICENSE` |
| aurora | See `ref/aurora/LICENSE` |
| nod | MIT (encounter/nod) |
| zeus | GPL-3.0 (AxioDL/zeus) |
| spdlog | MIT |
| libjpeg-turbo | BSD-3 / IJG |
| SDL3 | Zlib |
| Dawn | Apache-2.0 / BSD-3 (Chromium) |
| ios-cmake (`ios.toolchain.cmake`) | BSD-3 |

Full texts live inside each repository. Verify before any redistribution.
