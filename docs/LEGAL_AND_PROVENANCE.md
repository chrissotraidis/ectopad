# Legal & Provenance

Last updated: 2026-08-12

## Boundaries

- **Metaforce** is the engine: a reverse-engineered, clean-room-style native
  reimplementation of the Metroid Prime engine. Its license: see
  `ref/metaforce/LICENSE`.
- **PrimeDecomp/prime** is a matching decompilation used only as a research /
  accuracy reference. It is **not** required game code and **not** a build
  dependency. Incomplete decomp progress must not block this port.
- **Game data:** the user supplies their own legally obtained Metroid Prime
  (USA, Rev 2) disc image. All Nintendo assets remain the property of their
  rights holders and are **not** distributed.
- **No redistribution of:** the ISO, converted images, extracted Nintendo
  assets, generated game-derived data, saves, signing material, provisioning
  profiles, certificates, or private artifacts.

## Supported revision / hash (provenance anchor)

- Game: Metroid Prime (USA) (Rev 2), serial DOL-GM8E-USA, game ID `GM8E01`
- SHA-1: `1a737910b55b59c6ad91be9e3e3c43517fd52efb`
- MD5: `fdfc41b8414dd7d24834c800f567c0f8`
- CRC32: `61592372`

## Release boundary

- Distributable: engine/application source code and legally redistributable
  dependencies under their licenses.
- Not distributable: the Metroid Prime disc image or any extracted Nintendo
  assets.
- Every IPA/app/archive is audited before being called distributable
  (see [TESTING.md](TESTING.md)). The current deterministic IPA is explicitly
  an unsigned local validation artifact, not a distributable release.
- Exact final local validation artifact: SHA-256
  `308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`.
  It was reproduced byte-identically, but remains unsigned and non-public.
- The combined binary includes GPL-covered SunPad-derived code and zeus plus a
  statically linked LGPL-2.1 soxr build. Public distribution remains blocked
  until corresponding-source and LGPL relink materials are assembled and
  reviewed; `THIRD_PARTY_NOTICES.md` alone is not sufficient.
- The pinned prebuilt Dawn library embeds its upstream GitHub Actions builder
  source paths (`/Users/runner/work/dawn-build/...`) in diagnostic strings.
  They are non-local provenance, not user data, but a public release should
  rebuild Dawn with path remapping or obtain a scrubbed upstream package.

## Public-facing language

- "Native reimplementation / native Apple port"
- "Requires user-supplied, legally obtained game data"
- "No Nintendo game data included"
- "Not affiliated with or endorsed by Nintendo"

Because Metroid Prime Remastered is a commercially active Nintendo product,
elevated takedown/DMCA exposure is assumed; keep the above language factual and
consistent, and never add ROM-download links, asset mirrors, bundled keys, or
piracy-oriented instructions.
