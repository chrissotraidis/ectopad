#!/usr/bin/env bash
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
APP="$ROOT/ref/metaforce/build/macos-default-relwithdebinfo/Binaries/EctoPad.app"
BINARY="$APP/Contents/MacOS/EctoPad"
EXPECTED_SHA1="1a737910b55b59c6ad91be9e3e3c43517fd52efb"

if [[ $# -lt 1 ]]; then
  echo "usage: $0 /path/to/your/Metroid-Prime-USA-Rev-2.iso [EctoPad options]" >&2
  exit 2
fi

DISC_IMAGE="$1"
shift

[[ -x "$BINARY" ]] || {
  echo "macOS build not found; run ./scripts/build-macos.sh first" >&2
  exit 1
}
[[ -f "$DISC_IMAGE" ]] || { echo "disc image not found: $DISC_IMAGE" >&2; exit 1; }

ACTUAL_SHA1="$(shasum -a 1 "$DISC_IMAGE" | awk '{print $1}')"
if [[ "$ACTUAL_SHA1" != "$EXPECTED_SHA1" ]]; then
  echo "unsupported disc image: expected Metroid Prime USA Rev 2" >&2
  echo "expected SHA-1: $EXPECTED_SHA1" >&2
  echo "actual SHA-1:   $ACTUAL_SHA1" >&2
  exit 1
fi

echo "Starting EctoPad in fullscreen. SDL controllers may be connected before or after launch."
exec "$BINARY" -l "$DISC_IMAGE" +fullscreen "$@"
