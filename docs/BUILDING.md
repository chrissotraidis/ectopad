# Building

Last updated: 2026-08-11

## Prerequisites (macOS)

- Xcode (26.6 installed) with command-line tools
- CMake ≥ 3.25 (3.27.1 installed), Ninja (1.13.2)
- Python 3 + `markupsafe` (2.1.5 installed)
- SDL3 (Homebrew `sdl3` 3.4.12 installed; aurora uses it via `system` provider)

## macOS (Apple Silicon)

```sh
cd ref/metaforce
cmake --preset macos-default-relwithdebinfo
cmake --build --preset macos-default-relwithdebinfo
```

Expected provider resolution on this machine:

- Dawn: `package` (prebuilt `dawn-darwin-arm64.tar.gz` at the pinned version)
- SDL3: `system` (Homebrew 3.4.12)
- nod, kabufuda, zeus, musyx, etc.: from in-tree submodules

Artifacts land in `ref/metaforce/build/` (git-ignored via `ref/`).

## iOS / iPadOS (device, arm64)

```sh
cd ref/metaforce
cmake --preset ios-default
cmake --build --preset ios-default --target install
```

Upstream CI does exactly this on macOS runners (`x-ios-ci` preset) and produces
`build/install/Metaforce.app` for arm64 iOS. Local reproduction is Gate 2 work.

## Notes / known environment quirks

- The project README says builds are "currently unavailable while the project
  undergoes large changes," but upstream CI builds all platforms; treat README
  statements as stale unless reproduced.
- Rust toolchain is present (1.97.1) with only `aarch64-apple-darwin`; the iOS
  preset references `Rust_CARGO_TARGET=aarch64-apple-ios` — verify whether any
  Rust component is actually linked before adding the target.

