# Public binary release gate

Last updated: 2026-08-18

EctoPad currently distributes source and documentation only. No IPA, TestFlight,
App Store, or AltStore PAL release is approved.

## Blocking items

- [ ] Resolve the redistribution status of pinned
  [`AxioDL/zeus`](https://github.com/AxioDL/zeus) revision `d64e9f5`. No
  repository-wide license file or authoritative grant was found in the pinned
  tree or its visible history.
- [ ] Resolve the redistribution status of pinned
  [`AxioDL/kabufuda`](https://github.com/AxioDL/kabufuda) revision `9493afc`. No
  repository-wide license file or authoritative grant was found in the pinned
  tree or its visible history.
- [ ] Have the copyright holder select a root license for EctoPad-owned patches,
  scripts, integration code, and documentation. Do not infer or apply a license
  automatically.
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
