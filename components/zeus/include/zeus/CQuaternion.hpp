// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cmath>

#include "zeus/CAxisAngle.hpp"
#include "zeus/CMatrix3f.hpp"
#include "zeus/CRelAngle.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/CVector4f.hpp"
#include "zeus/Global.hpp"

namespace zeus {
class CNUQuaternion;

// Rotation quaternion, lanes ordered w, x, y, z.
class CQuaternion {
public:
  constexpr CQuaternion() : mSimd(1.f, 0.f, 0.f, 0.f) {}
  constexpr CQuaternion(float wi, float xi, float yi, float zi) : mSimd(wi, xi, yi, zi) {}
  // From Euler angles (radians) about X, Y and Z.
  CQuaternion(float xi, float yi, float zi) { fromVector3f(CVector3f(xi, yi, zi)); }
  CQuaternion(float wi, const CVector3f& vec) : mSimd(wi, vec.x(), vec.y(), vec.z()) {}
  template <typename T>
  constexpr CQuaternion(const simd<T>& s) : mSimd(s) {}
  constexpr CQuaternion(const simd<float>& s) : mSimd(s) {}
  CQuaternion(const CMatrix3f& mat);
  CQuaternion(const CVector3f& vec) { fromVector3f(vec); }
  constexpr CQuaternion(const CVector4f& vec) : mSimd(vec.mSimd) {}
  // The rotation taking direction vecA to direction vecB.
  CQuaternion(const CVector3f& vecA, const CVector3f& vecB);
  void fromVector3f(const CVector3f& vec);

  CQuaternion& operator=(const CQuaternion& q) {
    mSimd = q.mSimd;
    return *this;
  }
  CQuaternion operator+(const CQuaternion& q) const { return CQuaternion(mSimd + q.mSimd); }
  CQuaternion operator-(const CQuaternion& q) const { return CQuaternion(mSimd - q.mSimd); }
  CQuaternion operator*(const CQuaternion& q) const;
  CQuaternion operator/(const CQuaternion& q) const { return *this * q.inverse(); }
  CQuaternion operator*(float scale) const { return CQuaternion(mSimd * simd<float>(scale)); }
  CQuaternion operator/(float scale) const { return CQuaternion(mSimd / simd<float>(scale)); }
  CQuaternion operator-() const { return CQuaternion(-mSimd); }
  const CQuaternion& operator+=(const CQuaternion& q) {
    mSimd += q.mSimd;
    return *this;
  }
  const CQuaternion& operator-=(const CQuaternion& q) {
    mSimd -= q.mSimd;
    return *this;
  }
  const CQuaternion& operator*=(const CQuaternion& q) { return *this = *this * q; }
  const CQuaternion& operator*=(float scale) {
    mSimd *= simd<float>(scale);
    return *this;
  }
  const CQuaternion& operator/=(float scale) {
    mSimd /= simd<float>(scale);
    return *this;
  }
  float magnitude() const { return std::sqrt(magSquared()); }
  float magSquared() const { return mSimd.dot4(mSimd); }
  void normalize() { *this = normalized(); }
  CQuaternion normalized() const { return *this / magnitude(); }
  static constexpr simd<float> InvertQuat = {1.f, -1.f, -1.f, -1.f};
  void invert() { mSimd *= InvertQuat; }
  CQuaternion inverse() const { return CQuaternion(mSimd * InvertQuat); }
  static CQuaternion fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle);
  void rotateX(const CRelAngle& angle);
  void rotateY(const CRelAngle& angle);
  void rotateZ(const CRelAngle& angle);
  static CVector3f rotate(const CQuaternion& rotation, const CAxisAngle& v);
  static CQuaternion lookAt(const CUnitVector3f& source, const CUnitVector3f& dest, const CRelAngle& maxAng);
  CVector3f transform(const CVector3f& v) const;
  CQuaternion log() const;
  CQuaternion exp() const;
  CTransform toTransform() const { return CTransform(CMatrix3f(*this)); }
  CTransform toTransform(const zeus::CVector3f& origin) const { return CTransform(CMatrix3f(*this), origin); }
  float dot(const CQuaternion& rhs) const { return mSimd.dot4(rhs.mSimd); }
  static CQuaternion lerp(const CQuaternion& a, const CQuaternion& b, double t);
  static CQuaternion slerp(const CQuaternion& a, const CQuaternion& b, double t);
  static CQuaternion slerpShort(const CQuaternion& a, const CQuaternion& b, double t);
  static CQuaternion nlerp(const CQuaternion& a, const CQuaternion& b, double t);
  static CQuaternion shortestRotationArc(const zeus::CVector3f& v0, const zeus::CVector3f& v1);
  static CQuaternion clampedRotateTo(const zeus::CUnitVector3f& v0, const zeus::CUnitVector3f& v1,
                                     const zeus::CRelAngle& angle);
  float roll() const;
  float pitch() const;
  float yaw() const;
  CQuaternion buildEquivalent() const;
  CVector3f getImaginary() const { return {x(), y(), z()}; }
  void setImaginary(const CVector3f& i) {
    x() = i.x();
    y() = i.y();
    z() = i.z();
  }
  CRelAngle angleFrom(const CQuaternion& other) const;

