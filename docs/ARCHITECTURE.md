# Architecture

Last updated: 2026-08-11

## Current Metaforce architecture (pinned `621ee0f`)

```
Metaforce Runtime (game reimplementation)
   │
   ├── aurora::core / aurora::gx / aurora::pad / aurora::si / aurora::vi / aurora::mtx
   │        (Aurora compatibility layer, SDL3-based)
   │
   ├── GX compatibility layer (aurora::gx) → WebGPU (Dawn)
   │        backends: Apple → Metal; Windows → D3D12/D3D11/Vulkan; Linux → Vulkan/GL; Android → Vulkan/GLES
   │
   ├── musyx (GameCube audio reimplementation)
   ├── nod (disc image reading)
   ├── kabufuda (memory card / CARD)
   ├── zeus (SIMD math), spdlog (logging), libjpeg-turbo (JPEG), fixNES/jbus
   └── SDL3 (app shell, windowing, input, audio device)
```

Key findings (all verified in the cloned tree, 2026-08-11):

1. **Graphics moved into Aurora.** The old direct Dawn integration is gone.
   `Runtime/Graphics/CGX.cpp` calls the GX API provided by `aurora::gx`
   (`GXSetLineWidth`, `GXClearVtxDesc`, …). Aurora implements GX on WebGPU/Dawn.
2. **Dawn backends on Apple are Metal-only.** `AuroraDawnProvider.cmake` sets
   `DAWN_ENABLE_METAL ON` and D3D12/D3D11/Vulkan/GL off on `APPLE`. This is the
   `Metaforce → Aurora (GX) → Dawn → Metal` path the project requires.
3. **Dawn is pinned** to encounter/dawn `v20260807.225922` / ref
   `1155e0ed…`, and **prebuilt packages exist for `darwin-arm64` and `ios-arm64`**,
   so neither Gate 1 nor Gate 2 needs a Dawn source build by default
   (`AURORA_DAWN_PROVIDER=package`).
4. **App layer is SDL3** (aurora::main): windowing, input, audio on Windows, Linux,
   macOS, iOS, tvOS, Android. Aurora documents iOS/tvOS support.
5. **`ios.toolchain.cmake`** is the leetal/ios-cmake toolchain (BSD-3). Preset
   `ios-default` uses `PLATFORM=OS64`, `DEPLOYMENT_TARGET=14.0`, bitcode off.
6. **Platform app shells:** `Runtime/platforms/{macos,ios,tvos,win,freedesktop}`
   with Info.plist templates and icons. Upstream CI produces `Metaforce.app` for
   macOS and iOS.
7. **Input abstraction:** Retro-style `Runtime/Input` + aurora PAD layer over
   `SDL_Gamepad`; GameCube controller semantics (see IMPLEMENTATION_PLAN for the
   Prime-native control design work).
8. **Saves:** `CMemoryCardSys*` (OSX/Nix/Win variants) — CARD-compatible save
   handling via kabufuda; layout to be documented once exercised.
9. **Android exists in-tree** (`android/` dir, presets) but is not our target; we
   reuse its toolchain patterns where useful and do not assume its assumptions
   hold on iOS.
10. **Audio is mid-refactor upstream:** the boo voice engine and amuse/musyx
    playback calls are commented out in the pinned tree; no audio output device
    code exists yet (KI-003). The port will need to (re)wire output — SDL3 audio
    via aurora (macOS + iOS), or CoreAudio/AVAudioSession directly.
11. **Disc access is Rust-backed:** nod (Rust, via corrosion + nod-ffi) reads the
    ISO; the iOS build targets `aarch64-apple-ios` (rustup target added
    2026-08-11).

## Target Apple rendering path

`Metaforce → aurora::gx (GX on WebGPU) → Dawn → Metal`

Confirm empirically on macOS (Gate 1) and iOS device (Gate 2); see
[TESTING.md](TESTING.md) and diagnostics work in [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md).

## Non-goals / explicit decisions

- No Dolphin/RetroArch-style emulator frontend wrapping the game.
- No transplant of SunPad's DolRecomp/static-recompilation architecture; SunPad
  informs the Apple app shell, UX, packaging, input, diagnostics, game-data
  handling, testing discipline, and release boundaries only.
- PrimeDecomp/prime is a research/accuracy reference, not a build dependency.
