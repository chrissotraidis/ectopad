#!/usr/bin/env bash
# Back up EctoPad's memory cards and preferences from an attached iPhone/iPad
# before installing a new build. Uses libimobiledevice's afcclient (house_arrest),
# which keeps working when `devicectl device copy` hangs.
#
#   brew install libimobiledevice
#   scripts/backup-device-saves.sh <device-udid> <backup-folder>
set -euo pipefail

DEVICE="${1:?usage: $0 <device-udid> <backup-folder>}"
DEST="${2:?usage: $0 <device-udid> <backup-folder>}"
BUNDLE_ID="com.axiodl.Metaforce"
GC="Library/Application Support/dolphin-emu/GC"
command -v afcclient >/dev/null || { echo "afcclient not found: brew install libimobiledevice" >&2; exit 1; }

mkdir -p "$DEST"
cd "$DEST"
printf '%s\n' \
  "get \"$GC/MemoryCardA.USA.raw\" MemoryCardA.USA.raw" \
  "get \"$GC/MemoryCardB.USA.raw\" MemoryCardB.USA.raw" \
  "get Library/Preferences/$BUNDLE_ID.plist $BUNDLE_ID.plist" \
  quit | afcclient -u "$DEVICE" --container "$BUNDLE_ID" >/dev/null

# A memory card image is 16 MiB; anything else means the copy did not happen.
for card in MemoryCardA.USA.raw MemoryCardB.USA.raw; do
  [[ -f "$card" && "$(stat -f %z "$card")" == 16777216 ]] || { echo "backup of $card failed" >&2; exit 1; }
done
shasum -a 256 MemoryCard*.USA.raw
echo "Saves backed up to $DEST"
