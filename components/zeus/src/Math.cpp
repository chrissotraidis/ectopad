// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/Math.hpp"

#include "zeus/CVector2f.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
namespace {
CPUInfo g_cpuInfo;
} // namespace

void detectCPU() {}
const CPUInfo& cpuFeatures() { return g_cpuInfo; }
std::pair<bool, const CPUInfo&> validateCPU() { return {true, g_cpuInfo}; }

CVector3f min(const CVector3f& a, const CVector3f& b) {
  return {std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z())};
}
CVector3f max(const CVector3f& a, const CVector3f& b) {
  return {std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z())};
}

CVector3f baryToWorld(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& bary) {
  return p0 * bary.x() + p1 * bary.y() + p2 * bary.z();
}

// Cubic Bezier through control points a..d (Bernstein form).
CVector3f getBezierPoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t) {
  const float u = 1.f - t;
  return a * (u * u * u) + b * (3.f * u * u * t) + c * (3.f * u * t * t) + d * (t * t * t);
}

// Uniform Catmull-Rom segment between b and c.
float getCatmullRomSplinePoint(float a, float b, float c, float d, float t) {
  const float t2 = t * t;
  const float t3 = t2 * t;
  return 0.5f * (2.f * b + (c - a) * t + (2.f * a - 5.f * b + 4.f * c - d) * t2 + (3.f * b - a - 3.f * c + d) * t3);
}
CVector3f getCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d,
                                   float t) {
  const float t2 = t * t;
  const float t3 = t2 * t;
  return (b * 2.f + (c - a) * t + (a * 2.f - b * 5.f + c * 4.f - d) * t2 + (b * 3.f - a - c * 3.f + d) * t3) * 0.5f;
}

// Centripetal Catmull-Rom (knot spacing by the square root of the chord length).
CVector3f getRoundCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c,
                                        const CVector3f& d, float t) {
  auto knot = [](float ti, const CVector3f& p, const CVector3f& q) {
    return ti + std::sqrt((q - p).magnitude());
  };
  const float t0 = 0.f;
  const float t1 = knot(t0, a, b);
  const float t2 = knot(t1, b, c);
  const float t3 = knot(t2, c, d);
  if (t1 == t0 || t2 == t1 || t3 == t2)
    return getCatmullRomSplinePoint(a, b, c, d, t);
  const float s = t1 + (t2 - t1) * t;
  const CVector3f A1 = a * ((t1 - s) / (t1 - t0)) + b * ((s - t0) / (t1 - t0));
  const CVector3f A2 = b * ((t2 - s) / (t2 - t1)) + c * ((s - t1) / (t2 - t1));
  const CVector3f A3 = c * ((t3 - s) / (t3 - t2)) + d * ((s - t2) / (t3 - t2));
  const CVector3f B1 = A1 * ((t2 - s) / (t2 - t0)) + A2 * ((s - t0) / (t2 - t0));
  const CVector3f B2 = A2 * ((t3 - s) / (t3 - t1)) + A3 * ((s - t1) / (t3 - t1));
  return B1 * ((t2 - s) / (t2 - t1)) + B2 * ((s - t1) / (t2 - t1));
}

int floorPowerOfTwo(int x) {
  if (x <= 0)
    return 0;
  return int(std::bit_floor(unsigned(x)));
}
int ceilingPowerOfTwo(int x) {
  if (x <= 0)
    return 0;
  return int(std::bit_ceil(unsigned(x)));
}

bool close_enough(const CVector3f& a, const CVector3f& b, float epsilon) {
  return std::fabs(a.x() - b.x()) <= epsilon && std::fabs(a.y() - b.y()) <= epsilon &&
         std::fabs(a.z() - b.z()) <= epsilon;
}
bool close_enough(const CVector2f& a, const CVector2f& b, float epsilon) {
  return std::fabs(a.x() - b.x()) <= epsilon && std::fabs(a.y() - b.y()) <= epsilon;
}
} // namespace zeus

