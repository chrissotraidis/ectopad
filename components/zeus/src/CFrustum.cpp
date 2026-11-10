// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CFrustum.hpp"

namespace zeus {
// Planes from the combined clip matrix (Gribb/Hartmann); normals point inward.
void CFrustum::updatePlanes(const CMatrix4f& viewMtx, const CMatrix4f& projection) {
  const CMatrix4f mvp = projection * viewMtx;
  auto row = [&](int r) { return CVector4f(mvp[0][r], mvp[1][r], mvp[2][r], mvp[3][r]); };
  const CVector4f r0 = row(0), r1 = row(1), r2 = row(2), r3 = row(3);
  const CVector4f p[6] = {r3 + r0, r3 - r0, r3 + r1, r3 - r1, r3 + r2, r3 - r2};
  for (int i = 0; i < 6; ++i) {
    CPlane plane(p[i].x(), p[i].y(), p[i].z(), -p[i].w());
    plane.normalize();
    planes[i] = plane;
  }
  valid = true;
}

// viewPointMtx is the camera's world transform; zeus cameras look along +Y with +Z up,
// so camera space is turned into the projection's (+X right, +Y up, looking down -Z).
void CFrustum::updatePlanes(const CTransform& viewPointMtx, const CProjection& projection) {
  const CMatrix4f zeusToView(CVector4f(1.f, 0.f, 0.f, 0.f), CVector4f(0.f, 0.f, -1.f, 0.f), CVector4f(0.f, 1.f, 0.f, 0.f),
                             CVector4f(0.f, 0.f, 0.f, 1.f));
  updatePlanes(zeusToView * viewPointMtx.inverse().toMatrix4f(), projection.getCachedMatrix());
}

bool CFrustum::aabbFrustumTest(const CAABox& aabb) const {
  if (!valid)
    return true;
  for (const CPlane& plane : planes)
    if (plane.pointToPlaneDist(aabb.furthestPointAlongVector(plane.normal())) < 0.f)
      return false;
  return true;
}

bool CFrustum::sphereFrustumTest(const CSphere& sphere) const {
  if (!valid)
    return true;
  for (const CPlane& plane : planes)
    if (plane.pointToPlaneDist(sphere.position) < -sphere.radius)
      return false;
  return true;
}

bool CFrustum::pointFrustumTest(const CVector3f& point) const {
  if (!valid)
    return true;
  for (const CPlane& plane : planes)
    if (plane.pointToPlaneDist(point) < 0.f)
      return false;
  return true;
}
} // namespace zeus
