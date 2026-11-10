// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <array>

#include "zeus/CAABox.hpp"
#include "zeus/CMatrix4f.hpp"
#include "zeus/CPlane.hpp"
#include "zeus/CProjection.hpp"
#include "zeus/CSphere.hpp"
#include "zeus/CTransform.hpp"

namespace zeus {
class CFrustum {
  std::array<CPlane, 6> planes;
  bool valid = false;

public:
  void updatePlanes(const CMatrix4f& viewMtx, const CMatrix4f& projection);
  void updatePlanes(const CTransform& viewPointMtx, const CProjection& projection);
  bool aabbFrustumTest(const CAABox& aabb) const;
  bool sphereFrustumTest(const CSphere& sphere) const;
  bool pointFrustumTest(const CVector3f& point) const;
};
} // namespace zeus

