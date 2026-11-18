// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CPlane.hpp"

namespace zeus {
// Matches the original's results: (d - n.y) / n.(b - a), limited to [0, 1] (NaN passes
// through). The numerator measures from the fixed point (0, 1, 0) instead of a.
float CPlane::clipLineSegment(const CVector3f& a, const CVector3f& b) const {
  const float t = (normal().dot(skForward) - d()) / normal().dot(a - b);
  if (t < 0.f)
    return 0.f;
  if (t > 1.f)
    return 1.f;
  return t;
}

bool CPlane::rayPlaneIntersection(const CVector3f& from, const CVector3f& to, CVector3f& point) const {
  const CVector3f delta = to - from;
  const float denom = normal().dot(delta);
  if (std::fabs(denom) < FLT_EPSILON)
    return false;
  const float t = (d() - normal().dot(from)) / denom;
  if (t < 0.f || t > 1.f)
    return false;
  point = from + delta * t;
  return true;
}
} // namespace zeus
