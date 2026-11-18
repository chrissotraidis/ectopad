// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <array>
#include <cassert>

#include "zeus/CMatrix3f.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CVector4f.hpp"

namespace zeus {
// 4x4 matrix stored as four column vectors.
class CMatrix4f {
public:
  explicit constexpr CMatrix4f(bool zero = false)
  : m{CVector4f(zero ? 0.f : 1.f, 0.f, 0.f, 0.f), CVector4f(0.f, zero ? 0.f : 1.f, 0.f, 0.f),
      CVector4f(0.f, 0.f, zero ? 0.f : 1.f, 0.f), CVector4f(0.f, 0.f, 0.f, zero ? 0.f : 1.f)} {}
  // Arguments in row-major reading order.
  constexpr CMatrix4f(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20,
                      float m21, float m22, float m23, float m30, float m31, float m32, float m33)
  : m{CVector4f(m00, m10, m20, m30), CVector4f(m01, m11, m21, m31), CVector4f(m02, m12, m22, m32),
      CVector4f(m03, m13, m23, m33)} {}
  CMatrix4f(const CVector3f& scaleVec)
  : m{CVector4f(scaleVec.x(), 0.f, 0.f, 0.f), CVector4f(0.f, scaleVec.y(), 0.f, 0.f),
      CVector4f(0.f, 0.f, scaleVec.z(), 0.f), CVector4f(0.f, 0.f, 0.f, 1.f)} {}
  constexpr CMatrix4f(const CVector4f& c0, const CVector4f& c1, const CVector4f& c2, const CVector4f& c3)
  : m{c0, c1, c2, c3} {}
  constexpr CMatrix4f(const CMatrix4f& other) noexcept = default;
  constexpr CMatrix4f(const simd<float>& c0, const simd<float>& c1, const simd<float>& c2, const simd<float>& c3)
  : m{CVector4f(c0), CVector4f(c1), CVector4f(c2), CVector4f(c3)} {}
  constexpr CMatrix4f(const CMatrix3f& other)
  : m{CVector4f(other.m[0].mSimd[0], other.m[0].mSimd[1], other.m[0].mSimd[2], 0.f),
      CVector4f(other.m[1].mSimd[0], other.m[1].mSimd[1], other.m[1].mSimd[2], 0.f),
      CVector4f(other.m[2].mSimd[0], other.m[2].mSimd[1], other.m[2].mSimd[2], 0.f), CVector4f(0.f, 0.f, 0.f, 1.f)} {}
  constexpr CMatrix4f& operator=(const CMatrix4f& other) noexcept = default;

  bool operator==(const CMatrix4f& rhs) const {
    return m[0] == rhs.m[0] && m[1] == rhs.m[1] && m[2] == rhs.m[2] && m[3] == rhs.m[3];
  }
  bool operator!=(const CMatrix4f& rhs) const { return !(*this == rhs); }
  CVector4f operator*(const CVector4f& v) const {
    return m[0] * v.x() + m[1] * v.y() + m[2] * v.z() + m[3] * v.w();
  }
  constexpr CVector4f& operator[](size_t i) { return m[i]; }
  constexpr const CVector4f& operator[](size_t i) const { return m[i]; }
  CMatrix4f transposed() const {
    CMatrix4f t;
    for (int c = 0; c < 4; ++c)
      for (int r = 0; r < 4; ++r)
        t.m[c][r] = m[r][c];
    return t;
  }
  CVector3f multiplyOneOverW(const CVector3f& point) const {
    float w;
    return multiplyOneOverW(point, w);
  }
  CVector3f multiplyOneOverW(const CVector3f& point, float& wOut) const {
    const CVector4f r = *this * CVector4f(point, 1.f);
    wOut = r.w();
    return r.toVec3f() / r.w();
  }

  std::array<CVector4f, 4> m;
};
extern const CMatrix4f skIdentityMatrix4f;

inline CMatrix4f operator*(const CMatrix4f& lhs, const CMatrix4f& rhs) {
  return {lhs * rhs.m[0], lhs * rhs.m[1], lhs * rhs.m[2], lhs * rhs.m[3]};
}
} // namespace zeus
