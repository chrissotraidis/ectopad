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

## Upstream authorship and platform history

- Metaforce's MIT license names Jack Andersen and Phillip "Antidote" Stephens
  as its original authors. EctoPad preserves that notice and does not claim
  authorship of the Metaforce engine.
- Metaforce already contained iOS/tvOS build support before EctoPad. The pinned
  repository history traces its introduction to Luke Street's 2022 commit
  `4048492` ("iOS and tvOS support"), followed by later Metaforce and Aurora
  work.
- EctoPad is a downstream Apple-focused integration and patch set. Its claims
  must be limited to the additions and device evidence recorded in this
  repository.
- AxioDL made Metaforce, zeus, amuse and kabufuda private in 2026. The exact
  pinned revisions were recovered from Software Heritage and are kept, with
  their original files and authorship, in private `chrissotraidis/ectopad-*`
  mirrors ([SOURCE_RECOVERY.md](SOURCE_RECOVERY.md)). Recovery does not change
  any component's license.

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

- the pinned `zeus` tree contains no license file or other authoritative
  redistribution grant (none in its history). AxioDL's `kabufuda` had the same
  gap and was replaced on 2026-11-10 by EctoPad's own GPL-3.0-or-later
  memory-card library (`components/kabufuda`);
- EctoPad-owned patches, scripts, and integration code do not yet have a root
  project license selected by their copyright holder;
- the combined executable includes SunPad-derived GPL-3.0-or-later code, but
  exact corresponding source for the binary has not been assembled;
- soxr is statically linked under LGPL-2.1-or-later, and the required library
  source, license text, relinkable application material, and instructions have
  not been packaged; and
- the full binary-derived dependency notice inventory has not been bundled and
  verified.

Progress on 2026-11-10 (restored 0.1.4, local only): the app no longer contains
nod's embedded Wii common keys (blanked at bootstrap; the IPA and Mac zip pass
`release_gate.py`); license texts for every linked component except zeus are
bundled by `scripts/collect-licenses.sh`; the IPA and Mac zip rebuild
byte-identically from a fresh checkout; EctoPad's own work is GPL-3.0-or-later
(`LICENSE`); and kabufuda is replaced by EctoPad's own library. The zeus,
corresponding-source and soxr items above remain open.

The pinned prebuilt Dawn library also embeds upstream GitHub Actions builder
paths (`/Users/runner/work/dawn-build/...`) in diagnostic strings. Those paths
are provenance and release-hygiene debt rather than the central license blocker,
but a future public build should use a path-remapped or scrubbed package.

See [RELEASE_COMPLIANCE.md](RELEASE_COMPLIANCE.md) for the republication gate.

## Public-facing language

- "Downstream Apple-focused integration of Metaforce"
- "Requires user-supplied, legally obtained game data"
- "No Nintendo game data included"
- "Not affiliated with or endorsed by Nintendo"
- Credit Metaforce and its original authors prominently whenever introducing
  EctoPad.
- Do not claim that EctoPad first brought Metaforce or Metroid Prime to iOS,
  that it replaces Metaforce, or that the complete app is stable while known
  blockers remain.

Because Metroid Prime Remastered is a commercially active Nintendo product,
elevated takedown/DMCA exposure is assumed; keep the above language factual and
consistent, and never add ROM-download links, asset mirrors, bundled keys, or
piracy-oriented instructions.
