// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cstddef>

#include "zeus/simd/simd.hpp"

namespace zeus {
template <typename SizeT>
constexpr void hash_combine_impl(SizeT& seed, SizeT value) noexcept {
  seed ^= value + SizeT(0x9e3779b97f4a7c15ull) + (seed << 6) + (seed >> 2);
}
} // namespace zeus
