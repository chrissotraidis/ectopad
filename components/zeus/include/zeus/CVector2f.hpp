// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cassert>
#include <cfloat>
#include <cmath>

#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

namespace zeus {
class CVector2f {
public:
  simd<float> mSimd;
  constexpr CVector2f() : mSimd() {}
  template <typename T>
  constexpr CVector2f(const simd<T>& s) : mSimd(s) {}
  constexpr CVector2f(const simd<float>& s) : mSimd(s) {}
  explicit constexpr CVector2f(float xy) : mSimd(xy) {}
  constexpr CVector2f(float x, float y) : mSimd(x, y, 0, 0) {}
  constexpr void assign(float x, float y) { mSimd = simd<float>(x, y, 0, 0); }

  bool operator==(const CVector2f& rhs) const { return mSimd[0] == rhs.mSimd[0] && mSimd[1] == rhs.mSimd[1]; }
  bool operator!=(const CVector2f& rhs) const { return !(*this == rhs); }
  bool operator<(const CVector2f& rhs) const { return mSimd[0] < rhs.mSimd[0] && mSimd[1] < rhs.mSimd[1]; }
  bool operator<=(const CVector2f& rhs) const { return mSimd[0] <= rhs.mSimd[0] && mSimd[1] <= rhs.mSimd[1]; }
  bool operator>(const CVector2f& rhs) const { return mSimd[0] > rhs.mSimd[0] && mSimd[1] > rhs.mSimd[1]; }
  bool operator>=(const CVector2f& rhs) const { return mSimd[0] >= rhs.mSimd[0] && mSimd[1] >= rhs.mSimd[1]; }
  constexpr CVector2f operator+(const CVector2f& rhs) const { return CVector2f(mSimd + rhs.mSimd); }
  constexpr CVector2f operator-(const CVector2f& rhs) const { return CVector2f(mSimd - rhs.mSimd); }
  constexpr CVector2f operator-() const { return CVector2f(-mSimd); }
  constexpr CVector2f operator*(const CVector2f& rhs) const { return CVector2f(mSimd * rhs.mSimd); }
  constexpr CVector2f operator/(const CVector2f& rhs) const { return CVector2f(mSimd / rhs.mSimd); }
  constexpr CVector2f operator+(float val) const { return CVector2f(mSimd + simd<float>(val)); }
  constexpr CVector2f operator-(float val) const { return CVector2f(mSimd - simd<float>(val)); }
  constexpr CVector2f operator*(float val) const { return CVector2f(mSimd * simd<float>(val)); }
  constexpr CVector2f operator/(float val) const { return CVector2f(mSimd / simd<float>(val)); }
  constexpr const CVector2f& operator+=(const CVector2f& rhs) { mSimd += rhs.mSimd; return *this; }
  constexpr const CVector2f& operator-=(const CVector2f& rhs) { mSimd -= rhs.mSimd; return *this; }
  constexpr const CVector2f& operator*=(const CVector2f& rhs) { mSimd *= rhs.mSimd; return *this; }
  constexpr const CVector2f& operator/=(const CVector2f& rhs) { mSimd /= rhs.mSimd; return *this; }
  constexpr const CVector2f& operator+=(float rhs) { mSimd += simd<float>(rhs); return *this; }
  constexpr const CVector2f& operator-=(float rhs) { mSimd -= simd<float>(rhs); return *this; }
  constexpr const CVector2f& operator*=(float rhs) { mSimd *= simd<float>(rhs); return *this; }
  constexpr const CVector2f& operator/=(float rhs) { mSimd /= simd<float>(rhs); return *this; }

  void normalize() { *this = normalized(); }
  CVector2f normalized() const { return *this * (float(1) / magnitude()); }
  constexpr CVector2f perpendicularVector() const { return {-y(), x()}; }
  constexpr float cross(const CVector2f& rhs) const { return x() * rhs.y() - y() * rhs.x(); }
  constexpr float dot(const CVector2f& rhs) const { return mSimd.dot2(rhs.mSimd); }
  constexpr float magSquared() const { return mSimd.dot2(mSimd); }
  float magnitude() const { return std::sqrt(magSquared()); }
  constexpr void zeroOut() { mSimd = simd<float>(); }
  constexpr void splat(float xy) { mSimd = simd<float>(xy); }

  static float getAngleDiff(const CVector2f& a, const CVector2f& b);
  static CVector2f lerp(const CVector2f& a, const CVector2f& b, float t) { return a + (b - a) * t; }
  static CVector2f nlerp(const CVector2f& a, const CVector2f& b, float t) { return lerp(a, b, t).normalized(); }
  static CVector2f slerp(const CVector2f& a, const CVector2f& b, float t);

  bool isNormalized() const { return std::fabs(float(1) - magSquared()) <= float(0.01); }
  bool canBeNormalized() const;
  bool isZero() const { return x() == float(0) && y() == float(0); }
  bool isEqu(const CVector2f& other, float epsilon = FLT_EPSILON) const {
    return std::fabs(x() - other.x()) <= epsilon && std::fabs(y() - other.y()) <= epsilon;
  }

  constexpr typename simd<float>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr float operator[](size_t idx) const { return mSimd[idx]; }
  constexpr float x() const { return mSimd[0]; }
  constexpr typename simd<float>::reference x() { return mSimd[0]; }
  constexpr float y() const { return mSimd[1]; }
  constexpr typename simd<float>::reference y() { return mSimd[1]; }
};
constexpr inline CVector2f skOne2f(float(1));
constexpr inline CVector2f skNegOne2f(float(-1));
constexpr inline CVector2f skZero2f(float(0));
constexpr CVector2f operator+(float lhs, const CVector2f& rhs) { return CVector2f(simd<float>(lhs) + rhs.mSimd); }
constexpr CVector2f operator-(float lhs, const CVector2f& rhs) { return CVector2f(simd<float>(lhs) - rhs.mSimd); }
constexpr CVector2f operator*(float lhs, const CVector2f& rhs) { return CVector2f(simd<float>(lhs) * rhs.mSimd); }
constexpr CVector2f operator/(float lhs, const CVector2f& rhs) { return CVector2f(simd<float>(lhs) / rhs.mSimd); }
} // namespace zeus

