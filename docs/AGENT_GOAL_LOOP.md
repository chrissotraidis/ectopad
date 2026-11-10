# EctoPad Goal Loop: back online, then public

Last updated: 2026-11-10

This is the standing work loop for getting EctoPad playable again after the
upstream sources went private, and then preparing a public release. Work on
branch `codex/restore-0.1.4` (based on `origin/main`); draft PR
https://github.com/chrissotraidis/ectopad/pull/10. A draft GitHub release with
binaries waits for the release-audit Clear (AGENTS.md).

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
- [x] Launch with `scripts/run-macos.sh "ref/Metroid Prime.iso"`; reach title.
      2026-11-10 (workspace build): launched with `--warp 2 0` into Chozo
      Ruins: world and HUD rendered, W walked through an opening door into the
      next room, audio ran with 0 underruns, clean quit, saves unchanged
      (`docs/evidence/2026-11-10-gate-a/warp-chozo-excerpt.log`; screenshots
      kept out of the repo).
      2026-11-10 (reconstruction build): Metal, amuse, frontend music with 0
      underruns, first-run shader compile (`docs/evidence/2026-11-10-gate-a/`).
      Repeat on the workspace build with Chris at the keyboard.
- [~] Load the existing 59% save; walk, shoot, scan, Morph Ball, visors;
      music and effects audible; save and reload. (Warp starts with base
      inventory, so Morph Ball and save/reload need the real save.)
      2026-11-10 (with EctoPad's own memory-card library): file select lists
      Samus A and Samus B 59%; Samus B loaded into Chozo Ruins with the HUD,
      walked and mouse-looked to the save station, saved ("Game has been
      saved"), quit, relaunched: file select shows the new play time
      (0:00 to 2:13). Card backed up first (`ref/_saves-backup-20261110c`).
      2026-11-10 (own zeus math library): Samus B loaded into Chozo Ruins with
      correct HUD, radar and map; mouse look, walking, Power Beam shot (beam
      effect drawn), Morph Ball in and out (third-person camera follows) all
      worked; 0 errors, 0 audio underruns, card unchanged. Scanning and visors
      still to check with Chris.

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
- [x] iOS build via `scripts/build-padmint-ios.sh` succeeds (version 0.1.4).
      2026-11-10: IPA SHA-256 `fd650080…21b4` (local validation only).
- [x] Back up the iPad app container (it now holds the copied disc image
      from the reconstruction test), then install the workspace build in place.
      2026-11-10: cards, prefs, log saved to `ref/_ipad-backup-20261110/`;
      installed in place; disc image and cards preserved.
- [~] Diagnostic log: no crash, audio route opened; rendering confirmed.
      2026-11-10: title music playing (0 underruns, 0 clipped), ••• menu
      visible with 5 items, overlay attached
      (`docs/evidence/2026-11-10-gate-c/ipad-runtime-workspace-0.1.4.log`).
      Rendering to be confirmed by eye in the playtest.
- [x] Install the build with EctoPad's own zeus and kabufuda in place.
      2026-11-10 17:03 JST: saves backed up with
      `scripts/backup-device-saves.sh` (afcclient; `devicectl device copy`
      hung for every app) to `ref/_ipad-backup-20261110b/` (identical to the
      morning backup); installed in place; disc image (`game.iso`) and cards
      intact; the new session reached title music with no errors and 0
      underruns (`docs/evidence/2026-11-10-gate-c/ipad-runtime-own-zeus-kabufuda.log`).
- [ ] **Chris playtest on iPad:** touch controls (R hold, L/R latch), •••
      menu, render scale and aspect, controller connect/reconnect, music and
      sound, save at a station, quit, relaunch, load.
- [~] Same install and short smoke test on the iPhone.
      2026-11-10 21:43 JST, iPhone 14 (iOS 27.2): EctoPad was not installed
      before, so there were no saves to back up. Installed the same build as
      the iPad (binary `3de2370b…`); first launch opened the Metal view and the
      ••• menu (5 items) with no errors. The disc image was copied into the app
      container with afcclient (the same place the in-app import uses); on
      relaunch the audio session opened on the speaker and the title music
      played (frontend 0.57, 0 underruns, 0 clipped)
      (`docs/evidence/2026-11-10-gate-c/iphone-runtime-own-zeus-kabufuda.log`).
      Screen not captured (developer disk image not mounted). Still to do by
      Chris: look at the screen, touch controls, start or load a game.
- [ ] Known 0.1.3 defects still documented; any new defect fixed or listed.

### Gate D: rebuildable from scratch

- [x] `scripts/bootstrap-workspace.sh` fetches from the `ectopad-*` mirrors
      (2026-11-10); Metaforce's full 7,460-commit history is on
      `ectopad-metaforce` `main`.
- [x] Recapture `patches/workspace/` after new fixes (soxr CMake policy).
      2026-11-10: metaforce.patch updated; applies cleanly to `621ee0f`.
- [x] From an empty `ref/`: bootstrap + Mac build + iOS build succeed.
      2026-11-10: fresh worktree bootstrap + `build-macos.sh` (5/5 checks);
      freshly bootstrapped `build-padmint-ios.sh` produced the 0.1.4 IPA.
- [x] Update DEPENDENCIES.md, BUILDING.md, LEGAL_AND_PROVENANCE.md for the
      mirrors and recovery. 2026-11-10: all three plus SOURCE_RECOVERY.md;
      the stashed attribution edits (CREDITS.md, upstream-credit wording)
      are merged; the stash's mouse-aim doc edits were left out.

### Gate E: ready to publish (everything except the switch)

Blockers from docs/RELEASE_COMPLIANCE.md, updated 2026-11-10:

- [x] **zeus and kabufuda have no license.** 2026-11-10: Chris chose
      independent replacements. Verified: neither history ever had a license.
      kabufuda: `components/kabufuda` (GPL-3.0-or-later); round-trip tests
      pass, real cards read identically to AxioDL's reader, and a Mac
      save-station save reloads. zeus: `components/zeus` (GPL-3.0-or-later),
      written from the declarations Metaforce calls; its probe output matches
      the original on 2,020 of 2,065 lines, the rest in functions the game
      does not use or edge cases (components/zeus/README.md). The full game
      builds on it. Mac gameplay on it verified (Gate A); iPad installed, its
      playtest is Chris's.
- [x] **Chris chooses a license** for EctoPad's own patches, scripts, and docs.
      2026-11-10: delegated; GPL-3.0-or-later (`LICENSE`).
- [ ] Corresponding source: bootstrap + patches + mirrors at exact commits
      (largely done via Gate D; publish the mirrors with the release).
- [~] soxr (LGPL-2.1) is statically linked: verified 2026-11-10, 78 soxr
      symbols in the workspace Mac binary (voice resampler in SDLBackend.cpp).
      Kept: the GPL app's complete public source (bootstrap pins soxr and its
      patch) lets anyone relink a modified soxr; see RELEASE_COMPLIANCE.md.
      Needs the release to link the exact source, and the IP-law review.
- [~] Complete license/notice inventory from the final dependency graph.
      2026-11-10: THIRD_PARTY_NOTICES.md covers the C/C++ components; the 59
      Rust crates linked through nod are listed in docs/notices/rust-crates.md
      (all permissive). 2026-11-10: `scripts/collect-licenses.sh` bundles
      every license text (153 files incl. all Rust crates and Dawn's license
      from its source commit) into the IPA (`Licenses/`) and Mac app
      (`Contents/Resources/Licenses`). Nothing is missing now that zeus
      and kabufuda are EctoPad's own code.
- [ ] Dawn builder paths: 241 `/Users/runner/work/dawn-build/...` strings come
      from the prebuilt Dawn (non-local CI paths, allowed by the package
      audit). Remaining choice: build Dawn from source with path remapping
      (`AURORA_DAWN_PROVIDER=vendor`) or accept and document.
- [~] Unsigned IPA and Mac zip built, audited (`audit-ios-package.sh`,
      `release_gate.py`), reproduced byte-identically.
      2026-11-10: the gate first FAILED on Wii common keys embedded by nod;
      fixed by blanking them at bootstrap + nod.patch guard. Rebuilt
      EctoPad-0.1.4-unsigned.ipa and EctoPad-0.1.4-macos.zip both PASS
      `release_gate.py`; the IPA passes the package audit. Byte-identical
      reproduction still to do. Local files only (/tmp/ectopad-build).
      Packaging is deterministic (same app → same IPA hash). A clean build in a
      different checkout path produced a different binary (identical strings,
      +128 bytes, different UUID). Causes found: (1) Apple's linker recorded
      archive member timestamps; `ZERO_AR_DATE=1` in the build scripts makes
      relinks byte-identical (verified). (2) The UUID hashed absolute object
      paths before `strip -S`; the final link now uses
      `-oso_prefix,<source root>/`. (3) The iOS toolchain appends flags on every
      reconfigure, so release builds must start from a fresh build folder.
      Verified 2026-11-10: two fresh worktrees at different paths produced
      byte-identical IPAs, SHA-256 `4c2dbdc7ba48f6ad4682aa6edba348ef4151f1caeaf296b0d963f4a6056ffbca` (commit `2bf3f17`); that
      IPA passes `release_gate.py`. Mac zip: two fresh builds at different paths
      were byte-identical, SHA-256 `2b198694966bef33d4197ce2b608bb36d5cfdd00fc560489a4670e1e87074c89` (commit `fb94bee`), and pass
      `release_gate.py`.
      2026-11-10, with EctoPad's own zeus and kabufuda (commit `521ed6c`): a
      fresh worktree bootstrapped from the mirrors and the main checkout (fresh
      build folder) produced identical objects and libraries and the same IPA,
      SHA-256 `5b2623c7cbb151090c0dcdd9acc5660e3d660fc0f587354017fae742f3938e08`. It passes
      `audit-ios-package.sh` and `release_gate.py` and bundles 151 license texts
      with none missing. Caveat: the main checkout's first link once came out
      different (1,008 bytes of __text); relinking the same objects gave the
      matching binary twice. Recheck at release time by building twice.
      2026-11-10 Mac zip (commit `3fefa1f` scripts): the earlier zip linked
      Homebrew's SDL3, FreeType, libpng and zstd, which players do not have.
      `build-padmint-macos.sh` now hides Homebrew so Aurora builds them in,
      and refuses any non-system library link. Result: EctoPad-0.1.4-macos.zip,
      SHA-256 `292c1268264cad20674ee77f8045e980bf692e9075ed4def3482ce08727d4e2e`, links only
      /usr/lib, passes `release_gate.py`, bundles 148 license texts (0
      missing), and the zipped app loaded the 59% save into Chozo Ruins.
      Reproducibility at commit `747c98b`: two builds from fresh build folders
      gave byte-identical zips, SHA-256
      `ae2fabf9588c6ed7d10e7874e90667ffd247b8353d41b52d8af966118ac0a56f` (passes `release_gate.py`,
      starts with audio). The `292c1268…` zip above came from a reused build
      folder whose stale Rust (nod) build pulled in extra compiler builtins;
      release builds must start from a fresh build folder. Local release
      candidates in /tmp/ectopad-release: that Mac zip and the IPA
      `5b2623c7…`.
- [ ] IP-law review of the release boundary (Chris).
- [ ] README updated; draft GitHub release prepared, not published.
- [ ] Chris marks the repository Clear in his release audit.

### Gate F: public (needs Chris' go-ahead)

- [ ] Make the source mirrors public, publish the release, verify downloads.
- [ ] Optional PadMint un-pause with `player-check.sh` before and after.
