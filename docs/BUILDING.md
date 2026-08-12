# Building

Last updated: 2026-08-12

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
cmake --build --preset ios-default --target metaforce -j8
```

The local artifact is `build/ios-default/Binaries/Metaforce.app`. A full arm64
iOS compile and link completed on 2026-08-12. Cross-compiling must not import
host libraries through pkg-config; Aurora skips that fallback and vendors a
static zstd for iOS. Signing/install/launch still requires the user's identity
and physical hardware.

## iOS Simulator (arm64)

```sh
# 1. Host protoc (Dawn's codegen tool must run on the host, not the sim target):
#    build the vendored protobuf 33's protoc for macOS (see patches/ for the
#    wrapper recipe), then configure with -DWITH_PROTOC=<host protoc>.
# 2. Disable GLFW (macOS-only dependency): -DDAWN_USE_GLFW=OFF
cmake -B build/ios-sim -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake \
  -DPLATFORM=SIMULATORARM64 -DDEPLOYMENT_TARGET=14.0 \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_DISABLE_FIND_PACKAGE_BZip2=TRUE \
  -DCMAKE_DISABLE_FIND_PACKAGE_LibLZMA=TRUE \
  -DCMAKE_DISABLE_FIND_PACKAGE_zstd=TRUE \
  -DCMAKE_DISABLE_FIND_PACKAGE_PkgConfig=TRUE \
  -DAURORA_DAWN_PROVIDER=vendor -DAURORA_SDL3_PROVIDER=vendor \
  -DRust_CARGO_TARGET=aarch64-apple-ios-sim \
  -DWITH_PROTOC=/tmp/protoc-host/protobuf-build/protoc \
  -DDAWN_USE_GLFW=OFF
cmake --build build/ios-sim --target install
```

Then: `simctl boot <iPad>` → `simctl install <udid>
build/ios-sim/Binaries/Metaforce.app` → `simctl launch`. Production users import
through the unchanged SunPad Game Data & Saves menu; manual container copying is
only a development fallback. Run only one Simulator and shut it down afterward.

This was proven 2026-08-11 on iPad Pro 13-inch (M5), iOS 26.5 (title screen
rendered). One simulator at a time.

Audio note: the Simulator build was rebuilt 2026-08-11 (21:20) with the vendored
audio stack (amuse/athena/logvisor/soxr in `extern/`, added unconditionally by
`extern/CMakeLists.txt`); it compiles and links for the sim SDK and runs with
audio (amuse 32 kHz SDL backend, frontend RSF music, 2 voices, stable pump) —
see STATUS.md and KNOWN_ISSUES KI-003.

## Notes / known environment quirks

- The project README says builds are "currently unavailable while the project
  undergoes large changes," but upstream CI builds all platforms; treat README
  statements as stale unless reproduced.
- Rust toolchain is present (1.97.1) with only `aarch64-apple-darwin`; the iOS
  preset references `Rust_CARGO_TARGET=aarch64-apple-ios` — verify whether any
  Rust component is actually linked before adding the target.
