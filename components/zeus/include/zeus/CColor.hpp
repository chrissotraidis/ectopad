// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>

#include "zeus/CVector4f.hpp"
#include "zeus/Global.hpp"
#include "zeus/Math.hpp"

#undef min
#undef max

// Packs a 0xRRGGBBAA literal so that its bytes sit in memory as R, G, B, A.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define COLOR(rgba) (zeus::Comp32)(__builtin_bswap32((uint32_t)(rgba)))
#else
#define COLOR(rgba) (zeus::Comp32)(rgba)
#endif

namespace zeus {
using Comp8 = uint8_t;
using Comp32 = uint32_t;
constexpr float OneOver255 = 1.f / 255.f;

union RGBA32 {
  struct {
    Comp8 r, g, b, a;
  };
  Comp32 rgba;
};

class CColor {
public:
  simd<float> mSimd;
  constexpr CColor() : mSimd(1.f) {}
  constexpr CColor(float rgb, float a = 1.0) : mSimd(rgb, rgb, rgb, a) {}
  constexpr CColor(float r, float g, float b, float a = 1.0f) : mSimd(r, g, b, a) {}
  // A packed 0xRRGGBBAA color.
  constexpr CColor(Comp32 rgba)
  : mSimd(float((rgba >> kShiftR) & 0xff) * OneOver255, float((rgba >> kShiftG) & 0xff) * OneOver255,
          float((rgba >> kShiftB) & 0xff) * OneOver255, float((rgba >> kShiftA) & 0xff) * OneOver255) {}
  constexpr CColor(const Comp8* rgba)
  : mSimd(rgba[0] * OneOver255, rgba[1] * OneOver255, rgba[2] * OneOver255, rgba[3] * OneOver255) {}
  constexpr CColor(const CVector4f& other) : mSimd(other.mSimd) {}
  template <typename T>
  constexpr CColor(const simd<T>& s) : mSimd(s) {}
  constexpr CColor(const simd<float>& s) : mSimd(s) {}
  CColor& operator=(const CVector4f& other) {
    mSimd = other.mSimd;
    return *this;
  }

  bool operator==(const CColor& rhs) const {
    return r() == rhs.r() && g() == rhs.g() && b() == rhs.b() && a() == rhs.a();
  }
  bool operator!=(const CColor& rhs) const { return !(*this == rhs); }
  constexpr CColor operator+(const CColor& rhs) const { return CColor(mSimd + rhs.mSimd); }
  constexpr CColor operator-(const CColor& rhs) const { return CColor(mSimd - rhs.mSimd); }
  constexpr CColor operator-() const { return CColor(-mSimd); }
  constexpr CColor operator*(const CColor& rhs) const { return CColor(mSimd * rhs.mSimd); }
  constexpr CColor operator/(const CColor& rhs) const { return CColor(mSimd / rhs.mSimd); }
  constexpr CColor operator+(float val) const { return CColor(mSimd + simd<float>(val)); }
  constexpr CColor operator-(float val) const { return CColor(mSimd - simd<float>(val)); }
  constexpr CColor operator*(float val) const { return CColor(mSimd * simd<float>(val)); }
  constexpr CColor operator/(float val) const { return CColor(mSimd / simd<float>(val)); }
  constexpr const CColor& operator+=(const CColor& rhs) { mSimd += rhs.mSimd; return *this; }
  constexpr const CColor& operator-=(const CColor& rhs) { mSimd -= rhs.mSimd; return *this; }
  constexpr const CColor& operator*=(const CColor& rhs) { mSimd *= rhs.mSimd; return *this; }
  constexpr const CColor& operator/=(const CColor& rhs) { mSimd /= rhs.mSimd; return *this; }

  constexpr const CColor& operator+=(float rhs) { mSimd += simd<float>(rhs); return *this; }
  constexpr const CColor& operator-=(float rhs) { mSimd -= simd<float>(rhs); return *this; }
  constexpr const CColor& operator*=(float rhs) { mSimd *= simd<float>(rhs); return *this; }
  constexpr const CColor& operator/=(float rhs) { mSimd /= simd<float>(rhs); return *this; }

  void normalize() { *this = normalized(); }
  CColor normalized() const { return *this * (1.f / magnitude()); }
  float magSquared() const { return mSimd.dot4(mSimd); }
  float magnitude() const { return std::sqrt(magSquared()); }
  static CColor lerp(const CColor& a, const CColor& b, float t) { return a + (b - a) * t; }
  static CColor nlerp(const CColor& a, const CColor& b, float t) { return lerp(a, b, t).normalized(); }
  constexpr typename simd<float>::reference operator[](const size_t& idx) { return mSimd[idx]; }
  constexpr float operator[](const size_t& idx) const { return mSimd[idx]; }
  void splat(float rgb, float a) { mSimd = simd<float>(rgb, rgb, rgb, a); }
  float rgbDot(const CColor& rhs) const { return mSimd.dot3(rhs.mSimd); }

