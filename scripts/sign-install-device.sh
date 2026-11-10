#!/usr/bin/env bash
# Sign the built iOS app with a local Apple Development identity and install it
# IN PLACE on an attached device. Never uninstalls (that would delete imported
# game data and saves). Signing material is supplied by the caller and is never
# copied into the repository.
#
# usage: ECTOPAD_PROFILE=/path/to.mobileprovision ECTOPAD_IDENTITY=<SHA-1> \
#        scripts/sign-install-device.sh <device-udid> [app-path]
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
DEVICE="${1:?usage: $0 <device-udid> [app-path]}"
APP="${2:-$ROOT/ref/metaforce/build/ios-default/Binaries/Metaforce.app}"
PROFILE="${ECTOPAD_PROFILE:?set ECTOPAD_PROFILE to a provisioning profile covering com.axiodl.Metaforce}"
IDENTITY="${ECTOPAD_IDENTITY:?set ECTOPAD_IDENTITY to an Apple Development identity SHA-1}"
BUNDLE_ID="com.axiodl.Metaforce"

[[ -d "$APP" ]] || { echo "app not found: $APP" >&2; exit 1; }
WORK="$(mktemp -d "${TMPDIR:-/tmp}/ectopad-sign.XXXXXX")"
trap 'rm -rf "$WORK"' EXIT

security cms -D -i "$PROFILE" > "$WORK/profile.plist"
TEAM="$(/usr/libexec/PlistBuddy -c 'Print :TeamIdentifier:0' "$WORK/profile.plist")"
cat > "$WORK/entitlements.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>application-identifier</key><string>$TEAM.$BUNDLE_ID</string>
  <key>com.apple.developer.team-identifier</key><string>$TEAM</string>
  <key>get-task-allow</key><true/>
</dict>
</plist>
EOF

STAGED="$WORK/$(basename "$APP")"
ditto "$APP" "$STAGED"
[[ "$(/usr/libexec/PlistBuddy -c 'Print :CFBundleIdentifier' "$STAGED/Info.plist")" == "$BUNDLE_ID" ]]
cp "$PROFILE" "$STAGED/embedded.mobileprovision"
codesign --force --sign "$IDENTITY" --entitlements "$WORK/entitlements.plist" --timestamp=none "$STAGED"
codesign --verify --deep --strict "$STAGED"
echo "Signed $(basename "$APP") for team $TEAM"

xcrun devicectl device install app --device "$DEVICE" "$STAGED"
echo "Installed in place on $DEVICE. Launch with:"
echo "  xcrun devicectl device process launch --device $DEVICE --terminate-existing $BUNDLE_ID"
