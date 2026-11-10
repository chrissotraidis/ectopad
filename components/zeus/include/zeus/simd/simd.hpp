// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
// Four-lane value storage behind zeus's vector types. Lanes past a type's
// dimension stay zero. Plain arrays keep the code portable; the compiler
// vectorizes the element-wise loops.
#include <array>
#include <cstddef>

namespace zeus {

template <typename T>
struct simd_mask {
  bool v[4] = {};
  constexpr bool operator[](size_t i) const { return v[i]; }
};

template <typename T>
struct alignas(16) simd {
  using value_type = T;
  using reference = T&;
  using mask_type = simd_mask<T>;
  T v[4];

  constexpr simd() : v{} {}
  constexpr simd(T s) : v{s, s, s, s} {}
  constexpr simd(T a, T b, T c, T d) : v{a, b, c, d} {}
  template <typename U>
  constexpr explicit simd(const simd<U>& o) : v{T(o.v[0]), T(o.v[1]), T(o.v[2]), T(o.v[3])} {}

  static constexpr size_t size() { return 4; }
  constexpr T& operator[](size_t i) { return v[i]; }
  constexpr T operator[](size_t i) const { return v[i]; }

#define ZEUS_SIMD_OP(op)                                                                                               \
  constexpr simd operator op(const simd& o) const {                                                                    \
    return {v[0] op o.v[0], v[1] op o.v[1], v[2] op o.v[2], v[3] op o.v[3]};                                           \
  }                                                                                                                    \
  constexpr simd& operator op##=(const simd& o) {                                                                      \
    for (int i = 0; i < 4; ++i)                                                                                        \
      v[i] op## = o.v[i];                                                                                              \
    return *this;                                                                                                      \
  }
  ZEUS_SIMD_OP(+)
  ZEUS_SIMD_OP(-)
  ZEUS_SIMD_OP(*)
  ZEUS_SIMD_OP(/)
#undef ZEUS_SIMD_OP
  constexpr simd operator-() const { return {-v[0], -v[1], -v[2], -v[3]}; }

  constexpr T dot2(const simd& o) const { return v[0] * o.v[0] + v[1] * o.v[1]; }
  constexpr T dot3(const simd& o) const { return v[0] * o.v[0] + v[1] * o.v[1] + v[2] * o.v[2]; }
  constexpr T dot4(const simd& o) const { return v[0] * o.v[0] + v[1] * o.v[1] + v[2] * o.v[2] + v[3] * o.v[3]; }
};

template <typename T>
constexpr simd<T> operator+(T s, const simd<T>& a) { return simd<T>(s) + a; }
template <typename T>
constexpr simd<T> operator-(T s, const simd<T>& a) { return simd<T>(s) - a; }
template <typename T>
constexpr simd<T> operator*(T s, const simd<T>& a) { return simd<T>(s) * a; }
template <typename T>
constexpr simd<T> operator/(T s, const simd<T>& a) { return simd<T>(s) / a; }

// A plain copy of a vector's lanes, for code that wants a T* (simd_floats).
template <typename T>
struct simd_values {
  std::array<T, 4> lanes{};
  constexpr simd_values() = default;
  constexpr simd_values(const simd<T>& s) : lanes{s.v[0], s.v[1], s.v[2], s.v[3]} {}
  constexpr T* data() { return lanes.data(); }
  constexpr const T* data() const { return lanes.data(); }
  constexpr T& operator[](size_t i) { return lanes[i]; }
  constexpr T operator[](size_t i) const { return lanes[i]; }
};
using simd_floats = simd_values<float>;
using simd_doubles = simd_values<double>;

} // namespace zeus
