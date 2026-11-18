# EctoPad math library (zeus replacement)

This is EctoPad's own vector, matrix, quaternion, color and bounding-volume
library. It replaces AxioDL's zeus, whose recovered history (354 commits) never
carried a license, so it could not be redistributed.

It keeps the `zeus::` names, signatures and memory layouts that Metaforce
uses, so Metaforce builds against it unchanged: four-lane 16-byte vectors with
unused lanes zeroed, column-major matrices, transforms as a 3x3 basis plus an
origin, quaternions stored w, x, y, z. The code was written for EctoPad from
the declarations Metaforce calls (names and signatures only, read from the
pinned headers with clang's declaration dump) and from standard math. Where the
original's results are unusual, the behavior was established by black-box
tests that compare outputs, never by reading its implementation.

License: GPL-3.0-or-later, like the rest of EctoPad (see the repository
`LICENSE`).

## How it was checked (2026-11-10)

`tests/zeus_probe.cpp` drives every public function with fixed inputs and
prints the results. Built against this library and against the original, the
two outputs agree on 2,020 of 2,065 lines (to 0.02% relative). The 45 lines
that differ are:

- functions Metaforce never calls: `CColor::fromHSL`, `CColor::Clamp` (the
  original returns black for both), `getRoundCatmullRomSplinePoint`,
  `CAABox::insidePlane`;
- `CEulerAngles(const CTransform&)`: x is computed like the quaternion version
  (the original's x follows an irregular curve); Metaforce reads only z from
  it, which matches;
- edge cases with non-unit or opposite inputs (`CQuaternion::slerp` of a
  quaternion and its scaled negation, `CQuaternion(a, -a)`);
- `CAABox::projectedPointTest` with a camera looking along the box's horizon
  (w near zero). With boxes in front of the camera it matched the original in
  2,000 of 2,000 random cases.

Some of the original's quirks are reproduced on purpose because the game
depends on them, for example `CAABox::getTri` (degenerate triangles),
`CPlane::clipLineSegment` (measures from a fixed point) and
`CTransform::multiplyIgnoreTranslation` (adds translations). Comments in the
source mark each one.

Build the probe standalone: `cmake -S components/zeus -B /tmp/zeus-probe &&
cmake --build /tmp/zeus-probe && /tmp/zeus-probe/zeus_probe`.

`scripts/bootstrap-workspace.sh` copies this folder into
`ref/metaforce/extern/zeus` in place of the old submodule.
