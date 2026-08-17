# Installing the iOS build

Last updated: 2026-08-17

There are two artifacts, and they are not interchangeable.

## Unsigned validation IPA

`scripts/package-ios.sh` creates a deterministic **unsigned local validation
IPA** from the iPhoneOS `.app`. It is game-data-free but not installable as-is:
the person installing it must sign it with an Apple identity and provisioning
profile valid for their device. It is not approved for public redistribution
until the corresponding-source and LGPL relink package described below is
complete.

## Physical iPhone/iPad path (what actually launched)

The physically tested workflow development-signs the **source** app and installs
it in place on the target iPhone or iPad:

- app: `ref/metaforce/build/ios-default/Binaries/Metaforce.app`
- bundle ID: `com.axiodl.Metaforce`
- sign the source `.app`, then `codesign --verify --deep --strict`
- `xcrun devicectl device install app --device <UDID> <app-path>`
- `xcrun devicectl device process launch --device <UDID> --terminate-existing com.axiodl.Metaforce`

Install updates in place under the existing `com.axiodl.Metaforce` bundle ID.
Do not uninstall the previous app or use `--remove-existing-content`: either
operation can destroy the locally imported ISO, memory cards, controller/touch
preferences, and other settings. Preserve and, when possible, back up
`Documents` and `Library` separately.

Build/install/launch evidence is not gameplay acceptance. EctoPad 0.1.3 has two
confirmed P0 visual defects on physical iPhone and iPad: selective black world
geometry and the first tutorial door remaining visibly closed after its logical
open/collision completes. See [TECH-DEBT.md](TECH-DEBT.md).

EctoPad does not yet have `scripts/bootstrap-dependencies.sh` or
`scripts/deploy-ios-device.sh`. `ref/sunpad/scripts/` is the local reference
for that missing front door. First-time `ios-default` configure also needs
Rust plus Apple targets, Dawn, and the vendored audio stack; those are real
dependencies, not optional extras. See [BUILDING.md](BUILDING.md).

Metaforce never bundles Metroid Prime data. After installing a signed build,
use `••• → Game Data & Saves` to import a user-owned Metroid Prime NTSC-U
Rev 2 disc image. The importer validates the expected revision and hash
before atomically activating the private copy. The current native menu also
exposes **Share Diagnostic Log** for privacy-bounded runtime evidence.

Do not share an IPA that contains game data, saves, logs, provisioning
profiles, private keys, certificates, or other signing material. Public binary
distribution also remains blocked until the GPL corresponding-source and LGPL
relink requirements recorded in `THIRD_PARTY_NOTICES.md` are fulfilled.
