// SPDX-License-Identifier: GPL-3.0-or-later
// Part of EctoPad's zeus math library (independent implementation; see components/zeus/README.md).
#include "zeus/CAABox.hpp"

#include <algorithm>
#include <array>

#include "zeus/CMatrix4f.hpp"
#include "zeus/CVector2f.hpp"

namespace zeus {
namespace {
float squaredDistanceTo(const CAABox& box, const CVector3f& p) {
  float dist = 0.f;
  for (int i = 0; i < 3; ++i) {
    if (p[i] < box.min[i])
      dist += (p[i] - box.min[i]) * (p[i] - box.min[i]);
    else if (p[i] > box.max[i])
      dist += (p[i] - box.max[i]) * (p[i] - box.max[i]);
  }
  return dist;
}
} // namespace

bool CAABox::intersects(const CSphere& other) const {
  return squaredDistanceTo(*this, other.position) <= other.radius * other.radius;
}

// Squared distance from the sphere center to the box, or -1 when they do not touch.
float CAABox::intersectionRadius(const CSphere& other) const {
  const float dist = squaredDistanceTo(*this, other.position);
  return dist <= other.radius * other.radius ? dist : -1.f;
}

// Per axis, the overlap of the two ranges; an axis whose ranges do not touch becomes [0, 0].
CAABox CAABox::booleanIntersection(const CAABox& other) const {
  CAABox ret(0.f, 0.f);
  for (int i = 0; i < 3; ++i) {
    if (max[i] >= other.min[i] && other.max[i] >= min[i]) {
      ret.min[i] = std::max(min[i], other.min[i]);
      ret.max[i] = std::min(max[i], other.max[i]);
    }
  }
  return ret;
}

bool CAABox::inside(const CAABox& other) const {
  return min.x() >= other.min.x() && max.x() <= other.max.x() && min.y() >= other.min.y() &&
         max.y() <= other.max.y() && min.z() >= other.min.z() && max.z() <= other.max.z();
}

bool CAABox::insidePlane(const CPlane& plane) const {
  return plane.pointToPlaneDist(furthestPointAlongVector(plane.normal())) >= 0.f;
}

// Corner i: bit 0 selects max x, bit 1 max y, bit 2 max z.
CVector3f CAABox::getPoint(const int point) const {
  return {(point & 1) ? max.x() : min.x(), (point & 2) ? max.y() : min.y(), (point & 4) ? max.z() : min.z()};
}

CLineSeg CAABox::getEdge(EBoxEdgeId id) const {
  static constexpr int kEdges[12][2] = {{4, 0}, {0, 1}, {1, 5}, {5, 4}, {7, 3}, {3, 2},
                                        {2, 6}, {6, 7}, {4, 6}, {0, 2}, {1, 3}, {5, 7}};
  const auto& e = kEdges[int(id)];
  return CLineSeg(getPoint(e[0]), getPoint(e[1]));
}

// Matches the original's output: the triangle alternates two corners of the face
// (so it is degenerate and its plane is NaN), starting with the second when
// windOffset is odd.
CAABox::Tri CAABox::getTri(EBoxFaceId face, int windOffset) const {
  static constexpr int kFaces[6][2] = {{4, 5}, {7, 6}, {6, 4}, {5, 7}, {6, 7}, {0, 1}};
  const auto& f = kFaces[int(face)];
  const CVector3f a = getPoint(f[windOffset & 1]);
  const CVector3f b = getPoint(f[(windOffset + 1) & 1]);
  return {CPlane(a, b, a), {a, b, a}};
}

CAABox CAABox::getTransformedAABox(const CTransform& xfrm) const {
  CAABox box;
  for (int i = 0; i < 8; ++i)
    box.accumulateBounds(xfrm * getPoint(i));
  return box;
}

float CAABox::distanceBetween(const CAABox& other) const {
  float dist = 0.f;
  for (int i = 0; i < 3; ++i) {
    float gap = 0.f;
    if (max[i] < other.min[i])
      gap = other.min[i] - max[i];
    else if (other.max[i] < min[i])
      gap = min[i] - other.max[i];
    dist += gap * gap;
  }
  return std::sqrt(dist);
}

// Whether the 2D point (normalized device coordinates) falls inside the box's projected
// outline: the convex hull of its eight projected corners.
bool CAABox::projectedPointTest(const CMatrix4f& mvp, const CVector2f& point) const {
  std::array<CVector2f, 8> pts;
  for (int i = 0; i < 8; ++i)
    pts[i] = mvp.multiplyOneOverW(getPoint(i)).toVec2f();
  std::sort(pts.begin(), pts.end(), [](const CVector2f& a, const CVector2f& b) {
    return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
  });
  auto turn = [](const CVector2f& o, const CVector2f& a, const CVector2f& b) { return (a - o).cross(b - o); };
  // Andrew's monotone chain, counter-clockwise.
  std::array<CVector2f, 16> hull;
  int n = 0;
  for (int i = 0; i < 8; ++i) {
    while (n >= 2 && turn(hull[n - 2], hull[n - 1], pts[i]) <= 0.f)
      --n;
    hull[n++] = pts[i];
  }
  for (int i = 6, lower = n + 1; i >= 0; --i) {
    while (n >= lower && turn(hull[n - 2], hull[n - 1], pts[i]) <= 0.f)
      --n;
    hull[n++] = pts[i];
  }
  --n;
  if (n < 3)
    return false;
  for (int i = 0; i < n; ++i)
    if (turn(hull[i], hull[(i + 1) % n], point) < 0.f)
      return false;
  return true;
}

void CAABox::splitX(CAABox& negX, CAABox& posX) const {
  const float mid = (min.x() + max.x()) * 0.5f;
  negX = {min, {mid, max.y(), max.z()}};
  posX = {{mid, min.y(), min.z()}, max};
}
void CAABox::splitY(CAABox& negY, CAABox& posY) const {
  const float mid = (min.y() + max.y()) * 0.5f;
  negY = {min, {max.x(), mid, max.z()}};
  posY = {{min.x(), mid, min.z()}, max};
}
void CAABox::splitZ(CAABox& negZ, CAABox& posZ) const {
  const float mid = (min.z() + max.z()) * 0.5f;
  negZ = {min, {max.x(), max.y(), mid}};
  posZ = {{min.x(), min.y(), mid}, max};
}
} // namespace zeus
