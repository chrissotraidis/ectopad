#!/usr/bin/env bash
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
IPA="${1:?usage: scripts/audit-ios-package.sh <Metaforce-unsigned.ipa>}"
[[ "$IPA" = /* ]] || IPA="$ROOT/$IPA"

fail() {
  echo "iOS package audit failed: $*" >&2
  exit 1
}

[[ -f "$IPA" ]] || fail "IPA not found: $IPA"
unzip -tq "$IPA" >/dev/null || fail "ZIP integrity check failed"

entries="$(unzip -Z1 "$IPA")"
for required in \
  Payload/Metaforce.app/Metaforce \
  Payload/Metaforce.app/Info.plist \
  Payload/Metaforce.app/LICENSE-Metaforce.txt \
  Payload/Metaforce.app/COPYING-GPL-3.0.txt \
  Payload/Metaforce.app/THIRD_PARTY_NOTICES.md \
  Payload/Metaforce.app/INSTALL_IPA.md; do
  grep -Fxq "$required" <<<"$entries" || fail "missing required entry: $required"
done

if grep -Eq '(^|/)\.\.(/|$)|^/|(^|/)__MACOSX(/|$)' <<<"$entries"; then
  fail "archive contains an unsafe path or AppleDouble metadata"
fi
if grep -Eiq '(^|/)\.DS_Store$|\.(iso|gcm|rvz|wia|wbfs|gcz|nkit|gci|sav|raw|pak|log|mobileprovision|p12|p8|pem|key|cer|crt)$|(^|/)_CodeSignature(/|$)' <<<"$entries"; then
  fail "archive contains game data, a save, a log, or signing material"
fi

extract_root="$(mktemp -d /tmp/metaforce-ipa-audit.XXXXXX)"
trap 'rm -rf "$extract_root"' EXIT
unzip -q "$IPA" -d "$extract_root"
app="$extract_root/Payload/Metaforce.app"
executable="$app/Metaforce"
plist="$app/Info.plist"

[[ "$(find "$extract_root/Payload" -mindepth 1 -maxdepth 1 -type d -name '*.app' | wc -l | tr -d ' ')" = 1 ]] ||
  fail "IPA must contain exactly one app"
[[ "$(lipo -archs "$executable")" = arm64 ]] || fail "app is not arm64-only"
vtool -show-build "$executable" | grep -Eq 'platform +IOS$' || fail "app is not an iPhoneOS product"

bundle_id="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleIdentifier' "$plist")"
short_version="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleShortVersionString' "$plist")"
build_version="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleVersion' "$plist")"
minimum_os="$(/usr/libexec/PlistBuddy -c 'Print :MinimumOSVersion' "$plist")"
[[ "$bundle_id" = com.axiodl.Metaforce ]] || fail "unexpected bundle identifier: $bundle_id"
[[ "$short_version" =~ ^[0-9]+(\.[0-9]+){0,2}$ ]] || fail "invalid marketing version: $short_version"
[[ "$build_version" =~ ^[1-9][0-9]*(\.[0-9]+){0,2}$ ]] || fail "invalid build version: $build_version"
[[ "$minimum_os" =~ ^[0-9]+\.[0-9]+$ ]] || fail "invalid minimum OS: $minimum_os"
[[ "$(/usr/libexec/PlistBuddy -c 'Print :UIFileSharingEnabled' "$plist")" = true ]] ||
  fail "Files-visible Documents sharing is not enabled"
[[ "$(/usr/libexec/PlistBuddy -c 'Print :LSSupportsOpeningDocumentsInPlace' "$plist")" = true ]] ||
  fail "opening Documents in place is not enabled"
[[ "$(/usr/libexec/PlistBuddy -c 'Print :UIApplicationSupportsIndirectInputEvents' "$plist")" = true ]] ||
  fail "indirect controller input is not enabled"

[[ ! -d "$app/_CodeSignature" && ! -f "$app/embedded.mobileprovision" ]] ||
  fail "unsigned validation package contains signing material"
codesign --verify --strict "$app" >/dev/null 2>&1 && fail "validation app is still signed"

if otool -L "$executable" | tail -n +2 | grep -Eq '/Users/|/Volumes/|/opt/homebrew|/usr/local'; then
  fail "app links a host-only or personal-path library"
fi

while IFS= read -r -d '' file; do
  size="$(stat -f %z "$file")"
  (( size < 1073741824 )) || fail "suspiciously large bundled file: ${file#"$app/"}"
  if [[ "$size" -ge 32 ]]; then
    header_id_hex="$(dd if="$file" bs=1 count=6 2>/dev/null | od -An -tx1 | tr -d ' \n')"
    header_magic="$(dd if="$file" bs=1 skip=28 count=4 2>/dev/null | od -An -tx1 | tr -d ' \n')"
    [[ "$header_id_hex" != 474d38453031 && "$header_magic" != c2339f3d ]] ||
      fail "bundled file has a Metroid Prime/GameCube disc header: ${file#"$app/"}"
  fi
done < <(find "$app" -type f -print0)

builder_home="${HOME:-}"
for file in "$executable" "$plist"; do
  strings_output="$(LC_ALL=C strings -a "$file")"
  if [[ -n "$builder_home" ]] && grep -F "$builder_home/" <<<"$strings_output" >/dev/null; then
    fail "package contains the current builder's home path: ${file#"$app/"}"
  fi
  if grep -E 'github_pat_|gh[pousr]_|AKIA[0-9A-Z]{16}|BEGIN [A-Z ]*PRIVATE KEY' <<<"$strings_output" >/dev/null; then
    fail "package contains likely credential material: ${file#"$app/"}"
  fi
  if grep -E '^/Volumes/' <<<"$strings_output" >/dev/null; then
    fail "package contains a local volume path: ${file#"$app/"}"
  fi
  unexpected_user_paths="$(grep -E '^/Users/' <<<"$strings_output" | grep -Ev '^/Users/runner/work/dawn-build/' || true)"
  [[ -z "$unexpected_user_paths" ]] ||
    fail "package contains a non-Dawn user path: ${file#"$app/"}"
done

upstream_ci_paths="$(LC_ALL=C strings -a "$executable" | grep -E '^/Users/runner/work/' | sort -u | wc -l | tr -d ' ')"

echo "iOS unsigned validation package audit passed: $IPA"
echo "Bundle: $bundle_id $short_version ($build_version), minimum iOS $minimum_os"
echo "Upstream Dawn CI source paths (non-local provenance): $upstream_ci_paths"
echo "Public redistribution is not approved; see THIRD_PARTY_NOTICES.md."
