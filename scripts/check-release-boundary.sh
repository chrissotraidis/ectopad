#!/usr/bin/env bash
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
cd "$ROOT"

fail() {
  printf 'release boundary check failed: %s\n' "$*" >&2
  exit 1
}

tracked_artifacts="$(git ls-files '*.ipa' 'artifacts/SHA256SUMS.txt')"
[[ -z "$tracked_artifacts" ]] ||
  fail "public binary artifacts are tracked: $tracked_artifacts"

if rg -n '\]\([^)]*\.ipa\)' README.md artifacts docs >/dev/null; then
  fail "documentation contains a direct IPA link"
fi

rg -q 'Public IPA \| \*\*Not available\*\*' README.md ||
  fail "README does not mark the public IPA as unavailable"
rg -q 'There is no public IPA right now' README.md ||
  fail "README FAQ does not explain the missing IPA"
rg -q 'No EctoPad IPA is currently distributed' artifacts/README.md ||
  fail "artifact boundary document is missing"

printf 'release boundary check passed\n'
