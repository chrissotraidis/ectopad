// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cassert>
#include <cfloat>
#include <cmath>

#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

namespace zeus {
class CColor;
class CVector4f {
public:
  zeus::simd<float> mSimd;
  constexpr CVector4f() : mSimd() {}
  template <typename T>
  constexpr CVector4f(const simd<T>& s) : mSimd(s) {}
  constexpr CVector4f(const simd<float>& s) : mSimd(s) {}
  explicit constexpr CVector4f(float xyzw) : mSimd(xyzw) {}
  void assign(float x, float y, float z, float w) { mSimd = simd<float>(x, y, z, w); }
  constexpr CVector4f(float x, float y, float z, float w) : mSimd(x, y, z, w) {}
  constexpr CVector4f(const CColor& other);
  CVector4f(const CVector3f& other, float wIn = float(1)) : mSimd(other.x(), other.y(), other.z(), wIn) {}
  static CVector4f ToClip(const zeus::CVector3f& v, float w) { return CVector4f(v * float(w), w); }
  CVector3f toVec3f() const { return {float(x()), float(y()), float(z())}; }
  constexpr CVector4f& operator=(const CColor& other);

  bool operator==(const CVector4f& rhs) const { return mSimd[0] == rhs.mSimd[0] && mSimd[1] == rhs.mSimd[1] && mSimd[2] == rhs.mSimd[2] && mSimd[3] == rhs.mSimd[3]; }
  bool operator!=(const CVector4f& rhs) const { return !(*this == rhs); }
  bool operator<(const CVector4f& rhs) const { return mSimd[0] < rhs.mSimd[0] && mSimd[1] < rhs.mSimd[1] && mSimd[2] < rhs.mSimd[2] && mSimd[3] < rhs.mSimd[3]; }
  bool operator<=(const CVector4f& rhs) const { return mSimd[0] <= rhs.mSimd[0] && mSimd[1] <= rhs.mSimd[1] && mSimd[2] <= rhs.mSimd[2] && mSimd[3] <= rhs.mSimd[3]; }
  bool operator>(const CVector4f& rhs) const { return mSimd[0] > rhs.mSimd[0] && mSimd[1] > rhs.mSimd[1] && mSimd[2] > rhs.mSimd[2] && mSimd[3] > rhs.mSimd[3]; }
  bool operator>=(const CVector4f& rhs) const { return mSimd[0] >= rhs.mSimd[0] && mSimd[1] >= rhs.mSimd[1] && mSimd[2] >= rhs.mSimd[2] && mSimd[3] >= rhs.mSimd[3]; }
  constexpr CVector4f operator+(const CVector4f& rhs) const { return CVector4f(mSimd + rhs.mSimd); }
  constexpr CVector4f operator-(const CVector4f& rhs) const { return CVector4f(mSimd - rhs.mSimd); }
  constexpr CVector4f operator-() const { return CVector4f(-mSimd); }
  constexpr CVector4f operator*(const CVector4f& rhs) const { return CVector4f(mSimd * rhs.mSimd); }
  constexpr CVector4f operator/(const CVector4f& rhs) const { return CVector4f(mSimd / rhs.mSimd); }
  constexpr CVector4f operator+(float val) const { return CVector4f(mSimd + simd<float>(val)); }
  constexpr CVector4f operator-(float val) const { return CVector4f(mSimd - simd<float>(val)); }
  constexpr CVector4f operator*(float val) const { return CVector4f(mSimd * simd<float>(val)); }
  constexpr CVector4f operator/(float val) const { return CVector4f(mSimd / simd<float>(val)); }
  constexpr const CVector4f& operator+=(const CVector4f& rhs) { mSimd += rhs.mSimd; return *this; }
  constexpr const CVector4f& operator-=(const CVector4f& rhs) { mSimd -= rhs.mSimd; return *this; }
  constexpr const CVector4f& operator*=(const CVector4f& rhs) { mSimd *= rhs.mSimd; return *this; }
  constexpr const CVector4f& operator/=(const CVector4f& rhs) { mSimd /= rhs.mSimd; return *this; }

  void normalize() { *this = normalized(); }
  CVector4f normalized() const { return *this * (float(1) / magnitude()); }
  constexpr float dot(const CVector4f& rhs) const { return mSimd.dot4(rhs.mSimd); }
  constexpr float magSquared() const { return mSimd.dot4(mSimd); }
  float magnitude() const { return std::sqrt(magSquared()); }
  constexpr void zeroOut() { mSimd = simd<float>(); }
  constexpr void splat(float xyzw) { mSimd = simd<float>(xyzw); }
  static CVector4f lerp(const CVector4f& a, const CVector4f& b, float t) { return a + (b - a) * t; }
  static CVector4f nlerp(const CVector4f& a, const CVector4f& b, float t) { return lerp(a, b, t).normalized(); }
  bool isNormalized() const { return std::fabs(float(1) - magSquared()) <= float(0.01); }
  bool canBeNormalized() const { return magSquared() >= float(FLT_EPSILON) * float(FLT_EPSILON); }
  bool isEqu(const CVector4f& other, float epsilon = FLT_EPSILON) const {
    for (int i = 0; i < 4; ++i)
      if (std::fabs(mSimd[i] - other.mSimd[i]) > epsilon)
        return false;
    return true;
  }

  constexpr typename simd<float>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr float operator[](size_t idx) const { return mSimd[idx]; }
  constexpr float x() const { return mSimd[0]; }
  constexpr typename simd<float>::reference x() { return mSimd[0]; }
  constexpr float y() const { return mSimd[1]; }
  constexpr typename simd<float>::reference y() { return mSimd[1]; }
  constexpr float z() const { return mSimd[2]; }
  constexpr typename simd<float>::reference z() { return mSimd[2]; }
  constexpr float w() const { return mSimd[3]; }
  constexpr typename simd<float>::reference w() { return mSimd[3]; }
};
constexpr CVector4f skOne4f(float(1));
constexpr CVector4f skNegOne4f(float(-1));
constexpr CVector4f skZero4f(float(0));
constexpr CVector4f operator+(float lhs, const CVector4f& rhs) { return CVector4f(simd<float>(lhs) + rhs.mSimd); }
constexpr CVector4f operator-(float lhs, const CVector4f& rhs) { return CVector4f(simd<float>(lhs) - rhs.mSimd); }
constexpr CVector4f operator*(float lhs, const CVector4f& rhs) { return CVector4f(simd<float>(lhs) * rhs.mSimd); }
constexpr CVector4f operator/(float lhs, const CVector4f& rhs) { return CVector4f(simd<float>(lhs) / rhs.mSimd); }
} // namespace zeus
