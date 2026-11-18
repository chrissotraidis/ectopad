#!/usr/bin/env bash
# PadMint's iPhone/iPad build of EctoPad, from a fresh clone: the exact
# Metaforce workspace EctoPad is tested with, EctoPad's own app icon, the iOS
# device app and an IPA to sign with your own Apple ID. No game data is read:
# players import their own Metroid Prime (USA, Rev 2) disc image inside EctoPad.
#
#   scripts/build-padmint-ios.sh OUTPUT.ipa
set -euo pipefail
# Reproducible links: Apple's linker otherwise records archive member
# timestamps, so identical sources would produce different binaries.
export ZERO_AR_DATE=1

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:?usage: scripts/build-padmint-ios.sh OUTPUT.ipa}"
jobs="${ECTOPAD_BUILD_JOBS:-$(sysctl -n hw.ncpu)}"
cd "$root"
# EctoPad's own version (VERSION), so the app, its release and PadMint agree.
version="$(tr -d '[:space:]' < VERSION)"

# Metaforce's disc reader (nod) is Rust and must build for iPhone, which needs
# rustup's toolchain with the aarch64-apple-ios target. Homebrew's own rust has
# no iPhone standard library, so rustup's tools go first.
rustup_bin="${CARGO_HOME:-$HOME/.cargo}/bin"
if [[ -x "$rustup_bin/rustup" ]]; then
  export PATH="$rustup_bin:$PATH"
fi
if ! command -v rustup >/dev/null || ! rustup target list --installed | grep -qx aarch64-apple-ios; then
  echo "EctoPad needs Rust from rustup with the iPhone target. Run: brew install rustup && rustup-init -y && rustup target add aarch64-apple-ios" >&2
  exit 1
fi


/bin/bash scripts/bootstrap-workspace.sh
bash scripts/sync-app-icon.sh
echo "EctoPad: configuring and building the iOS app"
(cd ref/metaforce && cmake --preset ios-default -DMETAFORCE_BUNDLE_SHORT_VERSION="$version" && cmake --build --preset ios-default --target metaforce -j "$jobs")
echo "EctoPad: packaging"
bash scripts/package-ios.sh "$root/ref/metaforce/build/ios-default/Binaries/Metaforce.app" "$output"

