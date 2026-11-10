// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CColor.hpp"

namespace zeus {
// GX RGB5A3: opaque colors use 1:5:5:5, translucent ones 0:3:4:4:4.
unsigned short CColor::toRGB5A3() const {
  Comp8 r8, g8, b8, a8;
  toRGBA8(r8, g8, b8, a8);
  if (a8 == 255)
    return static_cast<unsigned short>(0x8000 | (r8 >> 3) << 10 | (g8 >> 3) << 5 | (b8 >> 3));
  return static_cast<unsigned short>((a8 >> 5) << 12 | (r8 >> 4) << 8 | (g8 >> 4) << 4 | (b8 >> 4));
}

// Hue, saturation and value/lightness are all in [0, 1].
void CColor::fromHSV(float h, float s, float v, float _a) {
  h = h - std::floor(h);
  const float sector = h * 6.f;
  const int i = int(sector) % 6;
  const float f = sector - std::floor(sector);
  const float p = v * (1.f - s);
  const float q = v * (1.f - s * f);
  const float t = v * (1.f - s * (1.f - f));
  float r = v, g = t, b = p;
  switch (i) {
  case 1: r = q; g = v; b = p; break;
  case 2: r = p; g = v; b = t; break;
  case 3: r = p; g = q; b = v; break;
  case 4: r = t; g = p; b = v; break;
  case 5: r = v; g = p; b = q; break;
  default: break;
  }
  mSimd = simd<float>(r, g, b, _a);
}

namespace {
float hueOf(float r, float g, float b, float mx, float d) {
  if (d == 0.f)
    return 0.f;
  float h;
  if (mx == r)
    h = (g - b) / d + (g < b ? 6.f : 0.f);
  else if (mx == g)
    h = (b - r) / d + 2.f;
  else
    h = (r - g) / d + 4.f;
  return h / 6.f;
}
} // namespace

void CColor::toHSV(float& h, float& s, float& v) const {
  const float mx = std::max({r(), g(), b()});
  const float mn = std::min({r(), g(), b()});
  const float d = mx - mn;
  v = mx;
  s = mx == 0.f ? 0.f : d / mx;
  h = hueOf(r(), g(), b(), mx, d);
}

void CColor::fromHSL(float h, float s, float l, float _a) {
  if (s == 0.f) {
    mSimd = simd<float>(l, l, l, _a);
    return;
  }
  auto channel = [](float p, float q, float t) {
    if (t < 0.f)
      t += 1.f;
    if (t > 1.f)
      t -= 1.f;
    if (t < 1.f / 6.f)
      return p + (q - p) * 6.f * t;
    if (t < 0.5f)
      return q;
    if (t < 2.f / 3.f)
      return p + (q - p) * (2.f / 3.f - t) * 6.f;
    return p;
  };
  const float q = l < 0.5f ? l * (1.f + s) : l + s - l * s;
  const float p = 2.f * l - q;
  mSimd = simd<float>(channel(p, q, h + 1.f / 3.f), channel(p, q, h), channel(p, q, h - 1.f / 3.f), _a);
}

void CColor::toHSL(float& h, float& s, float& l) const {
  const float mx = std::max({r(), g(), b()});
  const float mn = std::min({r(), g(), b()});
  const float d = mx - mn;
  l = (mx + mn) * 0.5f;
  s = d == 0.f ? 0.f : (l > 0.5f ? d / (2.f - mx - mn) : d / (mx + mn));
  h = hueOf(r(), g(), b(), mx, d);
}

// Gray of equal RGB magnitude: sqrt((r^2 + g^2 + b^2) / 3); alpha is kept.
CColor CColor::toGrayscale() const {
  const float gray = std::sqrt(rgbDot(*this) / 3.f);
  return {gray, gray, gray, a()};
}
} // namespace zeus

