// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"

namespace zeus {
struct CMRay {
  CMRay(const CVector3f& start, const CVector3f& dirin, float len)
  : start(start), length(len), invLength(1.f / len), dir(dirin) {
    end = start + dir * length;
    delta = end - start;
  }
  CMRay(const CVector3f& start, const CVector3f& end, float len, float invLen)
  : start(start), end(end), delta(end - start), length(len), invLength(invLen) {
    dir = delta * invLength;
  }
  CMRay getInvUnscaledTransformRay(const CTransform& xfrm) const {
    const CTransform inv = xfrm.inverse();
    return CMRay(inv * start, inv * end, length, invLength);
  }
  CVector3f start;
  CVector3f end;
  CVector3f delta;
  float length;
  float invLength;
  CVector3f dir;
};
} // namespace zeus

