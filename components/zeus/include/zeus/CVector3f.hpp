// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cassert>
#include <cfloat>
#include <cmath>

#include "zeus/CVector2f.hpp"
#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

namespace zeus {
class CVector3d;
class CRelAngle;

class CVector3f {
public:
  zeus::simd<float> mSimd;
  constexpr CVector3f() : mSimd() {}
  template <typename T>
  constexpr CVector3f(const simd<T>& s) : mSimd(s) {}
  constexpr CVector3f(const simd<float>& s) : mSimd(s) {}
  inline CVector3f(const CVector3d& vec);
  explicit constexpr CVector3f(float xyz) : mSimd(xyz) {}
  constexpr CVector3f(float x, float y, float z) : mSimd(x, y, z, 0.f) {}
  constexpr CVector3f(const float* floats) : mSimd(floats[0], floats[1], floats[2], 0.f) {}
  CVector3f(const CVector2f& other, float z = 0.f) : mSimd(other.x(), other.y(), z, 0.f) {}
  void assign(float x, float y, float z) { mSimd = simd<float>(x, y, z, 0.f); }

  CVector2f toVec2f() const { return {x(), y()}; }

  bool operator==(const CVector3f& rhs) const { return x() == rhs.x() && y() == rhs.y() && z() == rhs.z(); }
  bool operator!=(const CVector3f& rhs) const { return !(*this == rhs); }
  simd<float>::mask_type operator<(const CVector3f& rhs) const {
    return {{mSimd[0] < rhs.mSimd[0], mSimd[1] < rhs.mSimd[1], mSimd[2] < rhs.mSimd[2], mSimd[3] < rhs.mSimd[3]}};
  }
  simd<float>::mask_type operator<=(const CVector3f& rhs) const {
    return {{mSimd[0] <= rhs.mSimd[0], mSimd[1] <= rhs.mSimd[1], mSimd[2] <= rhs.mSimd[2], mSimd[3] <= rhs.mSimd[3]}};
  }
  simd<float>::mask_type operator>(const CVector3f& rhs) const {
    return {{mSimd[0] > rhs.mSimd[0], mSimd[1] > rhs.mSimd[1], mSimd[2] > rhs.mSimd[2], mSimd[3] > rhs.mSimd[3]}};
  }
  simd<float>::mask_type operator>=(const CVector3f& rhs) const {
    return {{mSimd[0] >= rhs.mSimd[0], mSimd[1] >= rhs.mSimd[1], mSimd[2] >= rhs.mSimd[2], mSimd[3] >= rhs.mSimd[3]}};
  }
  constexpr CVector3f operator+(const CVector3f& rhs) const { return CVector3f(mSimd + rhs.mSimd); }
  constexpr CVector3f operator-(const CVector3f& rhs) const { return CVector3f(mSimd - rhs.mSimd); }
  constexpr CVector3f operator-() const { return CVector3f(-mSimd); }
  constexpr CVector3f operator*(const CVector3f& rhs) const { return CVector3f(mSimd * rhs.mSimd); }
  constexpr CVector3f operator/(const CVector3f& rhs) const { return CVector3f(mSimd / rhs.mSimd); }
  constexpr CVector3f operator+(float val) const { return CVector3f(mSimd + simd<float>(val)); }
  constexpr CVector3f operator-(float val) const { return CVector3f(mSimd - simd<float>(val)); }
  constexpr CVector3f operator*(float val) const { return CVector3f(mSimd * simd<float>(val)); }
  constexpr CVector3f operator/(float val) const { return CVector3f(mSimd / simd<float>(val)); }
  constexpr const CVector3f& operator+=(const CVector3f& rhs) { mSimd += rhs.mSimd; return *this; }
  constexpr const CVector3f& operator-=(const CVector3f& rhs) { mSimd -= rhs.mSimd; return *this; }
  constexpr const CVector3f& operator*=(const CVector3f& rhs) { mSimd *= rhs.mSimd; return *this; }
  constexpr const CVector3f& operator/=(const CVector3f& rhs) { mSimd /= rhs.mSimd; return *this; }

