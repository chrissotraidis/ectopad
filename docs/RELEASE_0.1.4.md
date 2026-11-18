# EctoPad 0.1.4 release

Prepared 2026-11-18. Nothing here is published until Chris marks the
repository Clear in his release audit (AGENTS.md).

## Release notes (draft)

EctoPad builds and runs again. AxioDL made Metaforce private in 2026; this
release fetches the exact Metaforce commits EctoPad was tested with from
EctoPad's own source mirrors, so it builds from a fresh clone with one command.

- **New math and memory-card libraries.** AxioDL's zeus and kabufuda never had
  a license, so they could not be shared. EctoPad now ships its own versions
  (GPL-3.0-or-later), checked against the originals' outputs, real memory
  cards and in-game play. Existing saves load unchanged.
- **No Nintendo keys.** The disc reader no longer contains the Wii decryption
  keys; EctoPad reads GameCube discs only.
- **Reproducible packages.** The IPA and Mac zip come out byte-identical from
  a fresh build, and every bundled component's license ships inside the app.
- **Self-contained Mac app.** The Mac zip no longer needs Homebrew libraries.

You need your own Metroid Prime (USA, Rev 2) disc image. EctoPad contains no
game data and never downloads any.

Known issues carried over from 0.1.3: the first tutorial door can look closed
after it opens, some walls can render black on iPhone and iPad, and a short
audio blip can play when a game starts. See docs/KNOWN_ISSUES.md.

## Release files

| File | Built from | SHA-256 |
| --- | --- | --- |
| EctoPad-0.1.4-unsigned.ipa | see AGENT_GOAL_LOOP.md Gate E | recorded at build |
| EctoPad-0.1.4-macos.zip | see AGENT_GOAL_LOOP.md Gate E | recorded at build |
| EctoPad-0.1.4-source.zip | bootstrapped workspace, no build output | recorded at build |

## Publishing steps (in order, after Clear)

1. Decide the Metaforce provenance question (RELEASE_COMPLIANCE.md). Rebuild from the tagged commit in a fresh worktree and confirm the hashes
   above. Run `python3 ~/.codex/release-gate/release_gate.py` on every file.
2. Make the source mirrors public, as listed in RELEASE_COMPLIANCE.md.
   Keep `ectopad-zeus`, `ectopad-kabufuda` and `ectopad-musyx` private.
3. From a clean clone on another path, run `scripts/bootstrap-workspace.sh`
   without GitHub credentials to prove the public sources are complete.
4. PadMint: run `scripts/player-check.sh` from PadMint's `main` with this
   clean checkout (AGENTS.md).
5. Merge PR #10, tag `v0.1.4`, and update the README status rows
   (Public IPA, FAQ) and `scripts/check-release-boundary.sh` together.
6. Create the GitHub release with the three files and the notes above.
7. Run PadMint's `scripts/player-check.sh` again without the checkout, then
   un-pause the PadMint catalog entry.
