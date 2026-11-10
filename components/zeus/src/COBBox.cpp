// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/COBBox.hpp"

namespace zeus {
CAABox COBBox::calculateAABox(const CTransform& worldXf) const {
  const CTransform xf = worldXf * transform;
  CAABox box;
  for (int i = 0; i < 8; ++i) {
    const CVector3f corner((i & 1) ? extents.x() : -extents.x(), (i & 2) ? extents.y() : -extents.y(),
                           (i & 4) ? extents.z() : -extents.z());
    box.accumulateBounds(xf * corner);
  }
  return box;
}

// Separating-axis test over the 15 candidate axes of two oriented boxes.
bool COBBox::OBBIntersectsBox(const COBBox& other) const {
  const CVector3f a[3] = {transform.basis[0], transform.basis[1], transform.basis[2]};
  const CVector3f b[3] = {other.transform.basis[0], other.transform.basis[1], other.transform.basis[2]};
  const CVector3f ea = extents, eb = other.extents;
  const CVector3f t = other.transform.origin - transform.origin;
  auto separated = [&](const CVector3f& axis) {
    if (axis.magSquared() < 1e-12f)
      return false;
    float ra = 0.f, rb = 0.f;
    for (int i = 0; i < 3; ++i) {
      ra += std::fabs(a[i].dot(axis)) * ea[i];
      rb += std::fabs(b[i].dot(axis)) * eb[i];
    }
    return std::fabs(t.dot(axis)) > ra + rb;
  };
  for (int i = 0; i < 3; ++i)
    if (separated(a[i]) || separated(b[i]))
      return false;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (separated(a[i].cross(b[j])))
        return false;
  return true;
}
} // namespace zeus

