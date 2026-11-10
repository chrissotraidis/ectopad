// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cstdint>
#include <cstdio>

#include "zeus/CMatrix3f.hpp"
#include "zeus/CMatrix4f.hpp"
#include "zeus/CUnitVector.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/Global.hpp"

namespace zeus {
// An affine transform: rotation/scale basis plus translation origin.
class CTransform {
public:
  constexpr CTransform() : basis(false), origin() {}
  constexpr CTransform(const CMatrix3f& basis, const CVector3f& offset = {}) : basis(basis), origin(offset) {}
  constexpr CTransform(const CVector3f& c0, const CVector3f& c1, const CVector3f& c2, const CVector3f& c3)
  : basis(c0, c1, c2), origin(c3) {}

  bool operator==(const CTransform& other) const { return origin == other.origin && basis == other.basis; }
  bool operator!=(const CTransform& other) const { return !(*this == other); }
  CTransform operator*(const CTransform& rhs) const { return {basis * rhs.basis, origin + basis * rhs.origin}; }
  CTransform inverse() const {
    const CMatrix3f inv = basis.inverted();
    return {inv, -(inv * origin)};
  }
  CTransform quickInverse() const {
    const CMatrix3f t = basis.transposed();
    return {t, -(t * origin)};
  }
  static CTransform Translate(const CVector3f& position) { return {CMatrix3f(), position}; }
  static CTransform Translate(float x, float y, float z) { return Translate({x, y, z}); }
  CTransform operator+(const CVector3f& other) const { return {basis, origin + other}; }
  CTransform& operator+=(const CVector3f& other) {
    origin += other;
    return *this;
  }
  CTransform operator-(const CVector3f& other) const { return {basis, origin - other}; }
  CTransform& operator-=(const CVector3f& other) {
    origin -= other;
    return *this;
  }
  CVector3f rotate(const CVector3f& vec) const { return basis * vec; }
  static CTransform RotateX(float theta) { return CTransform(CMatrix3f::RotateX(theta)); }
  static CTransform RotateY(float theta) { return CTransform(CMatrix3f::RotateY(theta)); }
  static CTransform RotateZ(float theta) { return CTransform(CMatrix3f::RotateZ(theta)); }
  void rotateLocalX(float theta);
  void rotateLocalY(float theta);
  void rotateLocalZ(float theta);
  CVector3f transposeRotate(const CVector3f& in) const {
    return {basis[0].dot(in), basis[1].dot(in), basis[2].dot(in)};
  }
  void scaleBy(float factor) {
    const CTransform s = Scale(factor);
    *this = *this * s;
  }
  static CTransform Scale(const CVector3f& factor) { return CTransform(CMatrix3f(factor)); }
  static CTransform Scale(float x, float y, float z) { return Scale(CVector3f(x, y, z)); }
  static CTransform Scale(float factor) { return Scale(CVector3f(factor)); }
  // Rotations compose; translations add without being rotated.
  CTransform multiplyIgnoreTranslation(const CTransform& rhs) const { return {basis * rhs.basis, origin + rhs.origin}; }
  CTransform getRotation() const { return CTransform(basis); }
  void setRotation(const CMatrix3f& mat) { basis = mat; }
  void setRotation(const CTransform& xfrm) { basis = xfrm.basis; }
  const CMatrix3f& buildMatrix3f() const { return basis; }
  CVector3f operator*(const CVector3f& other) const { return origin + basis * other; }
  CMatrix4f toMatrix4f() const {
    return {CVector4f(basis[0], 0.f), CVector4f(basis[1], 0.f), CVector4f(basis[2], 0.f), CVector4f(origin, 1.f)};
  }
  // Row-major 3x4 (rows 0..2 of mtx are written).
  void toCStyleMatrix(float mtx[3][4]) const {
    for (int r = 0; r < 3; ++r) {
      mtx[r][0] = basis[0][r];
      mtx[r][1] = basis[1][r];
      mtx[r][2] = basis[2][r];
      mtx[r][3] = origin[r];
    }
  }
  CVector3f upVector() const { return basis[2]; }
  CVector3f frontVector() const { return basis[1]; }
  CVector3f rightVector() const { return basis[0]; }
  void orthonormalize() { basis = basis.orthonormalized(); }
  void printMatrix() const;
  static CTransform MakeRotationsBasedOnY(const CUnitVector3f& uVec);

  CMatrix3f basis;
  CVector3f origin;
};

inline CTransform CTransformFromScaleVector(const CVector3f& scale) { return CTransform(CMatrix3f(scale)); }
CTransform CTransformFromEditorEuler(const CVector3f& eulerVec);
CTransform CTransformFromEditorEulers(const CVector3f& eulerVec, const CVector3f& origin);
CTransform CTransformFromAxisAngle(const CVector3f& axis, float angle);
CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up = skUp);
} // namespace zeus
