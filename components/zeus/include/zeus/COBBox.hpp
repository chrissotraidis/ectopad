// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include "zeus/CAABox.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
// Oriented box: a transform (center and axes) plus half extents.
class COBBox {
public:
  CTransform transform;
  CVector3f extents;

  constexpr COBBox() = default;
  COBBox(const CAABox& aabb) : transform(CTransform::Translate(aabb.center())), extents(aabb.extents()) {}
  constexpr COBBox(const CTransform& xf, const CVector3f& extents) : transform(xf), extents(extents) {}
  CAABox calculateAABox(const CTransform& worldXf = CTransform()) const;
  static COBBox FromAABox(const CAABox& box, const CTransform& xf) {
    return COBBox(xf * CTransform::Translate(box.center()), box.extents());
  }
  bool OBBIntersectsBox(const COBBox& other) const;
  bool AABoxIntersectsBox(const CAABox& other) const { return OBBIntersectsBox(COBBox(other)); }
};
} // namespace zeus
