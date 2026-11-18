# Source recovery (November 2026)

AxioDL made the Metaforce engine and several of its libraries private on
GitHub by October 2026. Metaforce and amuse had been published under the MIT
license, and the licenses granted for those published revisions still apply.
zeus and kabufuda were public but never carried a license (none in either
history, checked 2026-11-10); EctoPad replaced both with its own libraries
(`components/`). EctoPad's exact pins were recovered on
2026-11-09 from [Software Heritage](https://archive.softwareheritage.org/), a
public archive of open-source code.

| Project | Revision | Software Heritage identifier |
| --- | --- | --- |
| metaforce | `621ee0fed3fdd5fccbbdadbce33343f29dce071c` | `swh:1:rev:621ee0fed3fdd5fccbbdadbce33343f29dce071c` (directory `swh:1:dir:40f67bb361ce8a9ab2d3beb4ce919aa2bce729b9`) |
| zeus | `d64e9f502a07c808596c5112bfd46bf0a0d8138e` | `swh:1:rev:d64e9f502a07c808596c5112bfd46bf0a0d8138e` |
| amuse | `ad9bc96af472dae03fde215163446bdec80be67f` | `swh:1:rev:ad9bc96af472dae03fde215163446bdec80be67f` |
| kabufuda | `9493afc8ff961ee04193038046308411904b8137` | `swh:1:rev:9493afc8ff961ee04193038046308411904b8137` |
| fixNES | `156fcaca9f4cd9c23d423737994c45cfb05d16ca` | `swh:1:rev:156fcaca9f4cd9c23d423737994c45cfb05d16ca` |

All five were recovered with full history and saved, with their license files
and authorship intact, in private `chrissotraidis/ectopad-*` repositories
(`ectopad-metaforce`, `-zeus`, `-amuse`, `-kabufuda`). The AxioDL repositories
that are still public (`jbus`, `musyx`, `libjpeg-turbo`, `logvisor`, `lzokay`)
are mirrored there as well. Metaforce's `main` carries its full 7,460-commit
history, from Phillip Stephens' and Jack Andersen's first commits in 2015
through Luke Street's `621ee0f` in June 2026.

`scripts/bootstrap-workspace.sh` fetches those exact commits from the mirrors
and applies EctoPad's changes from `patches/workspace/`, which were captured
from the maintained workspace before the upstream repositories went private.
Git checks every commit by hash, so the mirrors yield byte-identical sources.
A workspace built this way on 2026-11-10 matched the patch-applied upstream
trees file for file.

The original authors and license files are kept unchanged. Metaforce's MIT
license names Jack Andersen and Phillip "Antidote" Stephens; see
[LEGAL_AND_PROVENANCE.md](LEGAL_AND_PROVENANCE.md).
