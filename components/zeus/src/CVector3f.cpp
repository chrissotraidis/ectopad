// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CVector3f.hpp"

#include "zeus/CQuaternion.hpp"
#include "zeus/CRelAngle.hpp"

namespace zeus {
float CVector3f::getAngleDiff(const CVector3f& a, const CVector3f& b) {
  const float mags = a.magnitude() * b.magnitude();
  if (mags <= FLT_EPSILON)
    return 0.f;
  const float c = std::clamp(a.dot(b) / mags, -1.f, 1.f);
  return std::acos(c);
}

// Rotates a toward b by clampAngle, about the axis perpendicular to both.
CVector3f CVector3f::slerp(const CVector3f& a, const CVector3f& b, CRelAngle clampAngle) {
  const CVector3f axis = a.cross(b).normalized();
  return CQuaternion::fromAxisAngle(CUnitVector3f(axis, false), clampAngle).transform(a);
}

bool CVector3f::canBeNormalized() const {
  if (!isNotInf())
    return false;
  return magSquared() >= FLT_EPSILON * FLT_EPSILON;
}
} // namespace zeus