  void normalize() { *this = normalized(); }
  CVector3f normalized() const { return *this * (1.f / magnitude()); }
  constexpr CVector3f cross(const CVector3f& rhs) const {
    return {y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x()};
  }
  constexpr float dot(const CVector3f& rhs) const { return mSimd.dot3(rhs.mSimd); }
  constexpr float magSquared() const { return mSimd.dot3(mSimd); }
  float magnitude() const { return std::sqrt(magSquared()); }
  bool isNotInf() const { return !(std::isinf(x()) || std::isinf(y()) || std::isinf(z())); }
  bool isMagnitudeSafe() const { return isNotInf() && magSquared() >= 9.9999994e-29f; }
  bool isNaN() const { return std::isnan(x()) || std::isnan(y()) || std::isnan(z()); }
  constexpr void zeroOut() { mSimd = simd<float>(); }
  constexpr void splat(float xyz) { mSimd = simd<float>(xyz); }

  static float getAngleDiff(const CVector3f& a, const CVector3f& b);
  static CVector3f lerp(const CVector3f& a, const CVector3f& b, float t) { return a + (b - a) * t; }
  static CVector3f nlerp(const CVector3f& a, const CVector3f& b, float t) { return lerp(a, b, t).normalized(); }
  static CVector3f slerp(const CVector3f& a, const CVector3f& b, CRelAngle clampAngle);

  bool isNormalized() const { return std::fabs(1.f - magSquared()) <= FLT_EPSILON; }
  bool canBeNormalized() const;
  bool isZero() const { return x() == 0.f && y() == 0.f && z() == 0.f; }
  void scaleToLength(float newLength) { *this = scaledToLength(newLength); }
  CVector3f scaledToLength(float newLength) const { return normalized() * newLength; }
  bool isEqu(const CVector3f& other, float epsilon = FLT_EPSILON) const {
    return std::fabs(x() - other.x()) <= epsilon && std::fabs(y() - other.y()) <= epsilon &&
           std::fabs(z() - other.z()) <= epsilon;
  }

  constexpr typename simd<float>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr float operator[](size_t idx) const { return mSimd[idx]; }
  constexpr float x() const { return mSimd[0]; }
  constexpr typename simd<float>::reference x() { return mSimd[0]; }
  constexpr float y() const { return mSimd[1]; }
  constexpr typename simd<float>::reference y() { return mSimd[1]; }
  constexpr float z() const { return mSimd[2]; }
  constexpr typename simd<float>::reference z() { return mSimd[2]; }

  static inline CVector3f radToDeg(const CVector3f& rad) { return rad * (180.f / M_PIF); }
  static inline CVector3f degToRad(const CVector3f& deg) { return deg * (M_PIF / 180.f); }
};

constexpr inline CVector3f skOne3f(1.f);
constexpr inline CVector3f skNegOne3f(-1.f);
constexpr inline CVector3f skZero3f(0.f);
constexpr inline CVector3f skForward(0.f, 1.f, 0.f);
constexpr inline CVector3f skBack(0.f, -1.f, 0.f);
constexpr inline CVector3f skLeft(-1.f, 0.f, 0.f);
constexpr inline CVector3f skRight(1.f, 0.f, 0.f);
constexpr inline CVector3f skUp(0.f, 0.f, 1.f);
constexpr inline CVector3f skDown(0.f, 0.f, -1.f);
constexpr inline CVector3f skRadToDegVec(180.f / M_PIF);
constexpr inline CVector3f skDegToRadVec(M_PIF / 180.f);

constexpr CVector3f operator+(float lhs, const CVector3f& rhs) { return CVector3f(simd<float>(lhs) + rhs.mSimd); }
constexpr CVector3f operator-(float lhs, const CVector3f& rhs) { return CVector3f(simd<float>(lhs) - rhs.mSimd); }
constexpr CVector3f operator*(float lhs, const CVector3f& rhs) { return CVector3f(simd<float>(lhs) * rhs.mSimd); }
constexpr CVector3f operator/(float lhs, const CVector3f& rhs) { return CVector3f(simd<float>(lhs) / rhs.mSimd); }

inline CVector3f radToDeg(const CVector3f& rad) { return CVector3f::radToDeg(rad); }
inline CVector3f degToRad(const CVector3f& deg) { return CVector3f::degToRad(deg); }
} // namespace zeus
