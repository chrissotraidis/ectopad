# EctoPad memory-card library

This is EctoPad's own reader and writer for GameCube memory-card images (the
`MemoryCardA.USA.raw` files Metroid Prime saves to). It replaces AxioDL's
kabufuda, whose recovered history never carried a license, so it could not be
redistributed.

It keeps the `kabufuda::` names and signatures that Metaforce's
`CMemoryCardSys` and `CMemoryCardDriver` call, so Metaforce builds against it
unchanged. The code was written for EctoPad from the GameCube card layout
(header, two directory copies, two block-allocation-table copies, chained 8 KiB
data blocks; summarized at the top of `src/Card.cpp`). All I/O is synchronous,
which Metaforce's card driver accepts and which suits iOS.

License: GPL-3.0-or-later, like the rest of EctoPad (see the repository
`LICENSE`).

## How it was checked (2026-11-10)

- `tests/card_tests.cpp`: format, create, write, read back after reopening,
  status (also after closing a file, as the game does), rename, delete and
  free-space accounting. Build it standalone:
  `cmake -S components/kabufuda -B /tmp/kabufuda-tests && cmake --build /tmp/kabufuda-tests && /tmp/kabufuda-tests/kabufuda_tests`.
  Passing a card path dumps its directory and file hashes from a private copy.
- Real cards (Mac cards A and B, an iPad card): the dump matches AxioDL's
  kabufuda reading the same files, and cards written by this library read
  identically in AxioDL's.
- Mac game: the file-select screen lists both saves, the 59% save loads into
  Chozo Ruins, an in-game save-station save succeeds, and after relaunch the
  file select shows the updated play time. Both libraries read the saved card
  identically.

`scripts/bootstrap-workspace.sh` copies this folder into
`ref/metaforce/extern/kabufuda` in place of the old submodule.
