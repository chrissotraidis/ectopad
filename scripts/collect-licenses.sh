#!/usr/bin/env bash
# Collect the license texts of everything linked into EctoPad into one folder.
# Missing texts are recorded as MISSING-*.txt so the gap stays visible.
#
#   scripts/collect-licenses.sh BUILD_DIR OUTPUT_DIR
#   (BUILD_DIR is e.g. ref/metaforce/build/ios-default)
set -euo pipefail

root="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
build="${1:?usage: scripts/collect-licenses.sh BUILD_DIR OUTPUT_DIR}"
out="${2:?usage: scripts/collect-licenses.sh BUILD_DIR OUTPUT_DIR}"
[[ "$build" = /* ]] || build="$root/$build"
m="$root/ref/metaforce"
deps="$build/_deps"
mkdir -p "$out/rust"

copy() { # name source-file
  [[ -f "$2" ]] || { echo "license text missing for $1: $2" >&2; exit 1; }
  cp "$2" "$out/$1.txt"
}
missing() { # name explanation
  printf '%s\n' "$2" > "$out/MISSING-$1.txt"
}

copy Metaforce "$m/LICENSE"
copy SunPad-GPL-3.0 "$root/ref/sunpad/LICENSE"
copy Aurora "$m/extern/aurora/LICENSE"
copy nod-MIT "$m/extern/nod/LICENSE-MIT"
copy nod-Apache-2.0 "$m/extern/nod/LICENSE-APACHE"
for c in amuse athena logvisor lzokay jbus musyx fixNES spdlog; do copy "$c" "$m/extern/$c/LICENSE"; done
copy athena-libyaml "$m/extern/athena/extern/yaml/LICENSE"
copy athena-zlib-README "$m/extern/athena/extern/zlib/README"
copy libjpeg-turbo-IJG-README "$m/extern/libjpeg-turbo/README"
copy soxr-LGPL-2.1 "$m/extern/soxr/COPYING.LGPL"
copy abseil "$deps/abseil-cpp-src/LICENSE"
copy fmt "$deps/fmt-src/LICENSE"
copy freetype "$deps/freetype-src/LICENSE.TXT"
copy freetype-FTL "$deps/freetype-src/docs/FTL.TXT"
copy imgui "$deps/imgui-src/LICENSE.txt"
copy libpng "$deps/png-src/LICENSE"
copy SDL3 "$deps/sdl-src/LICENSE.txt"
copy tracy "$deps/tracy-src/LICENSE"
copy xxhash "$deps/xxhash-src/LICENSE"
copy zstd "$deps/zstd-src/LICENSE"
copy bzip2 "$deps/nod_bzip2-src/COPYING"
copy xz-liblzma-0BSD "$deps/nod_xz-src/COPYING.0BSD"
copy xz-COPYING "$deps/nod_xz-src/COPYING"

# Dawn/Tint: the prebuilt package (encounter/dawn-build v20260603.191052, built
# from google/dawn 13abc3bc8ea2d3c2050f9e77a12d012108ceee24) ships no license
# file; this is that commit's LICENSE. Of Dawn's third_party code only abseil
# is linked (covered above), verified from the app's symbols on 2026-11-10.
copy Dawn-Tint "$root/docs/notices/Dawn-LICENSE.txt"
missing zeus "zeus (AxioDL/zeus d64e9f5): no license file or license commit exists in the recovered tree. Redistribution is not permitted without a grant."
missing kabufuda "kabufuda (AxioDL/kabufuda 9493afc): no license file or license commit exists in the recovered tree. Redistribution is not permitted without a grant."

# Rust crates statically linked through nod-ffi (normal dependencies only).
PATH="${CARGO_HOME:-$HOME/.cargo}/bin:$PATH" cargo metadata --format-version 1 \
  --filter-platform aarch64-apple-ios --manifest-path "$m/extern/nod/Cargo.toml" > "$out/.cargo.json"
python3 - "$out" <<'EOF'
import glob, json, os, shutil, sys
out = sys.argv[1]
meta = json.load(open(os.path.join(out, ".cargo.json")))
packages = {p["id"]: p for p in meta["packages"]}
nodes = {n["id"]: n for n in meta["resolve"]["nodes"]}
root = next(p["id"] for p in meta["packages"] if p["name"] == "nod-ffi")
seen, stack = set(), [root]
while stack:
    pid = stack.pop()
    if pid in seen:
        continue
    seen.add(pid)
    for dep in nodes[pid]["deps"]:
        if any(k["kind"] is None for k in dep["dep_kinds"]):
            stack.append(dep["pkg"])
gaps = []
for pid in sorted(seen, key=lambda i: packages[i]["name"]):
    p = packages[pid]
    if p["name"] in ("nod", "nod-ffi"):
        continue
    src = os.path.dirname(p["manifest_path"])
    files = [f for pat in ("LICEN[CS]E*", "COPYING*", "NOTICE*", "UNLICENSE*")
             for f in glob.glob(os.path.join(src, pat)) if os.path.isfile(f)]
    dest = os.path.join(out, "rust", f'{p["name"]}-{p["version"]}')
    os.makedirs(dest, exist_ok=True)
    if files:
        for f in files:
            shutil.copy(f, dest)
    else:
        gaps.append(p["name"])
        open(os.path.join(dest, "LICENSE-TEXT-NOT-IN-CRATE.txt"), "w").write(
            f'{p["name"]} {p["version"]} declares "{p.get("license")}" but its crate ships no license file.\n')
os.remove(os.path.join(out, ".cargo.json"))
print(f"rust crates: {len(seen) - 2}, without bundled license file: {len(gaps)} {gaps}")
EOF
echo "Collected licenses in $out ($(find "$out" -type f | wc -l | tr -d ' ') files; $(ls "$out" | grep -c '^MISSING') missing)"
