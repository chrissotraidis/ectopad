// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CEulerAngles.hpp"

#include "zeus/CQuaternion.hpp"

namespace zeus {
// Angles from the normalized quaternion: x = asin(2(wx + yz)),
// y = atan2(2(wy - xz), 1 - 2(x^2 + y^2)), z = atan2(2(wz - xy), 1 - 2(x^2 + z^2)).
namespace {
// atan2 that reads 0 when both arguments are rounding noise (gimbal lock).
float stableAtan2(float y, float x) {
  if (std::fabs(y) < 1e-6f && std::fabs(x) < 1e-6f)
    return 0.f;
  return std::atan2(y, x);
}
} // namespace

CEulerAngles::CEulerAngles(const CQuaternion& quat) {
  const CQuaternion q = quat.normalized();
  const float w = q.w(), x = q.x(), y = q.y(), z = q.z();
  assign(std::asin(std::clamp(2.f * (w * x + y * z), -1.f, 1.f)),
         stableAtan2(2.f * (w * y - x * z), 1.f - 2.f * (x * x + y * y)),
         stableAtan2(2.f * (w * z - x * y), 1.f - 2.f * (x * x + z * z)));
}
CEulerAngles::CEulerAngles(const CTransform& xf) : CEulerAngles(CQuaternion(xf.basis)) {}
} // namespace zeus
