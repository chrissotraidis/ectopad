# Legal & Provenance

Last updated: 2026-08-18

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

There is no approved public binary release. The previously tracked EctoPad
0.1.3 unsigned IPA, SHA-256
`264cc139fda991a0b91954ce5df5574e15027fc606d3041703bbb7432481267c`,
was removed from the current branch on 2026-08-18. It was a local package-audit
artifact, not a completed redistribution package.

Public binary distribution remains blocked by all of the following:

- the pinned `zeus` and `kabufuda` trees contain no repository-wide license
  file or other authoritative redistribution grant found by the current audit;
- EctoPad-owned patches, scripts, and integration code do not yet have a root
  project license selected by their copyright holder;
- the combined executable includes SunPad-derived GPL-3.0-or-later code, but
  exact corresponding source for the binary has not been assembled;
- soxr is statically linked under LGPL-2.1-or-later, and the required library
  source, license text, relinkable application material, and instructions have
  not been packaged; and
- the full binary-derived dependency notice inventory has not been bundled and
  verified.

The pinned prebuilt Dawn library also embeds upstream GitHub Actions builder
paths (`/Users/runner/work/dawn-build/...`) in diagnostic strings. Those paths
are provenance and release-hygiene debt rather than the central license blocker,
but a future public build should use a path-remapped or scrubbed package.

See [RELEASE_COMPLIANCE.md](RELEASE_COMPLIANCE.md) for the republication gate.

## Public-facing language

- "Native reimplementation / native Apple port"
- "Requires user-supplied, legally obtained game data"
- "No Nintendo game data included"
- "Not affiliated with or endorsed by Nintendo"

Because Metroid Prime Remastered is a commercially active Nintendo product,
elevated takedown/DMCA exposure is assumed; keep the above language factual and
consistent, and never add ROM-download links, asset mirrors, bundled keys, or
piracy-oriented instructions.
