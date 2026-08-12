# Third-party notices

This application combines Metaforce with its pinned dependencies and a direct
port of SunPad's Apple overlay, input, settings, mapping, and diagnostics code.
No Nintendo game data, extracted assets, saves, keys, or signing material are
included.

| Component | License / notice |
| --- | --- |
| Metaforce | MIT |
| SunPad-derived Apple application-shell code | GPL-3.0-or-later |
| Aurora | MIT |
| zeus | GPL-3.0 |
| nod | MIT OR Apache-2.0 |
| SDL3 | Zlib |
| Dawn / Tint / WebGPU headers | BSD-3-Clause and Apache-2.0; see upstream notices |
| amuse, athena, logvisor, lzokay, musyx, jbus, fixNES | MIT |
| soxr | LGPL-2.1 |
| libjpeg-turbo | BSD-3-Clause and IJG |
| spdlog, fmt, ImGui, Tracy, xxHash | permissive upstream licenses |
| zlib, zstd, bzip2, xz/liblzma, libpng, FreeType, Abseil | upstream licenses |

The packaged `LICENSE-Metaforce.txt` and `COPYING-GPL-3.0.txt` files preserve
the principal project license texts. The complete dependency inventory, pins,
and source locations are documented in `docs/DEPENDENCIES.md` in the Ectopad
source tree; the upstream checkouts retain their individual license files.

The SunPad-derived files retain their original copyright and license headers.
At source checkout `7d84cec`, SunPad's root `LICENSE` supplies the full GNU GPL
version 3 terms bundled as `COPYING-GPL-3.0.txt`.

This notice file is not by itself sufficient for public binary distribution.
In particular, a public build must satisfy the GPL corresponding-source terms
for the combined work and the LGPL static-link/relink terms that apply to soxr.
The unsigned validation IPA produced by this repository is therefore a local
engineering artifact until those release materials have been assembled and
reviewed.

Metroid Prime, Nintendo, and GameCube names and game content belong to their
respective rights holders. This project is not affiliated with or endorsed by
Nintendo. Users must supply their own legally obtained compatible game data.
