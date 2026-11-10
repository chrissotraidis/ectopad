// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include "zeus/CVector3f.hpp"

namespace zeus {
class CLineSeg {
public:
  CLineSeg(const CVector3f& start, const CVector3f& end) : x0_start(start), x18_end(end) {
    xc_dir = (end - start).normalized();
  }
  CVector3f x0_start;
  CVector3f xc_dir;
  CVector3f x18_end;
};
} // namespace zeus

