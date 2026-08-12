# Game Data

Last updated: 2026-08-12

## Supported disc

- **Game:** Metroid Prime
- **Platform:** Nintendo GameCube
- **Region:** USA / NTSC-U
- **Serial:** DOL-GM8E-USA
- **Revision:** 2
- **Image:** `ref/Metroid Prime [GM8E01] [61592372][b].iso` (raw ISO — no conversion needed)

## Validation evidence (2026-08-11)

| Property | Value |
| --- | --- |
| File size | 1,459,978,240 bytes (`0x57058000`) — exact standard GameCube disc size |
| Game ID (offset `0x00`) | `GM8E01` |
| Disc number (offset `0x06`) | `0x00` |
| Version / revision byte (offset `0x07`) | `0x02` → **Rev 2** |
| Disc magic (offset `0x10`) | `0xC2339F3D` ✓ |
| Title string (offset `0x20`) | "Metroid Prime" |
| CRC32 | `61592372` |
| MD5 | `fdfc41b8414dd7d24834c800f567c0f8` |
| SHA-1 | `1a737910b55b59c6ad91be9e3e3c43517fd52efb` |

All three hashes match Redump's **"Metroid Prime (USA) (Rev 2)"** entry
(redump.org disc 1324) byte-for-byte. The `[b]` (bad-dump) tag in the source
filename is not reflected in the hashes; the image is byte-identical to the
Redump reference and is treated as a good dump.

## Validation procedure (reproducible)

```sh
# Size
stat -f "%z" "ref/Metroid Prime [GM8E01] [61592372][b].iso"   # expect 1459978240

# Header
xxd -l 64 "ref/Metroid Prime [GM8E01] [61592372][b].iso"     # GM8E01, magic c2339f3d
xxd -s 6 -l 2 "ref/Metroid Prime [GM8E01] [61592372][b].iso" # version byte at 0x07 == 02

# Hashes
shasum -a 1 "ref/Metroid Prime [GM8E01] [61592372][b].iso"
md5 "ref/Metroid Prime [GM8E01] [61592372][b].iso"
python3 -c "import zlib; ..."  # CRC32 == 61592372
```

The SHA-1 (`1a737910b55b59c6ad91be9e3e3c43517fd52efb`) is the canonical
integrity reference for this project.

## Format and conversion

The image is a raw `.iso` (already in the form Metaforce's `nod` library reads
directly), so **no conversion is required**. If a future image arrives as
`.nkit.iso`, `.rvz`, or `.ciso`, convert locally to raw ISO/GCM with
`dolphin-tool convert` or NKit tooling, keep the result ignored, and record the
new hashes here.

## How the runtime consumes game data (verified 2026-08-11)

The current Metaforce tree reads the user's **disc image file directly** via
nod-backed `CDvdFile` — **no HECL extraction step exists in the current flow**:

- `CDvdFile::Initialize(path)` opens the path with SDL and hands it to
  `nod_disc_open_stream` (nod supports ISO/GCM/RVZ/CISO etc.). Verified: our raw
  ISO opens and all assets load from it.
- Disc-path sources:
  - **iOS/tvOS:** `CMain.cpp` sets `m_deferredProject = <store root>/game.iso`
    (`SDL_GetPrefPath` = the app container). The app is expected to place the
    user's ISO there.
  - **Desktop (macOS):** a CLI positional arg (existing file path), a drag-and-drop
    onto the window (`SDL_EVENT_DROP_FILE` → `m_gameDiscSelected`), or the ImGui
    disc-selection flow — all funnel into `m_deferredProject` and open on the next
    idle tick when no game is loaded.
  - `content://` URIs are handled (Android pattern; cleared from `lastDiscPath`
    on failure).

### Consequences for the port

- **No HECL/asset-preparation pipeline is required for the base flow.** The
  "prepare privately → stage → activate" workflow reduces to: validate the user's
  ISO → copy it into private storage as `game.iso` (staging + atomic rename) →
  point the runtime at it. This matches the goal's preference to not force HECL
  into iOS when the architecture doesn't need it.
- **iOS data flow (implemented 2026-08-12):** the unchanged SunPad
  `••• → Game Data & Saves` UI opens a Files picker (or lists ISO/GCM files in
  the Files-visible Documents folder). The bridge holds the security-scoped URL
  while a serial import service validates size/header, copies to a mode-0600
  same-directory staging file, fsyncs it, validates the staged SHA-1, then uses
  `rename(2)` for atomic activation as `<store root>/game.iso` and fsyncs the
  directory. The imported image is excluded from backup and receives iOS file
  protection where available. A failure removes staging and leaves the active
  image untouched.
- **Removal and separation:** removal deletes only `game.iso` and stale import
  files. The Dolphin memory card, configuration, touch settings, caches, and
  logs are not removed. Reimport likewise leaves saves untouched.
- **Validation** before activation uses the procedure above; the canonical
  integrity reference is SHA-1 `1a737910b55b59c6ad91be9e3e3c43517fd52efb`.

### Runtime evidence (2026-08-11)

- `Metaforce -l <iso>` opened the ISO via nod and identified
  **"Metroid Prime USA (Build v1.111 3/10/2003 17:56:21)"**; thousands of assets
  (STRG/TXTR/CMDL/CSKR/ANIM/EVNT/PART/SWHC/DGRP/AGSC/SAVW/HINT/ATBL/DPSC/CRSC/
  WPSC/ELSC/SCAN/FONT/FRME...) built from the disc at runtime with no errors.
- Store root (`SDL_GetPrefPath("AxioDL", "metaforce")`):
  `~/Library/Application Support/AxioDL/metaforce/` — contains `dawn_cache.db`,
  `pipeline_cache.db`, `imgui.ini`.
- **iOS Simulator (2026-08-11):** copied the ISO to the app container at
  `Library/Application Support/axioDL/metaforce/game.iso` (the SDL pref path on
  iOS); the app loaded it and rendered the title screen. Confirms the iOS data
  path: place/validate the user's ISO at the store root.

### Atomic import evidence (2026-08-12, iPad Simulator)

- A 4 KiB invalid file was rejected on size; the existing canonical image,
  save hash, and lack of staging files were unchanged.
- A canonical image imported successfully. Its active inode changed, the
  active SHA-1 remained canonical, and a normal `--autostart` restart loaded
  **Metroid Prime USA (Build v1.111)** through Dawn/WebGPU → Metal.
- Removing stored game data deleted `game.iso` while preserving the save hash;
  a subsequent valid reimport restored a playable image.
- A full-size image with a payload byte changed passed header/size checks but
  failed staged SHA-1 validation. The prior active inode/SHA-1 and save hash
  remained unchanged, and staging was cleaned.
- Runtime log: `/tmp/metaforce-import-atomic-2026-08-12.log`. The production
  picker/delegate and Files-visible plist keys compile for Simulator and arm64
  iOS, but picker tapping still awaits working Simulator touch delivery or a
  physical device. The tested hook calls the same import service.

## Privacy boundaries

- The ISO, converted images, extracted Nintendo assets, generated game-derived
  data, and saves are **never** committed, packaged, or redistributed.
- The app must only ever import data the user supplies.
