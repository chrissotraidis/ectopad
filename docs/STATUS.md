# Status

Last updated: 2026-08-11

## Status legend

- **Proven** — verified with dated evidence on the named hardware.
- **Partially proven** — works but with caveats or incomplete verification.
- **Simulator-only** — verified in an iOS Simulator, not on physical hardware.
- **Physical-device verified** — verified on a named physical Apple device.
- **Blocked** — cannot proceed for a documented reason.
- **Not yet tested** — not attempted or not yet verified.

## Current state

| Item | Status | Evidence |
| --- | --- | --- |
| Game data validated (GM8E01, Rev 2, CRC 61592372) | **Proven** | Hashes match Redump "(USA) (Rev 2)" entry exactly; see [GAME_DATA.md](GAME_DATA.md) |
| Reference repos cloned/pinned | **Proven** | metaforce @ `621ee0f`, aurora @ `5143394` (pin used by metaforce), prime @ `72e31c7`, sunpad @ `7d84cec`; see [DEPENDENCIES.md](DEPENDENCIES.md) |
| Architecture investigation | **Proven** | Current tree renders via Aurora GX-on-WebGPU (Dawn) → Metal on Apple; see [ARCHITECTURE.md](ARCHITECTURE.md) |
| macOS ARM64 build of current Metaforce | **Not yet tested** | Configure/build started this session |
| Metaforce launch on macOS (Metal, frame, title flow) | **Not yet tested** | |
| Input on macOS (keyboard/mouse/controller) | **Not yet tested** | |
| Audio on macOS | **Not yet tested** | |
| HECL/game-data extraction from supplied ISO | **Not yet tested** | Raw ISO; no conversion required |
| iOS/iPadOS ARM64 device build | **Not yet tested** | Upstream CI builds iOS `.app`; not yet reproduced locally |
| Dawn/WebGPU reaching Metal on iOS device | **Not yet tested** | Principal technical unknown (Gate 2) |
| Touch controls (iPhone/iPad layouts) | **Not yet tested** | |
| GameController support | **Not yet tested** | |
| Save/reload behavior | **Not yet tested** | |
| Frigate Orpheon / later-area gameplay | **Not yet tested** | |

## Upstream context

- Metaforce README marks the project **alpha** and says builds are "currently
  unavailable while the project undergoes large changes," but upstream CI
  ([`.github/workflows/build.yml`](../ref/metaforce/.github/workflows/build.yml))
  actively builds macOS, iOS, tvOS, Windows, and Linux.
- The current tree moved graphics into the **Aurora** compatibility layer
  (GX on WebGPU/Dawn). Older documentation referencing a direct Dawn integration
  is stale; see [ARCHITECTURE.md](ARCHITECTURE.md).

## Proven / partially proven detail

### Proven

- **Dump integrity (2026-08-11):** `ref/Metroid Prime [GM8E01] [61592372][b].iso`
  (1,459,978,240 bytes) has CRC32 `61592372`, MD5 `fdfc41b8414dd7d24834c800f567c0f8`,
  SHA-1 `1a737910b55b59c6ad91be9e3e3c43517fd52efb`. Game ID `GM8E01`, revision byte
  `0x02` at header offset `0x07` (Rev 2), magic `0xC2339F3D`, title "Metroid Prime".
- **Repository pins (2026-08-11):** see [DEPENDENCIES.md](DEPENDENCIES.md).

### Partially proven

- Nothing yet.

### Simulator-only

- Nothing yet.

### Physical-device verified

- Nothing yet.

### Blocked

- Nothing yet.

### Not yet tested

- Everything in the "Current state" table marked **Not yet tested**.

