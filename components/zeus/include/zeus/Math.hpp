// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <algorithm>
#include <bit>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <utility>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif
#ifndef M_PI_4
#define M_PI_4 0.78539816339744830962
#endif
#ifndef M_1_PI
#define M_1_PI 0.31830988618379067154
#endif
#ifndef M_2_PI
#define M_2_PI 0.63661977236758134308
#endif
#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257390
#endif
#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440
#endif
#define M_PIF 3.14159265358979323846f
#define M_SQRT1_2F 0.70710678118654752440f

namespace zeus {
class CVector2f;
class CVector3f;
class CTransform;

// Metaforce prints these at startup; on Apple silicon there is nothing to check.
struct CPUInfo {
  const char cpuBrand[48] = {};
  const char cpuVendor[32] = {};
  bool isIntel = false;
  bool SSE1 = false;
  bool SSE2 = false;
  bool SSE3 = false;
  bool SSSE3 = false;
  bool SSE41 = false;
  bool SSE42 = false;
  bool SSE4a = false;
  bool AESNI = false;
  bool AVX = false;
  bool AVX2 = false;
};
void detectCPU();
const CPUInfo& cpuFeatures();
std::pair<bool, const CPUInfo&> validateCPU();

template <typename T>
constexpr T min(const T& a, const T& b) { return b < a ? b : a; }
template <typename T>
constexpr T max(const T& a, const T& b) { return a < b ? b : a; }
CVector3f min(const CVector3f& a, const CVector3f& b);
CVector3f max(const CVector3f& a, const CVector3f& b);
template <typename T>
constexpr T clamp(const T& lo, const T& val, const T& hi) { return max(lo, min(hi, val)); }

constexpr float radToDeg(float rad) { return rad * (180.f / M_PIF); }
constexpr float degToRad(float deg) { return deg * (M_PIF / 180.f); }
constexpr double radToDeg(double rad) { return rad * (180.0 / M_PI); }
constexpr double degToRad(double deg) { return deg * (M_PI / 180.0); }

CVector3f baryToWorld(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& bary);
CVector3f getBezierPoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t);
float getCatmullRomSplinePoint(float a, float b, float c, float d, float t);
CVector3f getCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d,
                                   float t);
CVector3f getRoundCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c,
                                        const CVector3f& d, float t);

// Rounds half away from zero, then one step further (matches the original's results).
inline double round(double val) { return val < 0.0 ? std::ceil(val - 0.5) : std::ceil(val + 0.5); }
inline double powD(float a, float b) { return std::pow(double(a), double(b)); }
inline double invSqrtD(double val) { return 1.0 / std::sqrt(val); }
inline float invSqrtF(float val) { return 1.f / std::sqrt(val); }

int floorPowerOfTwo(int x);
int ceilingPowerOfTwo(int x);

template <typename U>
typename std::enable_if<!std::is_enum<U>::value && std::is_integral<U>::value, int>::type PopCount(U x) {
  return std::popcount(static_cast<std::make_unsigned_t<U>>(x));
}
template <typename E>
typename std::enable_if<std::is_enum<E>::value, int>::type PopCount(E e) {
  return PopCount(static_cast<std::underlying_type_t<E>>(e));
}
template <typename U>
typename std::enable_if<!std::is_enum<U>::value && std::is_integral<U>::value, int>::type countLeadingZeros(U x) {
  return std::countl_zero(static_cast<std::make_unsigned_t<U>>(x));
}
template <typename E>
typename std::enable_if<std::is_enum<E>::value, int>::type countLeadingZeros(E e) {
  return countLeadingZeros(static_cast<std::underlying_type_t<E>>(e));
}

bool close_enough(const CVector3f& a, const CVector3f& b, float epsilon = FLT_EPSILON);
bool close_enough(const CVector2f& a, const CVector2f& b, float epsilon = FLT_EPSILON);
inline bool close_enough(float a, float b, double epsilon = FLT_EPSILON) { return std::fabs(a - b) <= epsilon; }
inline bool close_enough(double a, double b, double epsilon = FLT_EPSILON) { return std::fabs(a - b) <= epsilon; }
} // namespace zeus
