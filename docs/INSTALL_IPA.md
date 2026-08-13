# Installing the iOS build

Last updated: 2026-08-12

There are two artifacts, and they are not interchangeable.

## Unsigned validation IPA

`scripts/package-ios.sh` creates an **unsigned local validation IPA**. It is
not installable as-is and is not approved for public redistribution.

The exact final validation artifact is
`/tmp/Metaforce-unsigned-menu-audio-2026-08-12.ipa`, SHA-256
`308f9e26861327b42e28359406237902f8b9ab60e30ba3c8751419055111617d`.
It was packaged twice with identical bytes and passed the repository audit.
Do not install this archive on the iPad.

## Physical iPad path (what actually launched)

A later 2026-08-12 session development-signed the **source** app and installed
it in place on the attached iPad:

- app: `ref/metaforce/build/ios-default/Binaries/Metaforce.app`
- bundle ID: `com.axiodl.Metaforce`
- sign the source `.app`, then `codesign --verify --deep --strict`
- `xcrun devicectl device install app --device <UDID> <app-path>`
- `xcrun devicectl device process launch --device <UDID> --terminate-existing com.axiodl.Metaforce`

That launch is Gate 2 proof only. Gate 3 failed; see
[TECH-DEBT.md](TECH-DEBT.md). Preserve the live container. Back up
`Documents` and `Library` separately. Never use
`--remove-existing-content` without explicit authorization and a tested
restore.

EctoPad does not yet have `scripts/bootstrap-dependencies.sh` or
`scripts/deploy-ios-device.sh`. `ref/sunpad/scripts/` is the local reference
for that missing front door. First-time `ios-default` configure also needs
Rust plus Apple targets, Dawn, and the vendored audio stack; those are real
dependencies, not optional extras. See [BUILDING.md](BUILDING.md).

Metaforce never bundles Metroid Prime data. After installing a signed build,
use `••• → Game Data & Saves` to import a user-owned Metroid Prime NTSC-U
Rev 2 disc image. The importer validates the expected revision and hash
before atomically activating the private copy. On the current iPad build the
`•••` menu itself is an open P1 defect, so import may need the already-
imported container rather than assuming the menu is complete.

Do not share an IPA that contains game data, saves, logs, provisioning
profiles, private keys, certificates, or other signing material. Public binary
distribution also remains blocked until the GPL corresponding-source and LGPL
relink requirements recorded in `THIRD_PARTY_NOTICES.md` are fulfilled.
