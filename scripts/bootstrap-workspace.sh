#!/bin/bash
# Recreate EctoPad's ignored ref/metaforce workspace from a fresh clone.
#
# Metaforce and every component are fetched at the commits EctoPad was built
# and tested with, then patches/workspace/<component>.patch restores EctoPad's
# changes exactly (captured from the maintained workspace). Git checks each
# commit by its hash, so mirrors yield identical sources. AxioDL made Metaforce,
# zeus, amuse and kabufuda private in 2026; their pinned commits were recovered
# from Software Heritage (docs/SOURCE_RECOVERY.md) and are fetched from
# EctoPad's chrissotraidis/ectopad-* repositories, which also mirror the AxioDL
# repositories that are still public.
#
#   scripts/bootstrap-workspace.sh
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
ref="$root/ref/metaforce"
patches="$root/patches/workspace"
marker="$ref/.ectopad-workspace"
want="$(cat "$patches"/*.patch | shasum -a 256 | cut -d' ' -f1)"

# EctoPad's touch shell comes from SunPad (GPL-3.0); its license ships in the app.
if [ ! -f "$root/ref/sunpad/LICENSE" ]; then
  echo "EctoPad: fetching SunPad for its license"
  git init -q "$root/ref/sunpad"
  git -C "$root/ref/sunpad" remote add origin https://github.com/chrissotraidis/sunpad.git
  git -C "$root/ref/sunpad" fetch -q --depth 1 origin 24911e836ad9c2ece881b11bf426641abeaf374a
  git -C "$root/ref/sunpad" checkout -q --detach FETCH_HEAD
fi

if [ -f "$marker" ] && [ "$(cat "$marker")" = "$want" ]; then
  echo "EctoPad workspace is ready at $ref"
  exit 0
fi
if [ -e "$ref" ]; then
  echo "ref/metaforce exists but was not made by this script for these patches; move it aside first." >&2
  exit 1
fi

fetch() { # directory url commit
  git init -q "$1"
  git -C "$1" remote add origin "$2"
  git -C "$1" fetch -q --filter=blob:none origin "$3"
  git -C "$1" checkout -q --detach "$3"
}

echo "EctoPad: fetching Metaforce and its components"
mirror="https://github.com/chrissotraidis/ectopad"
fetch "$ref" "$mirror-metaforce.git" 621ee0fed3fdd5fccbbdadbce33343f29dce071c
for component in zeus kabufuda jbus musyx libjpeg-turbo; do
  git -C "$ref" config "submodule.extern/$component.url" "$mirror-$component.git"
done
git -C "$ref" submodule update -q --init --recursive

ext="$ref/extern"
fetch "$ext/amuse" "$mirror-amuse.git" ad9bc96af472dae03fde215163446bdec80be67f
fetch "$ext/athena" https://github.com/encounter/athena.git 6adba82abd8b7f48e6eca9f74361d0c605bc9ec8
git -C "$ext/athena" submodule update -q --init --recursive
fetch "$ext/logvisor" "$mirror-logvisor.git" 208a8c1f84f9968695d712b8e5625c0dc85edbae
fetch "$ext/lzokay" "$mirror-lzokay.git" db2df1fcbebc2ed06c10f727f72567d40f06a2be
fetch "$ext/soxr" https://github.com/chirlu/soxr.git 945b592b70470e29f917f4de89b4281fbbd540c0

echo "EctoPad: applying the EctoPad changes"
apply() { # directory patch
  if [ -s "$patches/$2.patch" ]; then
    git -C "$1" apply --whitespace=nowarn "$patches/$2.patch"
  fi
}
apply "$ref" metaforce
for component in aurora kabufuda nod amuse athena logvisor lzokay soxr; do
  apply "$ext/$component" "$component"
done

# Nintendo's Wii common keys are not part of EctoPad. nod embeds them only to
# decrypt Wii disc tickets; EctoPad reads GameCube discs only, and nod.patch
# makes nod refuse Wii decryption once the key rows are blank.
scrub_wii_keys() {
  local wii_rs="$ext/nod/nod/src/disc/wii.rs"
  perl -pi -e 'if (/static (?:RETAIL|DEBUG)_COMMON_KEYS/ .. /^\];/) { s/\[0x[0-9a-f]{2}(?:, 0x[0-9a-f]{2}){15}\]/[0; 16]/ }' "$wii_rs"
  if [ "$(grep -c '\[0; 16\],' "$wii_rs")" != 6 ] || grep -Eq '^ *\[0x[0-9a-f]{2}(, 0x[0-9a-f]{2}){15}\],' "$wii_rs"; then
    echo "EctoPad: could not blank nod's Wii common keys in $wii_rs" >&2
    exit 1
  fi
}
scrub_wii_keys

echo "$want" > "$marker"
echo "EctoPad workspace is ready at $ref"

