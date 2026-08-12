#!/usr/bin/env bash
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
APP="${1:-$ROOT/ref/metaforce/build/ios-default/Binaries/Metaforce.app}"
OUTPUT="${2:-/tmp/Metaforce-unsigned-validation.ipa}"
[[ "$APP" = /* ]] || APP="$ROOT/$APP"
[[ "$OUTPUT" = /* ]] || OUTPUT="$ROOT/$OUTPUT"

[[ -d "$APP" ]] || { echo "app not found: $APP" >&2; exit 1; }
[[ -f "$APP/Metaforce" ]] || { echo "app executable not found: $APP/Metaforce" >&2; exit 1; }
[[ -f "$ROOT/ref/metaforce/LICENSE" ]] || { echo "Metaforce license checkout is missing" >&2; exit 1; }
[[ -f "$ROOT/ref/sunpad/LICENSE" ]] || { echo "SunPad GPL license checkout is missing" >&2; exit 1; }

package_root="$(mktemp -d /tmp/metaforce-package.XXXXXX)"
trap 'rm -rf "$package_root"' EXIT
staged_app="$package_root/Payload/Metaforce.app"
mkdir -p "$(dirname "$staged_app")" "$(dirname "$OUTPUT")"
ditto --norsrc "$APP" "$staged_app"

# This command deliberately creates an unsigned local validation package.
# Signing must happen after all stripping and package mutations in a separate,
# physical-device release workflow.
rm -rf "$staged_app/_CodeSignature"
rm -f "$staged_app/embedded.mobileprovision"
codesign --remove-signature "$staged_app" 2>/dev/null || true
strip -S "$staged_app/Metaforce"

cp "$ROOT/ref/metaforce/LICENSE" "$staged_app/LICENSE-Metaforce.txt"
cp "$ROOT/ref/sunpad/LICENSE" "$staged_app/COPYING-GPL-3.0.txt"
cp "$ROOT/THIRD_PARTY_NOTICES.md" "$staged_app/THIRD_PARTY_NOTICES.md"
cp "$ROOT/docs/INSTALL_IPA.md" "$staged_app/INSTALL_IPA.md"
xattr -cr "$package_root"

find "$package_root/Payload" -exec touch -h -t 200001010000 {} +
temporary_ipa="$package_root/Metaforce.ipa"
(
  cd "$package_root"
  find Payload \( -type f -o -type l \) -print | LC_ALL=C sort |
    zip -X -q -y "$temporary_ipa" -@
)
mv -f "$temporary_ipa" "$OUTPUT"

"$ROOT/scripts/audit-ios-package.sh" "$OUTPUT"
echo "IPA: $OUTPUT"
shasum -a 256 "$OUTPUT"
echo "This local validation IPA is unsigned and must not be publicly redistributed."
