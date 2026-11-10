// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cmath>

#include "zeus/Math.hpp"

namespace zeus {
// An angle in radians. MakeRelativeAngle/makeRel wrap it into [0, 2*pi).
class CRelAngle {
  float angle = 0.f;

public:
  static float MakeRelativeAngle(float angle) noexcept {
    constexpr float twoPi = 2.f * M_PIF;
    float r = std::fmod(angle, twoPi);
    if (r < 0.f)
      r += twoPi;
    if (r >= twoPi)
      r = 0.f;
    return r;
  }
  constexpr CRelAngle() noexcept = default;
  constexpr CRelAngle(float angle) noexcept : angle(angle) {}
  constexpr CRelAngle(const CRelAngle&) noexcept = default;
  constexpr CRelAngle& operator=(float ang) noexcept {
    angle = ang;
    return *this;
  }
  constexpr CRelAngle& operator=(const CRelAngle& ang) noexcept = default;
  constexpr float asDegrees() const noexcept { return radToDeg(angle); }
  constexpr float asRadians() const noexcept { return angle; }
  float arcCosine() const noexcept { return std::acos(angle); }
  static constexpr CRelAngle FromDegrees(float angle) noexcept { return CRelAngle(degToRad(angle)); }
  constexpr operator float() const noexcept { return angle; }
  static constexpr CRelAngle FromRadians(float angle) noexcept { return CRelAngle(angle); }
  constexpr bool operator<(const CRelAngle& other) const noexcept { return angle < other.angle; }
  constexpr CRelAngle& operator+=(const CRelAngle& other) noexcept { angle += other.angle; return *this; }
  constexpr CRelAngle& operator+=(float r) noexcept { angle += r; return *this; }
  constexpr CRelAngle& operator-=(const CRelAngle& other) noexcept { angle -= other.angle; return *this; }
  constexpr CRelAngle& operator-=(float r) noexcept { angle -= r; return *this; }
  constexpr CRelAngle& operator*=(const CRelAngle& other) noexcept { angle *= other.angle; return *this; }
  constexpr CRelAngle& operator*=(float r) noexcept { angle *= r; return *this; }
  constexpr CRelAngle& operator/=(const CRelAngle& other) noexcept { angle /= other.angle; return *this; }
  constexpr CRelAngle& operator/=(float r) noexcept { angle /= r; return *this; }
  void makeRel() noexcept { angle = MakeRelativeAngle(angle); }
  CRelAngle asRel() const noexcept { return CRelAngle(MakeRelativeAngle(angle)); }
};
} // namespace zeus
