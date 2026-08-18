# Building

Last updated: 2026-08-12

## Prerequisites (macOS)

- Xcode (26.6 installed) with command-line tools
- CMake ≥ 3.25 (3.27.1 installed), Ninja (1.13.2)
- Python 3 + `markupsafe` (2.1.5 installed)
- SDL3 (Homebrew `sdl3` 3.4.12 installed; aurora uses it via `system` provider)
- Rust 1.97.1 plus the Apple targets used by the proven macOS, Simulator, and
  iPhoneOS builds
- ignored `ref/` checkouts at the pins in [DEPENDENCIES.md](DEPENDENCIES.md),
  with the local `patches/` sequence applied; there is not yet an EctoPad
  bootstrap script for this
- for a physical iPad: an Apple Development identity, the attached device, and
  in-place `devicectl` install of the signed source `.app`

The first `ios-default` configure also pulls Dawn and the vendored audio stack.
That is expected and is part of why the first device build felt heavier than
the README suggested. See [TECH-DEBT.md](TECH-DEBT.md) P2.

## macOS (Apple Silicon)

From the EctoPad repository root, the maintained wrapper configures, builds,
checks the arm64 app bundle, and runs the five focused controller slot tests:

```sh
./scripts/build-macos.sh
./scripts/run-macos.sh /path/to/your/game.iso
```

The launcher accepts additional Metaforce options after the disc path. It
validates the supported USA Rev 2 image locally and never copies or packages
it. Connect a standard SDL3-compatible controller before or after launch.
Without a controller, click the game window to capture the mouse; Escape
releases it, and a second Escape opens the pause/back action. The desktop
defaults are WASD movement, persistent mouse look, Space to jump, left click to
fire, right click for Spider Ball where available, Shift to lock on/target, F
for missiles or Power Bombs, C for Morph Ball, Q/E/Z/X for
Combat/Scan/Thermal/X-Ray visors, 1-4 for
beams, Tab for the map, arrow keys for menus/map navigation, and Enter for
Start/confirm. Controller input owns player one while connected; mouse and
keyboard resume when it disconnects.

The equivalent underlying build commands are:

```sh
cd ref/metaforce
cmake --preset macos-default-relwithdebinfo
cmake --build --preset macos-default-relwithdebinfo --target metaforce controller_slots_tests
```

Expected provider resolution on this machine:

- Dawn: `package` (prebuilt `dawn-darwin-arm64.tar.gz` at the pinned version)
- SDL3: `system` (Homebrew 3.4.12)
- nod, kabufuda, zeus, musyx, etc.: from in-tree submodules

Artifacts land in `ref/metaforce/build/` (git-ignored via `ref/`). The native
artifact is
`ref/metaforce/build/macos-default-relwithdebinfo/Binaries/EctoPad.app`. Its
bundle icon and SDL runtime/window icon are both generated from the tracked
`assets/app-icon/EctoPad-AppIcon-1024.png` master.

## iOS / iPadOS (device, arm64)

After bootstrapping or refreshing the ignored `ref/metaforce` checkout, sync
the tracked original EctoPad icon before configuring or rebuilding:

```sh
./scripts/sync-app-icon.sh
```

This copies the 120×120 iPhone and 152×152 iPad icon resources and recompiles
the iOS `Assets.car` from the tracked 1024×1024 master in `assets/app-icon/`.
The provenance remains tracked in the root repository; no Nintendo or extracted
game artwork is used.

```sh
cd ref/metaforce
cmake --preset ios-default
cmake --build --preset ios-default --target metaforce -j8
```

The local artifact is `build/ios-default/Binaries/Metaforce.app`. A full arm64
iOS compile and link completed again with the desktop-input changes on
2026-08-17; keyboard/mouse input generation is excluded on iOS/tvOS.
Cross-compiling must not import
host libraries through pkg-config; Aurora skips that fallback and vendors a
static zstd for iOS. A later same-day session development-signed this source
`.app`, installed it in place, and launched it on the attached iPad. That is
not Gate 3 acceptance; see [TECH-DEBT.md](TECH-DEBT.md).

There is not yet an EctoPad `scripts/deploy-ios-device.sh`. Until one exists,
the honest device path is: build `ios-default`, sign the source `.app` with
the local Apple Development identity, `codesign --verify --deep --strict`,
`xcrun devicectl device install app` in place, then
`xcrun devicectl device process launch --terminate-existing com.axiodl.Metaforce`.
Preserve the live container. Back up `Documents` and `Library` separately.
Do not use `--remove-existing-content`. `ref/sunpad/scripts/` is the local
reference for bootstrap/provision/deploy shape.

### Unsigned validation IPA

No IPA is published or tracked by EctoPad. The command below creates temporary
local audit output only. Do not upload or share it as a release.

After the device app builds, create a deterministic local validation archive:

```sh
./scripts/package-ios.sh \
  ref/metaforce/build/ios-default/Binaries/Metaforce.app \
  /tmp/Metaforce-unsigned-validation.ipa
```

The script works on a temporary copy, removes any signature/provisioning
material, strips debug symbols, adds the project license/release notices,
normalizes timestamps, and runs `scripts/audit-ios-package.sh`. It never
modifies the source `.app`. This output is deliberately unsigned and is not a
physical-device or public-release artifact. Signing must happen only after all
bundle mutations in a separate device workflow; public distribution also
requires the GPL corresponding-source and LGPL relink materials described in
[LEGAL_AND_PROVENANCE.md](LEGAL_AND_PROVENANCE.md).

For historical reproducibility, the twice-reproduced 2026-08-12 archive was
`/tmp/Metaforce-unsigned-menu-audio-2026-08-12.ipa`, SHA-256
`308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`.
The package audit passed. That file is not a current public artifact. Rebuilding
it is unnecessary for physical testing;
sign the source device app using a valid development workflow instead.

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
see STATUS.md and KNOWN_ISSUES KI-003. The final 2026-08-12 build replaces the
render-frame-coupled pump with a measured 120 ms converted-output reserve.

## Notes / known environment quirks

- The project README says builds are "currently unavailable while the project
  undergoes large changes," but upstream CI builds all platforms; treat README
  statements as stale unless reproduced.
- Rust toolchain 1.97.1 and the required Apple targets produced the final
  Simulator and iPhoneOS builds. Do not change toolchains absent a reproduced
  build defect.
- Do not repeat Simulator UI work by default. Hardware launch already
  happened. The next meaningful step is the [TECH-DEBT.md](TECH-DEBT.md)
  queue, not another unsigned IPA.
