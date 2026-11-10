# EctoPad Goal Loop: back online, then public

Last updated: 2026-11-10

This is the standing work loop for getting EctoPad playable again after the
upstream sources went private, and then preparing a public release. Work on
branch `codex/restore-0.1.4` (based on `origin/main`).

## Goal

People can build or install EctoPad, supply their own Metroid Prime (USA,
Rev 2) disc image, and play on iPhone, iPad, and Apple Silicon Mac, with
complete corresponding source and credit to Metaforce and its authors.

"Done" for this loop means Gates A–E are checked with dated evidence and Chris
has the draft release in front of him. Gate F happens only on his word.

## Ground rules

- **Repository rule (AGENTS.md): releases are paused.** No public release,
  IPA, macOS build, or download link until this repository is marked Clear in
  Chris' private release audit. Every artifact must pass
  `python3 ~/.codex/release-gate/release_gate.py <artifact>`; a failure is a stop.
- **docs/RELEASE_COMPLIANCE.md blockers** must all be resolved before any
  public binary.
- **Never commit game data, saves, keys, or signing material.** `ref/` stays
  ignored. Run `scripts/audit-ios-package.sh` and
  `scripts/check-release-boundary.sh`.
- **Never uninstall EctoPad from a device to update it.** Install in place;
  bundle ID stays `com.axiodl.Metaforce`. Back up the app container first when
  it holds saves.
- **Prefer the original code.** `patches/workspace/` is the captured,
  device-tested EctoPad workspace. Add new fixes on top, then recapture the
  patches from the workspace.
- **Chris approves before anything public or irreversible.** PadMint rule:
  run PadMint's `scripts/player-check.sh` before tagging and after publishing.

## Where things are

- `scripts/bootstrap-workspace.sh` builds `ref/metaforce` from exact commits
  fetched from `chrissotraidis/ectopad-*` (private) plus public repos, then
  applies `patches/workspace/*.patch`. Source history:
  [SOURCE_RECOVERY.md](SOURCE_RECOVERY.md).
- Builds: `scripts/build-macos.sh`; `scripts/build-padmint-ios.sh OUT.ipa`;
  `scripts/build-padmint-macos.sh OUT.zip`. Device install:
  `scripts/sign-install-device.sh <udid>` with `ECTOPAD_PROFILE` and
  `ECTOPAD_IDENTITY`.
- Disc image: `ref/Metroid Prime.iso` (verified `GM8E01`, SHA-1
  `1a737910b55b59c6ad91be9e3e3c43517fd52efb`). Mac save backup:
  `ref/_saves-backup-20261110/`.
- Retired reconstruction (rebuilt from prose before the workspace patches were
  found): `ref/metaforce-reconstruction-20261110`. Reference only.
- Uncommitted attribution edits from an earlier session: `git stash` entry
  "Uncommitted attribution edits…" and `ref/_attribution-edits-*`.
- Devices: iPad Pro 12.9 (6th gen) `00008112-001D485114DBC01E`; iPhone 14
  `00008110-001C2C641123A01E`. Both must be unlocked for `devicectl`.

## The loop (every turn)

1. Orient: this checklist, `git status` here and in `ref/metaforce`.
2. Take the first unchecked item in the lowest open gate.
3. Make the smallest change that finishes it; rebuild only what it affects.
4. Verify with evidence (build output, logs, screenshots, device readback).
   Build or Simulator checks never count as device acceptance.
5. Record the date and evidence here; update CURRENT_STATE/KNOWN_ISSUES when
   behavior changes.
6. Save: commit, push the branch; recapture `patches/workspace/` after source
   changes.
7. Ask Chris only for device unlocks, playtests, game-feel calls, license or
   legal decisions, and approvals.

## Gates

### Gate A: Mac plays again

- [x] Recovered sources build into `EctoPad.app` (2026-11-10; original
      workspace + CMake 4 soxr policy fix; 5/5 controller checks).
- [~] Launch with `scripts/run-macos.sh "ref/Metroid Prime.iso"`; reach title.
      2026-11-10 (reconstruction build): Metal, amuse, frontend music with 0
      underruns, first-run shader compile (`docs/evidence/2026-11-10-gate-a/`).
      Repeat on the workspace build with Chris at the keyboard.
- [ ] Load the existing 59% save; walk, shoot, scan, Morph Ball, visors;
      music and effects audible; save and reload.

### Gate B: EctoPad's own changes restored

- [x] 2026-11-10: `patches/workspace/` (captured from the maintained
      workspace in October) applies cleanly to the recovered pins; the
      bootstrapped workspace matched the patch-applied trees file for file.
      This restores the real touch controls, menu, audio session, overlay
      recovery, readability, door/render work, and diagnostics. The earlier
      prose rebuilds are retired.
- [ ] Confirm on device during the Gate C playtest.

### Gate C: iPad and iPhone play again

- [x] `ref/sunpad` present (GPL license for packaging).
- [ ] iOS build via `scripts/build-padmint-ios.sh` succeeds (version 0.1.4).
- [ ] Back up the iPad app container (it now holds the copied disc image
      from the reconstruction test), then install the workspace build in place.
- [ ] Diagnostic log: no crash, audio route opened; rendering confirmed.
- [ ] **Chris playtest on iPad:** touch controls (R hold, L/R latch), •••
      menu, render scale and aspect, controller connect/reconnect, music and
      sound, save at a station, quit, relaunch, load.
- [ ] Same install and short smoke test on the iPhone.
- [ ] Known 0.1.3 defects still documented; any new defect fixed or listed.

### Gate D: rebuildable from scratch

- [x] `scripts/bootstrap-workspace.sh` fetches from the `ectopad-*` mirrors
      (2026-11-10); Metaforce's full 7,460-commit history is on
      `ectopad-metaforce` `main`.
- [ ] Recapture `patches/workspace/` after new fixes (soxr CMake policy).
- [ ] From an empty `ref/`: bootstrap + Mac build + iOS build succeed.
- [ ] Update DEPENDENCIES.md, BUILDING.md, LEGAL_AND_PROVENANCE.md for the
      mirrors and recovery.

### Gate E: ready to publish (everything except the switch)

Blockers from docs/RELEASE_COMPLIANCE.md, updated 2026-11-10:

- [ ] **zeus and kabufuda have no license.** Verified 2026-11-10: the pinned
      trees contain no license file and no license commit. Options: replace
      them with independently written or clearly licensed code, or obtain a
      written grant. Needs Chris' decision.
- [ ] **Chris chooses a license** for EctoPad's own patches, scripts, and docs.
- [ ] Corresponding source: bootstrap + patches + mirrors at exact commits
      (largely done via Gate D; publish the mirrors with the release).
- [ ] soxr (LGPL-2.1) is statically linked: verified 2026-11-10, 78 soxr
      symbols in the workspace Mac binary (voice resampler in SDLBackend.cpp).
      Replace it with a compatibly licensed resampler or ship LGPL source and
      relink materials.
- [ ] Complete license/notice inventory from the final dependency graph.
- [ ] Dawn builder paths: rebuild with path remapping or document.
- [ ] Unsigned IPA and Mac zip built, audited (`audit-ios-package.sh`,
      `release_gate.py`), reproduced byte-identically.
- [ ] IP-law review of the release boundary (Chris).
- [ ] README updated; draft GitHub release prepared, not published.
- [ ] Chris marks the repository Clear in his release audit.

### Gate F: public (needs Chris' go-ahead)

- [ ] Make the source mirrors public, publish the release, verify downloads.
- [ ] Optional PadMint un-pause with `player-check.sh` before and after.