  constexpr typename simd<float>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr float operator[](size_t idx) const { return mSimd[idx]; }
  constexpr float w() const { return mSimd[0]; }
  constexpr float x() const { return mSimd[1]; }
  constexpr float y() const { return mSimd[2]; }
  constexpr float z() const { return mSimd[3]; }
  constexpr typename simd<float>::reference w() { return mSimd[0]; }
  constexpr typename simd<float>::reference x() { return mSimd[1]; }
  constexpr typename simd<float>::reference y() { return mSimd[2]; }
  constexpr typename simd<float>::reference z() { return mSimd[3]; }

  simd<float> mSimd;

  static CQuaternion fromNUQuaternion(const CNUQuaternion& q);
};

// A quaternion that is not kept at unit length (used to accumulate rotations).
class CNUQuaternion {
public:
  constexpr CNUQuaternion() : mSimd(1.f, 0.f, 0.f, 0.f) {}
  constexpr CNUQuaternion(float wi, float xi, float yi, float zi) : mSimd(wi, xi, yi, zi) {}
  CNUQuaternion(float win, const zeus::CVector3f& vec) : mSimd(win, vec.x(), vec.y(), vec.z()) {}
  CNUQuaternion(const CQuaternion& other) : mSimd(other.mSimd) {}
  CNUQuaternion(const CMatrix3f& mtx) : CNUQuaternion(CQuaternion(mtx)) {}
  constexpr CNUQuaternion(const simd<float>& s) : mSimd(s) {}
  static CNUQuaternion fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle) {
    return CNUQuaternion(CQuaternion::fromAxisAngle(axis, angle));
  }
  float magnitude() const { return std::sqrt(magSquared()); }
  float magSquared() const { return mSimd.dot4(mSimd); }
  void normalize() { *this = normalized(); }
  CNUQuaternion normalized() const { return CNUQuaternion(mSimd / simd<float>(magnitude())); }
  CNUQuaternion operator*(const CNUQuaternion& q) const {
    return CNUQuaternion((CQuaternion(mSimd) * CQuaternion(q.mSimd)).mSimd);
  }
  CNUQuaternion operator*(float f) const { return CNUQuaternion(mSimd * simd<float>(f)); }
  const CNUQuaternion& operator+=(const CNUQuaternion& q) {
    mSimd += q.mSimd;
    return *this;
  }
  constexpr typename simd<float>::reference operator[](size_t idx) { return mSimd[idx]; }
  constexpr float operator[](size_t idx) const { return mSimd[idx]; }
  constexpr float w() const { return mSimd[0]; }
  constexpr float x() const { return mSimd[1]; }
  constexpr float y() const { return mSimd[2]; }
  constexpr float z() const { return mSimd[3]; }
  constexpr typename simd<float>::reference w() { return mSimd[0]; }
  constexpr typename simd<float>::reference x() { return mSimd[1]; }
  constexpr typename simd<float>::reference y() { return mSimd[2]; }
  constexpr typename simd<float>::reference z() { return mSimd[3]; }

  simd<float> mSimd;
};

inline CQuaternion CQuaternion::fromNUQuaternion(const CNUQuaternion& q) {
  return CQuaternion(q.normalized().mSimd);
}

CQuaternion operator+(float lhs, const CQuaternion& rhs);
CQuaternion operator-(float lhs, const CQuaternion& rhs);
CQuaternion operator*(float lhs, const CQuaternion& rhs);
CNUQuaternion operator*(float lhs, const CNUQuaternion& rhs);
} // namespace zeus
