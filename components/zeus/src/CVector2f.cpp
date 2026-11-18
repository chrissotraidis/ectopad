// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CVector2d.hpp"
#include "zeus/CVector2f.hpp"

namespace zeus {
namespace {
template <typename V, typename T>
T angleBetween(const V& a, const V& b) {
  const T mags = a.magnitude() * b.magnitude();
  if (mags <= T(FLT_EPSILON))
    return T(0);
  T c = a.dot(b) / mags;
  if (c > T(1))
    c = T(1);
  if (c < T(-1))
    c = T(-1);
  return std::acos(c);
}

// Spherical interpolation from a to b; magnitudes come along in the weights.
// Directions more than 90 degrees apart interpolate toward -b (the short way round).
template <typename V, typename T>
V slerpVec(const V& a, const V& bIn, T t) {
  const V b = a.dot(bIn) < T(0) ? -bIn : bIn;
  if (t <= T(0))
    return a;
  if (t >= T(1))
    return b;
  const T c = a.normalized().dot(b.normalized());
  if (c >= T(1) - T(FLT_EPSILON))
    return a;
  const T theta = std::acos(c < T(-1) ? T(-1) : c);
  const T s = std::sin(theta);
  return a * (std::sin((T(1) - t) * theta) / s) + b * (std::sin(t * theta) / s);
}
} // namespace

float CVector2f::getAngleDiff(const CVector2f& a, const CVector2f& b) { return angleBetween<CVector2f, float>(a, b); }
CVector2f CVector2f::slerp(const CVector2f& a, const CVector2f& b, float t) { return slerpVec(a, b, t); }
bool CVector2f::canBeNormalized() const {
  if (std::isinf(x()) || std::isinf(y()))
    return false;
  return magSquared() >= FLT_EPSILON * FLT_EPSILON;
}

double CVector2d::getAngleDiff(const CVector2d& a, const CVector2d& b) { return angleBetween<CVector2d, double>(a, b); }
CVector2d CVector2d::slerp(const CVector2d& a, const CVector2d& b, double t) { return slerpVec(a, b, t); }
bool CVector2d::canBeNormalized() const {
  if (std::isinf(x()) || std::isinf(y()))
    return false;
  return magSquared() >= DBL_EPSILON * DBL_EPSILON;
}
} // namespace zeus
