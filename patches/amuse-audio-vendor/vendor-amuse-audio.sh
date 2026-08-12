#!/bin/bash
# Vendors the amuse audio engine + Hecl deps into ref/metaforce/extern/ for the
# Metaforce Apple port (in-game audio via amuse, see docs/KNOWN_ISSUES KI-003).
#
# Usage: patches/amuse-audio-vendor/vendor-amuse-audio.sh
# Run from the ectopad repo root. Requires network access; no LLVM needed
# (the pre-generated atdna DNA sources in generated/ are used instead).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
EXT="$ROOT/ref/metaforce/extern"
PATCHDIR="$ROOT/patches/amuse-audio-vendor"

declare -A PINS=(
  [amuse]=ad9bc96af472dae03fde215163446bdec80be67f
  [athena]=6adba82abd8b7f48e6eca9f74361d0c605bc9ec8
  [logvisor]=208a8c1f84f9968695d712b8e5625c0dc85edbae
  [lzokay]=db2df1fcbebc2ed06c10f727f72567d40f06a2be
)

clone() {
  local name="$1" url="$2"
  if [ -d "$EXT/$name/.git" ]; then
    echo "  $name already present"
    return
  fi
  git clone --filter=blob:none "$url" "$EXT/$name"
  git -C "$EXT/$name" checkout --quiet "${PINS[$name]}"
}

echo "Vendoring amuse audio stack into $EXT"

clone amuse "https://github.com/AxioDL/amuse.git"
clone athena "https://github.com/encounter/athena.git"
clone logvisor "https://github.com/AxioDL/logvisor.git"
clone lzokay "https://github.com/AxioDL/lzokay.git"

# athena's extern submodules (fmt, lzokay, zlib, yaml)
git -C "$EXT/athena" submodule update --init --depth 1

echo "Applying patches..."
for name in amuse athena logvisor lzokay; do
  if [ -s "$PATCHDIR/$name.patch" ]; then
    git -C "$EXT/$name" apply "$PATCHDIR/$name.patch"
    echo "  applied $name.patch"
  fi
done

echo "Installing pre-generated atdna DNA sources..."
mkdir -p "$EXT/amuse/generated"
cp "$PATCHDIR"/generated/atdna_*.cpp "$EXT/amuse/generated/"

# amuse's CMakeLists references generated/ sources and skips target_atdna;
# athena's atdna tool is skipped when LLVM is absent.
echo "Done. Reconfigure the metaforce build (cmake --preset ...) and rebuild."
