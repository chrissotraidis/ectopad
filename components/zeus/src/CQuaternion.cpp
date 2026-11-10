// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CQuaternion.hpp"

namespace zeus {
// Euler angles about X, Y and Z, applied as Rz * Ry * Rx.
void CQuaternion::fromVector3f(const CVector3f& vec) {
  const float cx = std::cos(vec.x() * 0.5f), sx = std::sin(vec.x() * 0.5f);
  const float cy = std::cos(vec.y() * 0.5f), sy = std::sin(vec.y() * 0.5f);
  const float cz = std::cos(vec.z() * 0.5f), sz = std::sin(vec.z() * 0.5f);
  mSimd = simd<float>(cx * cy * cz + sx * sy * sz, sx * cy * cz - cx * sy * sz, cx * sy * cz + sx * cy * sz,
                      cx * cy * sz - sx * sy * cz);
}

CQuaternion::CQuaternion(const CMatrix3f& mat) {
  // m(r, c) = mat[c][r]
  const float m00 = mat[0][0], m11 = mat[1][1], m22 = mat[2][2];
  const float trace = m00 + m11 + m22;
  float w, x, y, z;
  if (trace > 0.f) {
    const float s = std::sqrt(trace + 1.f) * 2.f;
    w = 0.25f * s;
    x = (mat[1][2] - mat[2][1]) / s;
    y = (mat[2][0] - mat[0][2]) / s;
    z = (mat[0][1] - mat[1][0]) / s;
  } else if (m00 > m11 && m00 > m22) {
    const float s = std::sqrt(1.f + m00 - m11 - m22) * 2.f;
    w = (mat[1][2] - mat[2][1]) / s;
    x = 0.25f * s;
    y = (mat[1][0] + mat[0][1]) / s;
    z = (mat[2][0] + mat[0][2]) / s;
  } else if (m11 > m22) {
    const float s = std::sqrt(1.f + m11 - m00 - m22) * 2.f;
    w = (mat[2][0] - mat[0][2]) / s;
    x = (mat[1][0] + mat[0][1]) / s;
    y = 0.25f * s;
    z = (mat[2][1] + mat[1][2]) / s;
  } else {
    const float s = std::sqrt(1.f + m22 - m00 - m11) * 2.f;
    w = (mat[0][1] - mat[1][0]) / s;
    x = (mat[2][0] + mat[0][2]) / s;
    y = (mat[2][1] + mat[1][2]) / s;
    z = 0.25f * s;
  }
  mSimd = simd<float>(w, x, y, z);
}

// Half-way construction; opposite directions have no defined axis and give NaN.
CQuaternion::CQuaternion(const CVector3f& vecA, const CVector3f& vecB) {
  const CVector3f a = vecA.normalized();
  const CVector3f b = vecB.normalized();
  const float s = std::sqrt((1.f + a.dot(b)) * 2.f);
  *this = CQuaternion(s * 0.5f, a.cross(b) * (1.f / s));
}

CQuaternion CQuaternion::operator*(const CQuaternion& q) const {
  return {w() * q.w() - x() * q.x() - y() * q.y() - z() * q.z(), w() * q.x() + x() * q.w() + y() * q.z() - z() * q.y(),
          w() * q.y() - x() * q.z() + y() * q.w() + z() * q.x(), w() * q.z() + x() * q.y() - y() * q.x() + z() * q.w()};
}

CQuaternion CQuaternion::fromAxisAngle(const CUnitVector3f& axis, const CRelAngle& angle) {
  const float half = float(angle) * 0.5f;
  return CQuaternion(std::cos(half), axis * std::sin(half));
}

void CQuaternion::rotateX(const CRelAngle& angle) { *this *= fromAxisAngle(CUnitVector3f(skRight), angle); }
void CQuaternion::rotateY(const CRelAngle& angle) { *this *= fromAxisAngle(CUnitVector3f(skForward), angle); }
void CQuaternion::rotateZ(const CRelAngle& angle) { *this *= fromAxisAngle(CUnitVector3f(skUp), angle); }

// v is read as Euler angles (the CVector3f constructor), then conjugated by rotation.
CVector3f CQuaternion::rotate(const CQuaternion& rotation, const CAxisAngle& v) {
  const CQuaternion q(static_cast<const CVector3f&>(v));
  return (rotation * q * rotation.inverse()).getImaginary();
}

CVector3f CQuaternion::transform(const CVector3f& v) const {
  const CQuaternion r = *this * CQuaternion(0.f, v) * inverse();
  return r.getImaginary();
}

CQuaternion CQuaternion::log() const {
  const float a = std::acos(std::clamp(w(), -1.f, 1.f));
  const float s = std::sin(a);
  if (std::fabs(s) < FLT_EPSILON)
    return {0.f, x(), y(), z()};
  return CQuaternion(0.f, getImaginary() * (a / s));
}

CQuaternion CQuaternion::exp() const {
  const float a = getImaginary().magnitude();
  const float s = std::sin(a);
  if (std::fabs(s) < FLT_EPSILON)
    return {std::cos(a), x(), y(), z()};
  return CQuaternion(std::cos(a), getImaginary() * (s / a));
}

CQuaternion CQuaternion::lerp(const CQuaternion& a, const CQuaternion& b, double t) {
  return a + (b - a) * float(t);
}
CQuaternion CQuaternion::nlerp(const CQuaternion& a, const CQuaternion& b, double t) { return lerp(a, b, t).normalized(); }

namespace {
CQuaternion slerpRaw(const CQuaternion& a, const CQuaternion& b, double t) {
  if (t <= 0.0)
    return a;
  if (t >= 1.0)
    return b;
  const double c = a.dot(b);
  if (std::fabs(c) >= 1.0 - FLT_EPSILON)
    return a;
  const double theta = std::acos(std::clamp(c, -1.0, 1.0));
  const double s = std::sin(theta);
  return a * float(std::sin((1.0 - t) * theta) / s) + b * float(std::sin(t * theta) / s);
}
} // namespace

// Both take the shorter arc: slerp flips b, slerpShort flips a (giving the negated quaternion).
CQuaternion CQuaternion::slerp(const CQuaternion& a, const CQuaternion& b, double t) {
  return slerpRaw(a, a.dot(b) < 0.f ? -b : b, t);
}
CQuaternion CQuaternion::slerpShort(const CQuaternion& a, const CQuaternion& b, double t) {
  return a.dot(b) < 0.f ? slerpRaw(-a, b, t) : slerpRaw(a, b, t);
}

CQuaternion CQuaternion::shortestRotationArc(const zeus::CVector3f& v0, const zeus::CVector3f& v1) {
  const CVector3f a = v0.normalized();
  const CVector3f b = v1.normalized();
  const float d = a.dot(b);
  if (d <= -1.f + FLT_EPSILON)
    return CQuaternion();
  const float s = std::sqrt((1.f + d) * 2.f);
  return CQuaternion(s * 0.5f, a.cross(b) * (1.f / s));
}

// Turns source toward dest as a yaw about Z followed by a pitch about the horizontal axis
// beside source; each angle is limited to maxAng separately.
CQuaternion CQuaternion::lookAt(const CUnitVector3f& source, const CUnitVector3f& dest, const CRelAngle& maxAng) {
  auto flat = [](const CVector3f& v) { return v.x() * v.x() + v.y() * v.y() > FLT_EPSILON * FLT_EPSILON; };
  auto azimuth = [](const CVector3f& v) { return std::atan2(v.y(), v.x()); };
  auto elevation = [](const CVector3f& v) {
    return std::atan2(v.z(), std::sqrt(v.x() * v.x() + v.y() * v.y()));
  };
  const float limit = float(maxAng);
  // A vertical vector has no heading of its own; it takes the other one's, so the turn is pure pitch.
  const bool srcFlat = flat(source), dstFlat = flat(dest);
  if (!srcFlat && !dstFlat)
    return CQuaternion();
  const float dstAz = dstFlat ? azimuth(dest) : (srcFlat ? azimuth(source) : 0.f);
  const float srcAz = srcFlat ? azimuth(source) : dstAz;
  float yaw = dstAz - srcAz;
  if (yaw > M_PIF)
    yaw -= 2.f * M_PIF;
  else if (yaw < -M_PIF)
    yaw += 2.f * M_PIF;
  const float pitch = elevation(dest) - elevation(source);
  const CVector3f pitchAxis(std::sin(srcAz), -std::cos(srcAz), 0.f);
  return fromAxisAngle(CUnitVector3f(skUp), CRelAngle(std::clamp(yaw, -limit, limit))) *
         fromAxisAngle(CUnitVector3f(pitchAxis, false), CRelAngle(std::clamp(pitch, -limit, limit)));
}

// The shortest rotation from v0 to v1, limited to angle.
CQuaternion CQuaternion::clampedRotateTo(const zeus::CUnitVector3f& v0, const zeus::CUnitVector3f& v1,
                                         const zeus::CRelAngle& angle) {
  if (CVector3f::getAngleDiff(v0, v1) <= float(angle))
    return shortestRotationArc(v0, v1);
  CVector3f axis = v0.cross(v1);
  if (!axis.canBeNormalized())
    axis = v0.cross(std::fabs(v0.dot(skUp)) < 0.99f ? skUp : skRight);
  return fromAxisAngle(CUnitVector3f(axis), angle);
}

// Angles of the Rz(yaw) * Ry(roll) * Rx(pitch) decomposition.
float CQuaternion::pitch() const {
  return std::atan2(2.f * (w() * x() + y() * z()), 1.f - 2.f * (x() * x() + y() * y()));
}
float CQuaternion::roll() const { return std::asin(std::clamp(2.f * (w() * y() - z() * x()), -1.f, 1.f)); }
float CQuaternion::yaw() const {
  return std::atan2(2.f * (w() * z() + x() * y()), 1.f - 2.f * (y() * y() + z() * z()));
}

CQuaternion CQuaternion::buildEquivalent() const { return -*this; }

CRelAngle CQuaternion::angleFrom(const CQuaternion& other) const {
  return CRelAngle(std::acos(std::clamp(dot(other), -1.f, 1.f)));
}

CQuaternion operator+(float lhs, const CQuaternion& rhs) { return CQuaternion(simd<float>(lhs) + rhs.mSimd); }
CQuaternion operator-(float lhs, const CQuaternion& rhs) { return CQuaternion(simd<float>(lhs) - rhs.mSimd); }
CQuaternion operator*(float lhs, const CQuaternion& rhs) { return CQuaternion(simd<float>(lhs) * rhs.mSimd); }
CNUQuaternion operator*(float lhs, const CNUQuaternion& rhs) { return CNUQuaternion(simd<float>(lhs) * rhs.mSimd); }
} // namespace zeus