  void fromRGBA8(const Comp8 ri, const Comp8 gi, const Comp8 bi, const Comp8 ai) {
    mSimd = simd<float>(ri * OneOver255, gi * OneOver255, bi * OneOver255, ai * OneOver255);
  }
  void fromRGBA32(Comp32 rgba) { *this = CColor(rgba); }
  void toRGBA8(Comp8& ro, Comp8& go, Comp8& bo, Comp8& ao) const {
    ro = Comp8(r() * 255.f);
    go = Comp8(g() * 255.f);
    bo = Comp8(b() * 255.f);
    ao = Comp8(a() * 255.f);
  }
  Comp32 toRGBA() const {
    Comp8 c[4];
    toRGBA8(c[0], c[1], c[2], c[3]);
    return Comp32(c[0]) << kShiftR | Comp32(c[1]) << kShiftG | Comp32(c[2]) << kShiftB | Comp32(c[3]) << kShiftA;
  }
  unsigned short toRGB5A3() const;
  void fromHSV(float h, float s, float v, float _a = 1.0);
  void toHSV(float& h, float& s, float& v) const;
  void fromHSL(float h, float s, float l, float _a = 1.0);
  void toHSL(float& h, float& s, float& l) const;
  CColor toGrayscale() const;
  CColor& Clamp() {
    for (int i = 0; i < 4; ++i)
      mSimd[i] = std::clamp(mSimd[i], 0.f, 1.f);
    return *this;
  }

  constexpr float r() const { return mSimd[0]; }
  constexpr float g() const { return mSimd[1]; }
  constexpr float b() const { return mSimd[2]; }
  constexpr float a() const { return mSimd[3]; }
  constexpr typename simd<float>::reference r() { return mSimd[0]; }
  constexpr typename simd<float>::reference g() { return mSimd[1]; }
  constexpr typename simd<float>::reference b() { return mSimd[2]; }
  constexpr typename simd<float>::reference a() { return mSimd[3]; }

private:
  // Packed colors read numerically as 0xRRGGBBAA.
  static constexpr int kShiftR = 24, kShiftG = 16, kShiftB = 8, kShiftA = 0;
};

constexpr CVector4f::CVector4f(const zeus::CColor& other) : mSimd(other.mSimd) {}
constexpr CVector4f& CVector4f::operator=(const CColor& other) {
  mSimd = other.mSimd;
  return *this;
}

constexpr inline CColor skRed(1.f, 0.f, 0.f, 1.f);
constexpr inline CColor skBlack(0.f, 0.f, 0.f, 1.f);
constexpr inline CColor skBlue(0.f, 0.f, 1.f, 1.f);
constexpr inline CColor skCyan(0.f, 1.f, 1.f, 1.f);
constexpr inline CColor skGreen(0.f, 1.f, 0.f, 1.f);
constexpr inline CColor skGrey(0.5f, 0.5f, 0.5f, 1.f);
constexpr inline CColor skMagenta(1.f, 0.f, 1.f, 1.f);
constexpr inline CColor skOrange(1.f, 0.43f, 0.f, 1.f);
constexpr inline CColor skPurple(0.63f, 0.f, 1.f, 1.f);
constexpr inline CColor skYellow(1.f, 1.f, 0.f, 1.f);
constexpr inline CColor skWhite(1.f, 1.f, 1.f, 1.f);
constexpr inline CColor skClear(0.f, 0.f, 0.f, 0.f);

constexpr CColor operator+(float lhs, const CColor& rhs) { return CColor(simd<float>(lhs) + rhs.mSimd); }
constexpr CColor operator-(float lhs, const CColor& rhs) { return CColor(simd<float>(lhs) - rhs.mSimd); }
constexpr CColor operator*(float lhs, const CColor& rhs) { return CColor(simd<float>(lhs) * rhs.mSimd); }
constexpr CColor operator/(float lhs, const CColor& rhs) { return CColor(simd<float>(lhs) / rhs.mSimd); }
} // namespace zeus

namespace std {
template <>
struct hash<zeus::CColor> {
  size_t operator()(const zeus::CColor& color) const noexcept {
    size_t ret = std::hash<float>()(color.r());
    zeus::hash_combine_impl(ret, std::hash<float>()(color.g()));
    zeus::hash_combine_impl(ret, std::hash<float>()(color.b()));
    zeus::hash_combine_impl(ret, std::hash<float>()(color.a()));
    return ret;
  }
};
} // namespace std
