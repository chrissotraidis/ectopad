// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include "zeus/CUnitVector.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
// A rotation stored as axis * angle (radians).
struct CAxisAngle : CVector3f {
  constexpr CAxisAngle() = default;
  constexpr CAxisAngle(float x, float y, float z) : CVector3f(x, y, z) {}
  CAxisAngle(const CUnitVector3f& axis, float angle) : CVector3f(axis * angle) {}
  constexpr CAxisAngle(const CVector3f& axisAngle) : CVector3f(axisAngle) {}
  float angle() const { return magnitude(); }
  const CVector3f& getVector() const { return *this; }
};
} // namespace zeus
