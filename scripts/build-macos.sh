#!/usr/bin/env bash
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
SOURCE="$ROOT/ref/metaforce"
PRESET="macos-default-relwithdebinfo"
APP="$SOURCE/build/$PRESET/Binaries/EctoPad.app"
BINARY="$APP/Contents/MacOS/EctoPad"
CONTROLLER_TEST="$SOURCE/build/$PRESET/Binaries/controller_slots_tests"
ICON_SOURCE="$ROOT/assets/app-icon/EctoPad-AppIcon-1024.png"
ICON_OUTPUT="$SOURCE/Runtime/platforms/macos/mainicon.icns"

if [[ ! -f "$SOURCE/CMakePresets.json" ]]; then
  echo "Metaforce source is missing at $SOURCE" >&2
  echo "Bootstrap the pinned ref/ workspace and apply the tracked EctoPad patches first." >&2
  exit 1
fi

python3 -c \
  'from PIL import Image; import sys; Image.open(sys.argv[1]).convert("RGBA").save(sys.argv[2], format="ICNS")' \
  "$ICON_SOURCE" "$ICON_OUTPUT"

if [[ -n "${ECTOPAD_BUILD_JOBS:-}" ]]; then
  BUILD_JOBS="$ECTOPAD_BUILD_JOBS"
else
  BUILD_JOBS="$(sysctl -n hw.logicalcpu 2>/dev/null || getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)"
fi

(
  cd "$SOURCE"
  cmake --preset "$PRESET"
  cmake --build --preset "$PRESET" --target metaforce controller_slots_tests --parallel "$BUILD_JOBS"
)

[[ -x "$BINARY" ]] || { echo "macOS executable not found: $BINARY" >&2; exit 1; }
BINARY_DESCRIPTION="$(file "$BINARY")"
[[ "$BINARY_DESCRIPTION" == *"Mach-O 64-bit executable arm64"* ]]
[[ "$(plutil -extract CFBundlePackageType raw "$APP/Contents/Info.plist")" == "APPL" ]]
"$CONTROLLER_TEST"

echo "macOS app: $APP"
echo "Verified: native arm64 app bundle and 5 controller slot/reconnect checks."
echo "Run it with: ./scripts/run-macos.sh /path/to/your/game.iso"
