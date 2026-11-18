// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#pragma once
#include <cassert>

#include "zeus/CLineSeg.hpp"
#include "zeus/CPlane.hpp"
#include "zeus/CSphere.hpp"
#include "zeus/CTransform.hpp"
#include "zeus/CVector3f.hpp"
#include "zeus/Math.hpp"

namespace zeus {
class CVector2f;
class CMatrix4f;

class CAABox {
public:
  enum class EBoxEdgeId { Z0, X0, Z1, X1, Z2, X2, Z3, X3, Y0, Y1, Y2, Y3 };
  enum class EBoxFaceId { YMin, YMax, XMin, XMax, ZMax, ZMin };

  CVector3f min;
  CVector3f max;

  // The default box is inverted (empty), ready for accumulateBounds().
  constexpr CAABox() : min(1e16f), max(-1e16f) {}
  constexpr CAABox(const CVector3f& min, const CVector3f& max) : min(min), max(max) {}
  constexpr CAABox(float min, float max) : min(min, min, min), max(max, max, max) {}
  constexpr CAABox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
  : min(minX, minY, minZ), max(maxX, maxY, maxZ) {}

  bool intersects(const CAABox& other) const {
    return max.x() >= other.min.x() && min.x() <= other.max.x() && max.y() >= other.min.y() &&
           min.y() <= other.max.y() && max.z() >= other.min.z() && min.z() <= other.max.z();
  }
  bool intersects(const CSphere& other) const;
  float intersectionRadius(const CSphere& other) const;
  CAABox booleanIntersection(const CAABox& other) const;
  bool inside(const CAABox& other) const;
  bool insidePlane(const CPlane& plane) const;
  CVector3f center() const { return (min + max) * 0.5f; }
  CVector3f extents() const { return (max - min) * 0.5f; }
  float volume() const {
    const CVector3f d = max - min;
    return d.x() * d.y() * d.z();
  }
  CLineSeg getEdge(EBoxEdgeId id) const;

  struct Tri {
    zeus::CPlane x0_plane;
    zeus::CVector3f x10_verts[3];
  };
  Tri getTri(EBoxFaceId face, int windOffset) const;
  CAABox getTransformedAABox(const CTransform& xfrm) const;
  void accumulateBounds(const CVector3f& point) {
    for (int i = 0; i < 3; ++i) {
      if (point[i] < min[i])
        min[i] = point[i];
      if (point[i] > max[i])
        max[i] = point[i];
    }
  }
  void accumulateBounds(const CAABox& other) {
    accumulateBounds(other.min);
    accumulateBounds(other.max);
  }
  bool pointInside(const CVector3f& other) const {
    return other.x() >= min.x() && other.x() <= max.x() && other.y() >= min.y() && other.y() <= max.y() &&
           other.z() >= min.z() && other.z() <= max.z();
  }
  CVector3f closestPointAlongVector(const CVector3f& other) const {
    return {other.x() >= 0.f ? min.x() : max.x(), other.y() >= 0.f ? min.y() : max.y(),
            other.z() >= 0.f ? min.z() : max.z()};
  }
  CVector3f furthestPointAlongVector(const CVector3f& other) const {
    return {other.x() >= 0.f ? max.x() : min.x(), other.y() >= 0.f ? max.y() : min.y(),
            other.z() >= 0.f ? max.z() : min.z()};
  }
  float distanceBetween(const CAABox& other) const;
  CVector3f getPoint(const int point) const;
  CVector3f clampToBox(const CVector3f& vec) const {
    return {zeus::clamp(min.x(), vec.x(), max.x()), zeus::clamp(min.y(), vec.y(), max.y()),
            zeus::clamp(min.z(), vec.z(), max.z())};
  }
  bool projectedPointTest(const CMatrix4f& mvp, const CVector2f& point) const;
  void splitX(CAABox& negX, CAABox& posX) const;
  void splitY(CAABox& negY, CAABox& posY) const;
  void splitZ(CAABox& negZ, CAABox& posZ) const;
  bool invalid() const { return max.x() < min.x() || max.y() < min.y() || max.z() < min.z(); }
  // Lanes 0..2 are min, 3..5 are max.
  float operator[](size_t idx) const { return idx < 3 ? min[idx] : max[idx - 3]; }
};

constexpr inline CAABox skInvertedBox;
constexpr inline CAABox skNullBox(CVector3f{}, CVector3f{});

inline bool operator==(const CAABox& left, const CAABox& right) {
  return left.min == right.min && left.max == right.max;
}
inline bool operator!=(const CAABox& left, const CAABox& right) { return !(left == right); }
} // namespace zeus
