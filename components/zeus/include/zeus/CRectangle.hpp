// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include "zeus/CVector2f.hpp"

namespace zeus {
class CRectangle {
public:
  constexpr CRectangle() = default;
  constexpr CRectangle(float x, float y, float w, float h) : position(x, y), size(w, h) {}
  bool contains(const CVector2f& point) const {
    return point.x() >= position.x() && point.x() <= position.x() + size.x() && point.y() >= position.y() &&
           point.y() <= position.y() + size.y();
  }
  bool intersects(const CRectangle& rect) const {
    return !(position.x() > rect.position.x() + rect.size.x() || rect.position.x() > position.x() + size.x() ||
             position.y() > rect.position.y() + rect.size.y() || rect.position.y() > position.y() + size.y());
  }
  CVector2f position;
  CVector2f size;
};
} // namespace zeus
