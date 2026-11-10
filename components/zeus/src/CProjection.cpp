// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CProjection.hpp"

namespace zeus {
const SProjOrtho kOrthoIdentity;

void CProjection::_updateCachedMatrix() {
  if (m_projType == EProjType::Perspective) {
    // OpenGL-style perspective: depth maps to [-1, 1].
    const float yScale = 1.f / std::tan(m_persp.fov * 0.5f);
    const float xScale = yScale / m_persp.aspect;
    const float n = m_persp.znear, f = m_persp.zfar;
    m_mtx = CMatrix4f(xScale, 0.f, 0.f, 0.f, 0.f, yScale, 0.f, 0.f, 0.f, 0.f, -(f + n) / (f - n),
                      -2.f * f * n / (f - n), 0.f, 0.f, -1.f, 0.f);
  } else {
    // Orthographic: depth maps to [0, 1].
    const SProjOrtho& o = m_ortho;
    const float w = o.right - o.left, h = o.top - o.bottom, d = o.zfar - o.znear;
    m_mtx = CMatrix4f(2.f / w, 0.f, 0.f, -(o.right + o.left) / w, 0.f, 2.f / h, 0.f, -(o.top + o.bottom) / h, 0.f, 0.f,
                      -1.f / d, -o.zfar / d, 0.f, 0.f, 0.f, 1.f);
  }
}
} // namespace zeus

