#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOURCE_DIR="$ROOT/assets/app-icon"
TARGET_DIR="$ROOT/ref/metaforce/Runtime/platforms/ios"
CATALOG_DIR="$SOURCE_DIR/Assets.xcassets/AppIcon.appiconset"
CATALOG_MASTER="$CATALOG_DIR/EctoPad-AppIcon-1024.png"

for file in AppIcon60x60@2x.png 'AppIcon76x76@2x~ipad.png'; do
  source_path="$SOURCE_DIR/$file"
  [[ -f "$source_path" ]] || {
    echo "Missing tracked app icon: $source_path" >&2
    exit 1
  }
done

[[ -d "$TARGET_DIR" ]] || {
  echo "Missing Metaforce iOS resources: $TARGET_DIR" >&2
  echo "Bootstrap the ignored ref/metaforce checkout first." >&2
  exit 1
}

cp "$SOURCE_DIR/EctoPad-AppIcon-1024.png" "$CATALOG_MASTER"

compile_dir="$(mktemp -d "${TMPDIR:-/tmp}/ectopad-app-icon.XXXXXX")"
trap 'rm -rf "$compile_dir"' EXIT
partial_plist="$compile_dir/AppIcon-Info.plist"
xcrun actool \
  --compile "$compile_dir" \
  --output-partial-info-plist "$partial_plist" \
  --platform iphoneos \
  --minimum-deployment-target 14.0 \
  --target-device iphone \
  --target-device ipad \
  --app-icon AppIcon \
  --compress-pngs \
  "$SOURCE_DIR/Assets.xcassets" >/dev/null

[[ -f "$compile_dir/Assets.car" ]] || {
  echo "actool did not produce Assets.car" >&2
  exit 1
}

cp "$SOURCE_DIR/AppIcon60x60@2x.png" "$TARGET_DIR/AppIcon60x60@2x.png"
cp "$SOURCE_DIR/AppIcon76x76@2x~ipad.png" "$TARGET_DIR/AppIcon76x76@2x~ipad.png"
cp "$compile_dir/Assets.car" "$TARGET_DIR/Assets.car"

echo "Synced EctoPad icon PNGs and Assets.car into ref/metaforce."
