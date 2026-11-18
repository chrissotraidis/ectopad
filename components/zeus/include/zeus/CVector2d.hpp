// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cassert>
#include <cfloat>
#include <cmath>

#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

namespace zeus {
class CVector2d {
public:
  simd<double> mSimd;
  constexpr CVector2d() : mSimd() {}
  template <typename T>
  constexpr CVector2d(const simd<T>& s) : mSimd(s) {}
  constexpr CVector2d(const simd<double>& s) : mSimd(s) {}
  explicit constexpr CVector2d(double xy) : mSimd(xy) {}
  constexpr CVector2d(double x, double y) : mSimd(x, y, 0, 0) {}
  constexpr void assign(double x, double y) { mSimd = simd<double>(x, y, 0, 0); }

  bool operator==(const CVector2d& rhs) const { return mSimd[0] == rhs.mSimd[0] && mSimd[1] == rhs.mSimd[1]; }
  bool operator!=(const CVector2d& rhs) const { return !(*this == rhs); }
  bool operator<(const CVector2d& rhs) const { return mSimd[0] < rhs.mSimd[0] && mSimd[1] < rhs.mSimd[1]; }
  bool operator<=(const CVector2d& rhs) const { return mSimd[0] <= rhs.mSimd[0] && mSimd[1] <= rhs.mSimd[1]; }
  bool operator>(const CVector2d& rhs) const { return mSimd[0] > rhs.mSimd[0] && mSimd[1] > rhs.mSimd[1]; }
  bool operator>=(const CVector2d& rhs) const { return mSimd[0] >= rhs.mSimd[0] && mSimd[1] >= rhs.mSimd[1]; }
  constexpr CVector2d operator+(const CVector2d& rhs) const { return CVector2d(mSimd + rhs.mSimd); }
  constexpr CVector2d operator-(const CVector2d& rhs) const { return CVector2d(mSimd - rhs.mSimd); }
  constexpr CVector2d operator-() const { return CVector2d(-mSimd); }
  constexpr CVector2d operator*(const CVector2d& rhs) const { return CVector2d(mSimd * rhs.mSimd); }
  constexpr CVector2d operator/(const CVector2d& rhs) const { return CVector2d(mSimd / rhs.mSimd); }
  constexpr CVector2d operator+(double val) const { return CVector2d(mSimd + simd<double>(val)); }
  constexpr CVector2d operator-(double val) const { return CVector2d(mSimd - simd<double>(val)); }
  constexpr CVector2d operator*(double val) const { return CVector2d(mSimd * simd<double>(val)); }
  constexpr CVector2d operator/(double val) const { return CVector2d(mSimd / simd<double>(val)); }
  constexpr const CVector2d& operator+=(const CVector2d& rhs) { mSimd += rhs.mSimd; return *this; }
  constexpr const CVector2d& operator-=(const CVector2d& rhs) { mSimd -= rhs.mSimd; return *this; }
  constexpr const CVector2d& operator*=(const CVector2d& rhs) { mSimd *= rhs.mSimd; return *this; }
  constexpr const CVector2d& operator/=(const CVector2d& rhs) { mSimd /= rhs.mSimd; return *this; }
  constexpr const CVector2d& operator+=(double rhs) { mSimd += simd<double>(rhs); return *this; }
  constexpr const CVector2d& operator-=(double rhs) { mSimd -= simd<double>(rhs); return *this; }
  constexpr const CVector2d& operator*=(double rhs) { mSimd *= simd<double>(rhs); return *this; }
  constexpr const CVector2d& operator/=(double rhs) { mSimd /= simd<double>(rhs); return *this; }

  void normalize() { *this = normalized(); }
  CVector2d normalized() const { return *this * (double(1) / magnitude()); }
  constexpr CVector2d perpendicularVector() const { return {-y(), x()}; }
  constexpr double cross(const CVector2d& rhs) const { return x() * rhs.y() - y() * rhs.x(); }
  constexpr double dot(const CVector2d& rhs) const { return mSimd.dot2(rhs.mSimd); }
  constexpr double magSquared() const { return mSimd.dot2(mSimd); }
  double magnitude() const { return std::sqrt(magSquared()); }
  constexpr void zeroOut() { mSimd = simd<double>(); }
  constexpr void splat(double xy) { mSimd = simd<double>(xy); }

  static double getAngleDiff(const CVector2d& a, const CVector2d& b);
  static CVector2d lerp(const CVector2d& a, const CVector2d& b, double t) { return a + (b - a) * t; }
  static CVector2d nlerp(const CVector2d& a, const CVector2d& b, double t) { return lerp(a, b, t).normalized(); }
  static CVector2d slerp(const CVector2d& a, const CVector2d& b, double t);

  bool isNormalized() const { return std::fabs(double(1) - magSquared()) <= double(0.01); }
  bool canBeNormalized() const;
  bool isZero() const { return x() == double(0) && y() == double(0); }
  bool isEqu(const CVector2d& other, double epsilon = FLT_EPSILON) const {
    return std::fabs(x() - other.x()) <= epsilon && std::fabs(y() - other.y()) <= epsilon;
  }

  constexpr typename simd<double>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr double operator[](size_t idx) const { return mSimd[idx]; }
  constexpr double x() const { return mSimd[0]; }
  constexpr typename simd<double>::reference x() { return mSimd[0]; }
  constexpr double y() const { return mSimd[1]; }
  constexpr typename simd<double>::reference y() { return mSimd[1]; }
};
constexpr inline CVector2d skOne2d(double(1));
constexpr inline CVector2d skNegOne2d(double(-1));
constexpr inline CVector2d skZero2d(double(0));
constexpr CVector2d operator+(double lhs, const CVector2d& rhs) { return CVector2d(simd<double>(lhs) + rhs.mSimd); }
constexpr CVector2d operator-(double lhs, const CVector2d& rhs) { return CVector2d(simd<double>(lhs) - rhs.mSimd); }
constexpr CVector2d operator*(double lhs, const CVector2d& rhs) { return CVector2d(simd<double>(lhs) * rhs.mSimd); }
constexpr CVector2d operator/(double lhs, const CVector2d& rhs) { return CVector2d(simd<double>(lhs) / rhs.mSimd); }
} // namespace zeus
