// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include "zeus/CVector3f.hpp"

namespace zeus {
class CQuaternion;
class CTransform;
// Rotation angles about X, Y and Z (radians).
class CEulerAngles : public CVector3f {
public:
  constexpr CEulerAngles(float x, float y, float z) : CVector3f(x, y, z) {}
  explicit CEulerAngles(const CQuaternion& quat);
  explicit CEulerAngles(const CTransform& xf);
  explicit CEulerAngles(const CVector3f& vec) : CVector3f(vec) {}
};
} // namespace zeus
