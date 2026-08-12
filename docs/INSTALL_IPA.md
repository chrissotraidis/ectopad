# Installing the iOS build

The packaging script currently creates an **unsigned local validation IPA**.
It is not installable as-is and is not approved for public redistribution.

To run on a physical iPhone or iPad, build and sign `Metaforce.app` with an
Apple development identity and provisioning profile for the target device,
then package the signed app without stripping or otherwise modifying it after
signing. This machine currently has no valid code-signing identity and no
connected physical device, so that workflow remains unverified.

Metaforce never bundles Metroid Prime data. After installing a signed build,
use the unchanged SunPad **... → Game Data & Saves** interface to import a
user-owned Metroid Prime NTSC-U Rev 2 disc image. The importer validates the
expected revision and hash before atomically activating the private copy.

Do not share an IPA that contains game data, saves, logs, provisioning
profiles, private keys, certificates, or other signing material. Public binary
distribution also remains blocked until the GPL corresponding-source and LGPL
relink requirements recorded in `THIRD_PARTY_NOTICES.md` are fulfilled.
