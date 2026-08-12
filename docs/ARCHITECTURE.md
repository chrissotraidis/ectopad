# Architecture

Last updated: 2026-08-12

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
3. **The operative Dawn pin** in Metaforce's Aurora `5143394` is
   `v20260603.191052`, and **prebuilt packages exist for `darwin-arm64` and `ios-arm64`**,
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
   - **Keyboard/mouse (local patch):** upstream disabled the kbm path
     (`#if 0` in `CInputGenerator::Update`; the kbm `CFinalInput` constructor was
     unfinished). We implemented it: CMain sources SDL key/mouse/text events into
     `CKeyboardMouseControllerData` (`g_kbmData`), and `CInputGenerator` pushes a
     kbm `CFinalInput` (WASD/IJKL sticks, J/K/I/U/H/Q/E buttons, Enter=Start,
     arrows=D-pad) whenever no gamepad is connected. Mapping constants live in
     `CFinalInput.cpp`; easy to remap for the Prime-native control design.
8. **Saves:** `CMemoryCardSys*` uses CARD-compatible kabufuda storage. macOS
   save/reload and mobile save-preserving game-data removal/reimport are proven.
9. **Android exists in-tree** (`android/` dir, presets) but is not our target; we
   reuse its toolchain patterns where useful and do not assume its assumptions
   hold on iOS.
10. **Audio is restored locally:** Metaforce drives amuse SFX/sequencing and
    streamed DSP/MIDI into Aurora's SDL3 output through soxr voice resampling.
    The producer measures converted output-ready depth and tops a 120 ms reserve
    independently of render FPS. Physical routes/interruptions still need proof.
11. **Disc access is Rust-backed:** nod (Rust, via corrosion + nod-ffi) reads the
    ISO; the iOS build targets `aarch64-apple-ios` (rustup target added
    2026-08-11).
12. **Mobile presentation is SunPad-owned:** unchanged UIKit overlay/settings/
    mixer/mapping sources sit above SDL's UIKit/Metal hierarchy through a thin
    `OverlayBridge.mm`. iOS desktop ImGui presentation is suppressed. The bridge
    also delegates private atomic game-data import, diagnostics, and display
    settings while keeping SunPad UI source unchanged.

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
