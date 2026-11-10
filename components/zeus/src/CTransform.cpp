// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CTransform.hpp"

#include "zeus/CQuaternion.hpp"

namespace zeus {
void CTransform::rotateLocalX(float theta) { basis = basis * CMatrix3f::RotateX(theta); }
void CTransform::rotateLocalY(float theta) { basis = basis * CMatrix3f::RotateY(theta); }
void CTransform::rotateLocalZ(float theta) { basis = basis * CMatrix3f::RotateZ(theta); }

void CTransform::printMatrix() const {
  for (int r = 0; r < 3; ++r)
    std::printf("%f, %f, %f, %f\n", basis[0][r], basis[1][r], basis[2][r], origin[r]);
  std::printf("%f, %f, %f, %f\n", 0.f, 0.f, 0.f, 1.f);
}

// A basis whose front (Y) axis is uVec.
CTransform CTransform::MakeRotationsBasedOnY(const CUnitVector3f& uVec) {
  const CVector3f front = uVec;
  CVector3f right = front.cross(skUp);
  if (!right.canBeNormalized())
    right = front.cross(skForward);
  right.normalize();
  // Column 2 is front x right, so this basis is mirrored relative to lookAt().
  return CTransform(CMatrix3f(right, front, front.cross(right)));
}

CTransform CTransformFromEditorEuler(const CVector3f& eulerVec) {
  return CTransform(CMatrix3f::RotateZ(eulerVec.z()) * CMatrix3f::RotateY(eulerVec.y()) *
                    CMatrix3f::RotateX(eulerVec.x()));
}
CTransform CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin) {
  CTransform ret = CTransformFromEditorEuler(eulerVec);
  ret.origin = origin;
  return ret;
}
CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle) {
  return CQuaternion::fromAxisAngle(CUnitVector3f(axis), CRelAngle(angle)).toTransform();
}

// A transform at pos whose front (Y) axis points at lookPos, rolled so Z is as close to up as possible.
CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up) {
  CVector3f front = lookPos - pos;
  if (!front.canBeNormalized())
    return CTransform::Translate(pos);
  front.normalize();
  CVector3f right = front.cross(up);
  if (!right.canBeNormalized())
    right = front.cross(std::fabs(front.dot(skForward)) < 0.99f ? skForward : skRight);
  right.normalize();
  const CVector3f upVec = right.cross(front);
  return {CMatrix3f(right, front, upVec), pos};
}
} // namespace zeus
