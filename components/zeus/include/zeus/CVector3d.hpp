// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cassert>
#include <cmath>

#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

namespace zeus {
class CVector3d {
public:
  zeus::simd<double> mSimd;
  constexpr CVector3d() : mSimd() {}
  template <typename T>
  constexpr CVector3d(const simd<T>& s) : mSimd(s) {}
  constexpr CVector3d(const simd<double>& s) : mSimd(s) {}
  explicit constexpr CVector3d(double xyz) : mSimd(xyz) {}
  CVector3d(const CVector3f& vec) : mSimd(vec.x(), vec.y(), vec.z(), 0.0) {}
  constexpr CVector3d(double x, double y, double z) : mSimd(x, y, z, 0.0) {}

  CVector3f asCVector3f() const { return {float(x()), float(y()), float(z())}; }
  double magSquared() const { return mSimd.dot3(mSimd); }
  double magnitude() const { return std::sqrt(magSquared()); }
  CVector3d cross(const CVector3d& rhs) const {
    return {y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x()};
  }
  double dot(const CVector3d& rhs) const { return mSimd.dot3(rhs.mSimd); }
  CVector3d asNormalized() const { return CVector3d(mSimd * simd<double>(1.0 / magnitude())); }
  void splat(double xyz) { mSimd = simd<double>(xyz); }
  void zeroOut() { mSimd = simd<double>(); }
  CVector3d operator+(const CVector3d& rhs) const { return CVector3d(mSimd + rhs.mSimd); }
  CVector3d operator-(const CVector3d& rhs) const { return CVector3d(mSimd - rhs.mSimd); }
  CVector3d operator*(const CVector3d& rhs) const { return CVector3d(mSimd * rhs.mSimd); }
  CVector3d operator/(const CVector3d& rhs) const { return CVector3d(mSimd / rhs.mSimd); }

  constexpr typename simd<double>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr double operator[](size_t idx) const { return mSimd[idx]; }
  constexpr double x() const { return mSimd[0]; }
  constexpr typename simd<double>::reference x() { return mSimd[0]; }
  constexpr double y() const { return mSimd[1]; }
  constexpr typename simd<double>::reference y() { return mSimd[1]; }
  constexpr double z() const { return mSimd[2]; }
  constexpr typename simd<double>::reference z() { return mSimd[2]; }
};
inline CVector3f::CVector3f(const CVector3d& vec) : mSimd(float(vec.x()), float(vec.y()), float(vec.z()), 0.f) {}
constexpr inline CVector3d skZero3d(0.0);
constexpr CVector3d operator+(double lhs, const CVector3d& rhs) { return CVector3d(simd<double>(lhs) + rhs.mSimd); }
constexpr CVector3d operator-(double lhs, const CVector3d& rhs) { return CVector3d(simd<double>(lhs) - rhs.mSimd); }
constexpr CVector3d operator*(double lhs, const CVector3d& rhs) { return CVector3d(simd<double>(lhs) * rhs.mSimd); }
constexpr CVector3d operator/(double lhs, const CVector3d& rhs) { return CVector3d(simd<double>(lhs) / rhs.mSimd); }
} // namespace zeus

