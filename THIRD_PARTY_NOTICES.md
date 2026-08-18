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
| zeus | **NOASSERTION**; no repository-wide license file found at pinned revision `d64e9f5` |
| kabufuda | **NOASSERTION**; no repository-wide license file found at pinned revision `9493afc` |
| nod | MIT OR Apache-2.0 |
| SDL3 | Zlib |
| Dawn / Tint / WebGPU headers | BSD-3-Clause and Apache-2.0; see upstream notices |
| amuse, athena, logvisor, lzokay, musyx, jbus, fixNES | MIT |
| soxr | LGPL-2.1-or-later |
| libjpeg-turbo | BSD-3-Clause and IJG |
| spdlog, fmt, ImGui, Tracy, xxHash | permissive upstream licenses |
| zlib, zstd, bzip2, xz/liblzma, libpng, FreeType, Abseil | upstream licenses |

Metaforce's MIT text and SunPad's GPLv3 text were included in the retired local
validation package. That package did not contain the complete license texts,
copyright notices, exact corresponding source, or LGPL relink materials needed
for a public release. A summary table is not a substitute for those materials.

The SunPad-derived source relationship is documented at checkout `7d84cec`;
SunPad's root `LICENSE` states GPL-3.0-or-later. The copied source files did not
carry separate file-level license headers in either tree, so this repository
does not claim that such headers were preserved.

No license is inferred merely because a repository is public or included as a
Metaforce submodule. `zeus` and `kabufuda` must receive authoritative license
clarification or be replaced before another binary is distributed. See
`docs/RELEASE_COMPLIANCE.md`.

Metroid Prime, Nintendo, and GameCube names and game content belong to their
respective rights holders. This project is not affiliated with or endorsed by
Nintendo. Users must supply their own legally obtained compatible game data.
