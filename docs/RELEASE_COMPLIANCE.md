# Public binary release gate

Last updated: 2026-11-10

EctoPad currently distributes source and documentation only. No IPA, TestFlight,
App Store, or AltStore PAL release is approved.

## Blocking items

Status checked 2026-11-10 against the restored 0.1.4 build:

- [x] **Nintendo key material.** `release_gate.py` found the Wii retail,
  Korean and vWii common keys in the app binary (from nod's Wii ticket
  decryption). `scripts/bootstrap-workspace.sh` now blanks those rows and
  `patches/workspace/nod.patch` refuses Wii decryption; the rebuilt 0.1.4 IPA
  passes `release_gate.py`. EctoPad reads GameCube discs only.
- zeus and kabufuda (below) were re-verified in the recovered trees: no
  license file and no license commit in either history.
- soxr (below) is still statically linked: 78 soxr symbols in the macOS
  binary, used by the voice resampler in `Runtime/Audio/SDLBackend.cpp`.
- AxioDL removed the upstream repositories; exact pins now come from EctoPad's
  mirrors ([SOURCE_RECOVERY.md](SOURCE_RECOVERY.md)). That moves where
  corresponding source lives; it does not change any component's license.
- License texts: `scripts/collect-licenses.sh` now bundles the texts of every
  linked component, including the 57 Rust crates in nod and Dawn's license from
  the commit its prebuilt package was built from (`google/dawn` `13abc3b`), in
  the IPA and Mac app. zeus and kabufuda are EctoPad's own code (below).
- Reproducible builds: two fresh IPA builds at different checkout paths were
  byte-identical (`ZERO_AR_DATE=1`, `-oso_prefix`, fixed file times).

- [x] Resolve the redistribution status of pinned
  [`AxioDL/zeus`](https://github.com/AxioDL/zeus) revision `d64e9f5`. No
  license exists in its history. 2026-11-10: replaced by EctoPad's own
  GPL-3.0-or-later math library (`components/zeus`), checked line by line
  against the original's outputs and by building and running the game.
- [x] Resolve the redistribution status of pinned
  [`AxioDL/kabufuda`](https://github.com/AxioDL/kabufuda) revision `9493afc`. No
  license exists in its history. 2026-11-10: replaced by EctoPad's own
  GPL-3.0-or-later memory-card library (`components/kabufuda`), checked against
  real cards and an in-game save/reload on the Mac.
- [x] Have the copyright holder select a root license for EctoPad-owned patches,
  scripts, integration code, and documentation. 2026-11-10: Chris delegated the
  choice; EctoPad-owned work is GPL-3.0-or-later (`LICENSE`), consistent with
  the GPL-3.0-or-later SunPad code the app combines.
- [ ] Assemble exact corresponding source for the final binary: every pinned
  upstream tree, submodule, vendored dependency, EctoPad patch, generated source
  needed for the build, and the scripts and configuration needed to rebuild it.
- [ ] Replace the statically linked
  [`soxr`](https://github.com/chirlu/soxr) path with a clearly compatible
  alternative, or provide its LGPL-2.1-or-later text, exact source, relinkable
  application material, and working relink instructions.
- [ ] Generate and bundle the complete license and copyright notice inventory
  from the actual final dependency graph.
- [ ] Rebuild Dawn with path remapping or use a scrubbed package so release
  diagnostics do not embed upstream CI source paths.
- [ ] Produce the final unsigned package and source/relink bundle together;
  audit ZIP safety, architecture, platform, notices, prohibited data, signing
  material, exact hashes, and reproducibility.
- [ ] Obtain an IP-law review of the final release boundary, including Nintendo
  screenshots, descriptive trademark use, and any DMCA-circumvention concerns.

## Contribution path

Evidence and implementation help are welcome through EctoPad issues and pull
requests. Useful contributions include authoritative license evidence for the
exact pinned revisions, clearly licensed replacement dependencies, and
reproducible source/relink tooling.

Do not contact or pressure upstream maintainers on EctoPad's behalf. Project
disagreement does not change a license grant, and community pressure does not
create one.

## Republication rule

Do not publish an IPA merely because it contains no game data or passes
`scripts/audit-ios-package.sh`. Republishing is allowed only after every blocker
above is resolved and README, notices, provenance, source, relink materials,
package contents, and recorded hashes describe the same final artifact.
