// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cstdio>
#include <cstdlib>

#include "zeus/CMatrix4f.hpp"

namespace zeus {
enum class EProjType { None = 0, Orthographic = 1, Perspective = 2 };

class SProjOrtho {
public:
  float top, bottom, left, right, znear, zfar;
  explicit SProjOrtho(float p_top = 1.0f, float p_bottom = -1.0f, float p_left = -1.0f, float p_right = 1.0f,
                      float p_near = 1.0f, float p_far = -1.0f)
  : top(p_top), bottom(p_bottom), left(p_left), right(p_right), znear(p_near), zfar(p_far) {}
};
struct SProjPersp {
  float fov, aspect, znear, zfar;
  SProjPersp(float p_fov = degToRad(55.0f), float p_aspect = 1.0f, float p_near = 0.1f, float p_far = 4096.f)
  : fov(p_fov), aspect(p_aspect), znear(p_near), zfar(p_far) {}
};
extern const SProjOrtho kOrthoIdentity;

class CProjection {
  void _updateCachedMatrix();

public:
  CProjection() : m_projType(EProjType::Orthographic), m_ortho(), m_mtx() {}
  CProjection(const CProjection& other) { *this = other; }
  CProjection(const SProjOrtho& ortho) { setOrtho(ortho); }
  CProjection(const SProjPersp& persp) { setPersp(persp); }
  CProjection& operator=(const CProjection& other) {
    if (this != &other) {
      m_projType = other.m_projType;
      if (m_projType == EProjType::Perspective)
        m_persp = other.m_persp;
      else
        m_ortho = other.m_ortho;
      m_mtx = other.m_mtx;
    }
    return *this;
  }
  void setOrtho(const SProjOrtho& ortho) {
    m_projType = EProjType::Orthographic;
    m_ortho = ortho;
    _updateCachedMatrix();
  }
  void setPersp(const SProjPersp& persp) {
    m_projType = EProjType::Perspective;
    m_persp = persp;
    _updateCachedMatrix();
  }
  EProjType getType() const { return m_projType; }
  const SProjOrtho& getOrtho() const { return m_ortho; }
  const SProjPersp& getPersp() const { return m_persp; }
  const CMatrix4f& getCachedMatrix() const { return m_mtx; }

protected:
  EProjType m_projType = EProjType::None;
  union {
    SProjOrtho m_ortho;
    SProjPersp m_persp;
  };
  CMatrix4f m_mtx;
};
} // namespace zeus

