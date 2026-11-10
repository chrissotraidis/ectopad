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
class CVector4d {
public:
  zeus::simd<double> mSimd;
  constexpr CVector4d() : mSimd() {}
  template <typename T>
  constexpr CVector4d(const simd<T>& s) : mSimd(s) {}
  constexpr CVector4d(const simd<double>& s) : mSimd(s) {}
  explicit constexpr CVector4d(double xyzw) : mSimd(xyzw) {}
  void assign(double x, double y, double z, double w) { mSimd = simd<double>(x, y, z, w); }
  constexpr CVector4d(double x, double y, double z, double w) : mSimd(x, y, z, w) {}
  constexpr CVector4d(const CColor& other);
  CVector4d(const CVector3f& other, double wIn = double(1)) : mSimd(other.x(), other.y(), other.z(), wIn) {}
  static CVector4d ToClip(const zeus::CVector3f& v, double w) { return CVector4d(v * float(w), w); }
  CVector3f toVec3f() const { return {float(x()), float(y()), float(z())}; }
  constexpr CVector4d& operator=(const CColor& other);

  bool operator==(const CVector4d& rhs) const { return mSimd[0] == rhs.mSimd[0] && mSimd[1] == rhs.mSimd[1] && mSimd[2] == rhs.mSimd[2] && mSimd[3] == rhs.mSimd[3]; }
  bool operator!=(const CVector4d& rhs) const { return !(*this == rhs); }
  bool operator<(const CVector4d& rhs) const { return mSimd[0] < rhs.mSimd[0] && mSimd[1] < rhs.mSimd[1] && mSimd[2] < rhs.mSimd[2] && mSimd[3] < rhs.mSimd[3]; }
  bool operator<=(const CVector4d& rhs) const { return mSimd[0] <= rhs.mSimd[0] && mSimd[1] <= rhs.mSimd[1] && mSimd[2] <= rhs.mSimd[2] && mSimd[3] <= rhs.mSimd[3]; }
  bool operator>(const CVector4d& rhs) const { return mSimd[0] > rhs.mSimd[0] && mSimd[1] > rhs.mSimd[1] && mSimd[2] > rhs.mSimd[2] && mSimd[3] > rhs.mSimd[3]; }
  bool operator>=(const CVector4d& rhs) const { return mSimd[0] >= rhs.mSimd[0] && mSimd[1] >= rhs.mSimd[1] && mSimd[2] >= rhs.mSimd[2] && mSimd[3] >= rhs.mSimd[3]; }
  constexpr CVector4d operator+(const CVector4d& rhs) const { return CVector4d(mSimd + rhs.mSimd); }
  constexpr CVector4d operator-(const CVector4d& rhs) const { return CVector4d(mSimd - rhs.mSimd); }
  constexpr CVector4d operator-() const { return CVector4d(-mSimd); }
  constexpr CVector4d operator*(const CVector4d& rhs) const { return CVector4d(mSimd * rhs.mSimd); }
  constexpr CVector4d operator/(const CVector4d& rhs) const { return CVector4d(mSimd / rhs.mSimd); }
  constexpr CVector4d operator+(double val) const { return CVector4d(mSimd + simd<double>(val)); }
  constexpr CVector4d operator-(double val) const { return CVector4d(mSimd - simd<double>(val)); }
  constexpr CVector4d operator*(double val) const { return CVector4d(mSimd * simd<double>(val)); }
  constexpr CVector4d operator/(double val) const { return CVector4d(mSimd / simd<double>(val)); }
  constexpr const CVector4d& operator+=(const CVector4d& rhs) { mSimd += rhs.mSimd; return *this; }
  constexpr const CVector4d& operator-=(const CVector4d& rhs) { mSimd -= rhs.mSimd; return *this; }
  constexpr const CVector4d& operator*=(const CVector4d& rhs) { mSimd *= rhs.mSimd; return *this; }
  constexpr const CVector4d& operator/=(const CVector4d& rhs) { mSimd /= rhs.mSimd; return *this; }

  void normalize() { *this = normalized(); }
  CVector4d normalized() const { return *this * (double(1) / magnitude()); }
  constexpr double dot(const CVector4d& rhs) const { return mSimd.dot4(rhs.mSimd); }
  constexpr double magSquared() const { return mSimd.dot4(mSimd); }
  double magnitude() const { return std::sqrt(magSquared()); }
  constexpr void zeroOut() { mSimd = simd<double>(); }
  constexpr void splat(double xyzw) { mSimd = simd<double>(xyzw); }
  static CVector4d lerp(const CVector4d& a, const CVector4d& b, double t) { return a + (b - a) * t; }
  static CVector4d nlerp(const CVector4d& a, const CVector4d& b, double t) { return lerp(a, b, t).normalized(); }
  bool isNormalized() const { return std::fabs(double(1) - magSquared()) <= double(0.01); }
  bool canBeNormalized() const { return magSquared() >= double(FLT_EPSILON) * double(FLT_EPSILON); }
  bool isEqu(const CVector4d& other, double epsilon = FLT_EPSILON) const {
    for (int i = 0; i < 4; ++i)
      if (std::fabs(mSimd[i] - other.mSimd[i]) > epsilon)
        return false;
    return true;
  }

  constexpr typename simd<double>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr double operator[](size_t idx) const { return mSimd[idx]; }
  constexpr double x() const { return mSimd[0]; }
  constexpr typename simd<double>::reference x() { return mSimd[0]; }
  constexpr double y() const { return mSimd[1]; }
  constexpr typename simd<double>::reference y() { return mSimd[1]; }
  constexpr double z() const { return mSimd[2]; }
  constexpr typename simd<double>::reference z() { return mSimd[2]; }
  constexpr double w() const { return mSimd[3]; }
  constexpr typename simd<double>::reference w() { return mSimd[3]; }
};
constexpr CVector4d skOne4d(double(1));
constexpr CVector4d skNegOne4d(double(-1));
constexpr CVector4d skZero4d(double(0));
constexpr CVector4d operator+(double lhs, const CVector4d& rhs) { return CVector4d(simd<double>(lhs) + rhs.mSimd); }
constexpr CVector4d operator-(double lhs, const CVector4d& rhs) { return CVector4d(simd<double>(lhs) - rhs.mSimd); }
constexpr CVector4d operator*(double lhs, const CVector4d& rhs) { return CVector4d(simd<double>(lhs) * rhs.mSimd); }
constexpr CVector4d operator/(double lhs, const CVector4d& rhs) { return CVector4d(simd<double>(lhs) / rhs.mSimd); }
} // namespace zeus
