// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cstdint>

#include "zeus/CVector2f.hpp"

namespace zeus {
class CVector2i {
public:
  int32_t x = 0;
  int32_t y = 0;
  constexpr CVector2i() noexcept = default;
  constexpr CVector2i(int32_t xin, int32_t yin) noexcept : x(xin), y(yin) {}
  CVector2i(const CVector2f& vec) noexcept : x(int32_t(vec.x())), y(int32_t(vec.y())) {}
  constexpr CVector2f toVec2f() const noexcept { return {float(x), float(y)}; }
  constexpr CVector2i operator+(const CVector2i& val) const noexcept { return {x + val.x, y + val.y}; }
  constexpr CVector2i operator-(const CVector2i& val) const noexcept { return {x - val.x, y - val.y}; }
  constexpr CVector2i operator*(const CVector2i& val) const noexcept { return {x * val.x, y * val.y}; }
  constexpr CVector2i operator/(const CVector2i& val) const noexcept { return {x / val.x, y / val.y}; }
  constexpr bool operator==(const CVector2i& other) const noexcept { return x == other.x && y == other.y; }
  constexpr bool operator!=(const CVector2i& other) const noexcept { return !(*this == other); }
  constexpr CVector2i operator*(int32_t val) const noexcept { return {x * val, y * val}; }
};
} // namespace zeus
