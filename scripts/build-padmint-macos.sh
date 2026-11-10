#!/usr/bin/env bash
# PadMint's Mac build of EctoPad, from a fresh clone: the exact Metaforce
# workspace EctoPad is tested with, EctoPad's app icon, the native Apple Silicon
# EctoPad.app and a zip of it. No game data is read: players choose their own
# Metroid Prime (USA, Rev 2) disc with Select Game when EctoPad opens.
#
#   scripts/build-padmint-macos.sh OUTPUT.zip
set -euo pipefail
# Reproducible links: Apple's linker otherwise records archive member
# timestamps, so identical sources would produce different binaries.
export ZERO_AR_DATE=1

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:?usage: scripts/build-padmint-macos.sh OUTPUT.zip}"
jobs="${ECTOPAD_BUILD_JOBS:-$(sysctl -n hw.ncpu)}"
cd "$root"
# EctoPad's own version (VERSION), so the app, its release and PadMint agree.
version="$(tr -d '[:space:]' < VERSION)"
rustup_bin="${CARGO_HOME:-$HOME/.cargo}/bin"
if [[ -x "$rustup_bin/rustup" ]]; then
  export PATH="$rustup_bin:$PATH"
fi
if ! command -v cargo >/dev/null; then
  echo "EctoPad needs Rust. Run: brew install rustup && rustup-init -y" >&2
  exit 1
fi

/bin/bash scripts/bootstrap-workspace.sh

echo "EctoPad: making the Mac app icon"
iconset="$(mktemp -d "${TMPDIR:-/tmp}/ectopad-icon.XXXXXX")/EctoPad.iconset"
mkdir -p "$iconset"
for size in 16 32 128 256 512; do
  sips -z "$size" "$size" assets/app-icon/EctoPad-AppIcon-1024.png --out "$iconset/icon_${size}x${size}.png" >/dev/null
  double=$((size * 2))
  sips -z "$double" "$double" assets/app-icon/EctoPad-AppIcon-1024.png --out "$iconset/icon_${size}x${size}@2x.png" >/dev/null
done
iconutil -c icns "$iconset" -o ref/metaforce/Runtime/platforms/macos/mainicon.icns

preset=macos-default-relwithdebinfo
echo "EctoPad: configuring and building the Mac app"
(cd ref/metaforce && cmake --preset "$preset" -DMETAFORCE_BUNDLE_SHORT_VERSION="$version" && cmake --build --preset "$preset" --target metaforce --parallel "$jobs")

app="$root/ref/metaforce/build/$preset/Binaries/EctoPad.app"
binary="$app/Contents/MacOS/EctoPad"
[[ -x "$binary" ]] || { echo "EctoPad Mac app not found: $app" >&2; exit 1; }
[[ "$(file "$binary")" == *"Mach-O 64-bit executable arm64"* ]] || { echo "EctoPad is not a native arm64 app" >&2; exit 1; }
for pattern in '*.iso' '*.gcm' '*.ciso' '*.rvz' '*.wbfs' '*.gci' '*.mobileprovision' '*.p12' '*.cer' '*.key'; do
  forbidden="$(find "$app" -type f -iname "$pattern" -print -quit)"
  if [[ -n "$forbidden" ]]; then
    echo "Refusing app containing private game data or signing material: $forbidden" >&2
    exit 1
  fi
done

echo "EctoPad: signing for this Mac and packaging"
codesign --force --deep --sign - "$app"
codesign --verify --deep --strict "$app"
rm -f "$output"
# Reproducible archive: every file carries the commit's time, and the zip holds
# no resource forks, extended attributes or quarantine flags.
stamp="$(TZ=UTC git -C "$root" log -1 --format=%cd --date=format-local:%Y%m%d%H%M.%S)"
find "$app" -exec env TZ=UTC touch -h -t "$stamp" {} +
ditto -c -k --norsrc --noextattr --noqtn --keepParent "$app" "$output"
