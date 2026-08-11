# Game Data

Last updated: 2026-08-11

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

## Extraction / preparation (HECL)

Expected flow (macOS runtime currently reads the ISO directly via nod-backed
`CDvdFile` — default path is `<store root>/game.iso`, or pass the ISO path as a
CLI argument):

1. Validate image (above).
2. Run Metaforce/HECL extraction tooling to produce the prepared game package
   (PAK data etc.) from the ISO.
3. Validate the prepared package.
4. Stage privately, then activate atomically on-device.

Details will be recorded here and in [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md)
as the pipeline is proven. All extracted/generated data stays local and git-ignored.

### Runtime evidence (2026-08-11)

- `Metaforce -l <iso>` opened the ISO via nod and identified
  **"Metroid Prime USA (Build v1.111 3/10/2003 17:56:21)"**; thousands of assets
  (STRG/TXTR/CMDL/CSKR/ANIM/EVNT/PART/SWHC/DGRP/AGSC/SAVW/HINT/ATBL/DPSC/CRSC/
  WPSC/ELSC/SCAN/FONT/FRME...) built from the disc at runtime with no errors.
- Store root (`SDL_GetPrefPath("AxioDL", "metaforce")`):
  `~/Library/Application Support/AxioDL/metaforce/` — contains `dawn_cache.db`,
  `pipeline_cache.db`, `imgui.ini`.

## Privacy boundaries

- The ISO, converted images, extracted Nintendo assets, generated game-derived
  data, and saves are **never** committed, packaged, or redistributed.
- The app must only ever import data the user supplies.
