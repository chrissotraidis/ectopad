// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <array>
#include <cassert>

#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

namespace zeus {
class CQuaternion;

// 3x3 matrix stored as three column vectors: m[c][r] is row r of column c.
class CMatrix3f {
public:
  explicit constexpr CMatrix3f(bool zero = false)
  : m{CVector3f(zero ? 0.f : 1.f, 0.f, 0.f), CVector3f(0.f, zero ? 0.f : 1.f, 0.f),
      CVector3f(0.f, 0.f, zero ? 0.f : 1.f)} {}
  // Arguments in row-major reading order.
  constexpr CMatrix3f(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
  : m{CVector3f(m00, m10, m20), CVector3f(m01, m11, m21), CVector3f(m02, m12, m22)} {}
  CMatrix3f(const CVector3f& scaleVec)
  : m{CVector3f(scaleVec.x(), 0.f, 0.f), CVector3f(0.f, scaleVec.y(), 0.f), CVector3f(0.f, 0.f, scaleVec.z())} {}
  CMatrix3f(float scale) : CMatrix3f(CVector3f(scale)) {}
  constexpr CMatrix3f(const CVector3f& c0, const CVector3f& c1, const CVector3f& c2) : m{c0, c1, c2} {}
  constexpr CMatrix3f(const CMatrix3f& other) noexcept = default;
  constexpr CMatrix3f(const simd<float>& c0, const simd<float>& c1, const simd<float>& c2)
  : m{CVector3f(c0), CVector3f(c1), CVector3f(c2)} {}
  CMatrix3f(const CQuaternion& quat);
  constexpr CMatrix3f& operator=(const CMatrix3f& other) noexcept = default;

  CVector3f operator*(const CVector3f& v) const { return m[0] * v.x() + m[1] * v.y() + m[2] * v.z(); }
  constexpr CVector3f& operator[](size_t i) { return m[i]; }
  constexpr const CVector3f& operator[](size_t i) const { return m[i]; }

  CMatrix3f orthonormalized() const;
  bool operator==(const CMatrix3f& other) const { return m[0] == other.m[0] && m[1] == other.m[1] && m[2] == other.m[2]; }
  bool operator!=(const CMatrix3f& other) const { return !(*this == other); }
  void transpose() { *this = transposed(); }
  CMatrix3f transposed() const {
    return {CVector3f(m[0].x(), m[1].x(), m[2].x()), CVector3f(m[0].y(), m[1].y(), m[2].y()),
            CVector3f(m[0].z(), m[1].z(), m[2].z())};
  }
  void invert() { *this = inverted(); }
  CMatrix3f inverted() const;
  void addScaledMatrix(const CMatrix3f& other, float scale) {
    for (int i = 0; i < 3; ++i)
      m[i] += other.m[i] * scale;
  }
  static CMatrix3f RotateX(float theta);
  static CMatrix3f RotateY(float theta);
  static CMatrix3f RotateZ(float theta);
  float determinant() const { return m[0].dot(m[1].cross(m[2])); }

  std::array<CVector3f, 3> m;
};

inline CMatrix3f operator*(const CMatrix3f& lhs, const CMatrix3f& rhs) {
  return {lhs * rhs.m[0], lhs * rhs.m[1], lhs * rhs.m[2]};
}
} // namespace zeus

