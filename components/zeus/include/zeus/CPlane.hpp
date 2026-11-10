// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cassert>

#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

namespace zeus {
// Plane n.p = d, lanes x, y, z = normal and lane 3 = d.
class CPlane {
public:
  constexpr CPlane() : mSimd(1.f, 0.f, 0.f, 0.f) {}
  constexpr CPlane(float a, float b, float c, float d) : mSimd(a, b, c, d) {}
  CPlane(const CVector3f& a, const CVector3f& b, const CVector3f& c) {
    const CVector3f n = (b - a).cross(c - a).normalized();
    mSimd = simd<float>(n.x(), n.y(), n.z(), n.dot(a));
  }
  CPlane(const CVector3f& point, float displacement) : mSimd(point.x(), point.y(), point.z(), displacement) {}
  float clipLineSegment(const CVector3f& a, const CVector3f& b) const;
  void normalize() {
    const float invMag = 1.f / normal().magnitude();
    mSimd *= simd<float>(invMag);
  }
  float pointToPlaneDist(const CVector3f& pos) const { return pos.dot(normal()) - d(); }
  bool rayPlaneIntersection(const CVector3f& from, const CVector3f& to, CVector3f& point) const;
  CVector3f normal() const { return {x(), y(), z()}; }
  constexpr zeus::simd<float>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr float operator[](size_t idx) const { return mSimd[idx]; }
  constexpr float x() const { return mSimd[0]; }
  constexpr float y() const { return mSimd[1]; }
  constexpr float z() const { return mSimd[2]; }
  constexpr float d() const { return mSimd[3]; }
  constexpr simd<float>::reference x() { return mSimd[0]; }
  constexpr simd<float>::reference y() { return mSimd[1]; }
  constexpr simd<float>::reference z() { return mSimd[2]; }
  constexpr simd<float>::reference d() { return mSimd[3]; }

  zeus::simd<float> mSimd;
};
} // namespace zeus

