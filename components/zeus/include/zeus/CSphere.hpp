// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include "zeus/CVector3f.hpp"

namespace zeus {
class CSphere {
public:
  constexpr CSphere(const CVector3f& position, float radius) : position(position), radius(radius) {}
  CVector3f getSurfaceNormal(const CVector3f& coord) const { return (coord - position).normalized(); }
  bool intersects(const CSphere& other) const {
    const float r = radius + other.radius;
    return (position - other.position).magSquared() <= r * r;
  }
  CVector3f position;
  float radius;
};
} // namespace zeus

