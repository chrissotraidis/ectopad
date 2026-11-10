// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CMatrix3f.hpp"

#include "zeus/CQuaternion.hpp"

namespace zeus {
CMatrix3f::CMatrix3f(const CQuaternion& q) {
  const float w = q.w(), x = q.x(), y = q.y(), z = q.z();
  m[0] = CVector3f(1.f - 2.f * (y * y + z * z), 2.f * (x * y + w * z), 2.f * (x * z - w * y));
  m[1] = CVector3f(2.f * (x * y - w * z), 1.f - 2.f * (x * x + z * z), 2.f * (y * z + w * x));
  m[2] = CVector3f(2.f * (x * z + w * y), 2.f * (y * z - w * x), 1.f - 2.f * (x * x + y * y));
}

// Gram-Schmidt on columns 0 and 1; column 2 is their cross product.
CMatrix3f CMatrix3f::orthonormalized() const {
  const CVector3f c0 = m[0].normalized();
  const CVector3f c1 = (m[1] - c0 * c0.dot(m[1])).normalized();
  return {c0, c1, c0.cross(c1)};
}

CMatrix3f CMatrix3f::inverted() const {
  const CVector3f r0 = m[1].cross(m[2]);
  const CVector3f r1 = m[2].cross(m[0]);
  const CVector3f r2 = m[0].cross(m[1]);
  const float invDet = 1.f / m[0].dot(r0);
  // Rows of the inverse are those cross products divided by the determinant.
  const CVector3f s0 = r0 * invDet, s1 = r1 * invDet, s2 = r2 * invDet;
  return {s0.x(), s0.y(), s0.z(), s1.x(), s1.y(), s1.z(), s2.x(), s2.y(), s2.z()};
}

CMatrix3f CMatrix3f::RotateX(float theta) {
  const float s = std::sin(theta), c = std::cos(theta);
  return {CVector3f(1.f, 0.f, 0.f), CVector3f(0.f, c, s), CVector3f(0.f, -s, c)};
}
CMatrix3f CMatrix3f::RotateY(float theta) {
  const float s = std::sin(theta), c = std::cos(theta);
  return {CVector3f(c, 0.f, -s), CVector3f(0.f, 1.f, 0.f), CVector3f(s, 0.f, c)};
}
CMatrix3f CMatrix3f::RotateZ(float theta) {
  const float s = std::sin(theta), c = std::cos(theta);
  return {CVector3f(c, s, 0.f), CVector3f(-s, c, 0.f), CVector3f(0.f, 0.f, 1.f)};
}
} // namespace zeus

