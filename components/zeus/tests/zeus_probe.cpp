// SPDX-License-Identifier: GPL-3.0-or-later
// Behavioral probe for EctoPad's zeus math library. It drives the public API
// with fixed inputs and prints every result, so two builds of the library can
// be compared line by line (diff their outputs).
#include <zeus/zeus.hpp>
#include <zeus/CEulerAngles.hpp>
#include <zeus/CLine.hpp>
#include <zeus/CLineSeg.hpp>
#include <zeus/CMRay.hpp>
#include <zeus/COBBox.hpp>
#include <zeus/CRectangle.hpp>
#include <zeus/CSphere.hpp>
#include <zeus/CVector2d.hpp>
#include <zeus/CVector2i.hpp>
#include <zeus/CVector3d.hpp>
#include <zeus/CVector4d.hpp>

#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>

using namespace zeus;

static uint32_t rngState = 12345;
static float rnd(float lo = -10.f, float hi = 10.f) {
  rngState = rngState * 1664525u + 1013904223u;
  return lo + (hi - lo) * float(rngState >> 8) / float(1u << 24);
}
static CVector3f rv(float s = 10.f) { return {rnd(-s, s), rnd(-s, s), rnd(-s, s)}; }
static CVector3f rn() { return rv(1.f).normalized(); }

static void p(const char* n, double v) { std::printf("%s %.6g\n", n, v); }
static void p(const char* n, const CVector2f& v) { std::printf("%s %.6g %.6g\n", n, v.x(), v.y()); }
static void p(const char* n, const CVector3f& v) { std::printf("%s %.6g %.6g %.6g\n", n, v.x(), v.y(), v.z()); }
static void p(const char* n, const CVector4f& v) { std::printf("%s %.6g %.6g %.6g %.6g\n", n, v.x(), v.y(), v.z(), v.w()); }
static void p(const char* n, const CColor& v) { std::printf("%s %.6g %.6g %.6g %.6g\n", n, v.r(), v.g(), v.b(), v.a()); }
static void p(const char* n, const CQuaternion& q) { std::printf("%s %.6g %.6g %.6g %.6g\n", n, q.w(), q.x(), q.y(), q.z()); }
static void p(const char* n, const CNUQuaternion& q) { std::printf("%s %.6g %.6g %.6g %.6g\n", n, q.w(), q.x(), q.y(), q.z()); }
static void p(const char* n, const CMatrix3f& m) {
  std::printf("%s", n);
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      std::printf(" %.6g", m[i][j]);
  std::printf("\n");
}
static void p(const char* n, const CMatrix4f& m) {
  std::printf("%s", n);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      std::printf(" %.6g", m[i][j]);
  std::printf("\n");
}
static void p(const char* n, const CTransform& t) {
  std::string s = std::string(n) + ".basis";
  p(s.c_str(), t.basis);
  s = std::string(n) + ".origin";
  p(s.c_str(), t.origin);
}
static void p(const char* n, const CAABox& b) {
  std::string s = std::string(n) + ".min";
  p(s.c_str(), b.min);
  s = std::string(n) + ".max";
  p(s.c_str(), b.max);
}
static void p(const char* n, const CPlane& pl) { std::printf("%s %.6g %.6g %.6g %.6g\n", n, pl.x(), pl.y(), pl.z(), pl.d()); }
static void pb(const char* n, bool b) { std::printf("%s %d\n", n, int(b)); }

static CTransform rxf() {
  return CTransform::Translate(rv()) * CTransform::RotateZ(rnd(-3, 3)) * CTransform::RotateY(rnd(-3, 3)) *
         CTransform::RotateX(rnd(-3, 3));
}
static CQuaternion rq() { return CQuaternion(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1), rnd(-1, 1)).normalized(); }

int main() {
  std::printf("sizeof v2f=%zu v3f=%zu v4f=%zu v2d=%zu v3d=%zu v4d=%zu col=%zu q=%zu m3=%zu m4=%zu xf=%zu aabb=%zu "
              "plane=%zu obb=%zu ray=%zu frus=%zu proj=%zu rel=%zu v2i=%zu\n",
              sizeof(CVector2f), sizeof(CVector3f), sizeof(CVector4f), sizeof(CVector2d), sizeof(CVector3d),
              sizeof(CVector4d), sizeof(CColor), sizeof(CQuaternion), sizeof(CMatrix3f), sizeof(CMatrix4f),
              sizeof(CTransform), sizeof(CAABox), sizeof(CPlane), sizeof(COBBox), sizeof(CMRay), sizeof(CFrustum),
              sizeof(CProjection), sizeof(CRelAngle), sizeof(CVector2i));
  std::printf("alignof v2f=%zu v3f=%zu v4f=%zu v3d=%zu col=%zu m3=%zu xf=%zu\n", alignof(CVector2f),
              alignof(CVector3f), alignof(CVector4f), alignof(CVector3d), alignof(CColor), alignof(CMatrix3f),
              alignof(CTransform));
  {
    CVector3f v(1.f, 2.f, 3.f);
    float raw[4];
    std::memcpy(raw, &v, sizeof(raw));
    std::printf("v3f raw %g %g %g %g\n", raw[0], raw[1], raw[2], raw[3]);
    CVector2f w(4.f, 5.f);
    float raw2[4] = {};
    std::memcpy(raw2, &w, sizeof(CVector2f));
    std::printf("v2f raw %g %g %g %g\n", raw2[0], raw2[1], raw2[2], raw2[3]);
  }
  // constants
  p("skUp", skUp); p("skForward", skForward); p("skRight", skRight); p("skOrange", skOrange); p("skPurple", skPurple);
  p("skGrey", skGrey); p("skInvertedBox", skInvertedBox); p("skIdentity4", skIdentityMatrix4f);
  p("M_PIF", M_PIF); p("M_SQRT1_2F", M_SQRT1_2F);

  // scalar Math
  for (int i = 0; i < 6; ++i) {
    float a = rnd(), b = rnd(), c = rnd(), d = rnd(), t = rnd(0, 1);
    p("catmull1", getCatmullRomSplinePoint(a, b, c, d, t));
    CVector3f A = rv(), B = rv(), C = rv(), D = rv();
    p("catmull3", getCatmullRomSplinePoint(A, B, C, D, t));
    p("roundcatmull3", getRoundCatmullRomSplinePoint(A, B, C, D, t));
    p("bezier", getBezierPoint(A, B, C, D, t));
    p("bary", baryToWorld(A, B, C, CVector3f(t, 1 - t, 0.25f)));
    p("invSqrtF", invSqrtF(std::fabs(a) + 0.01f));
    p("invSqrtD", invSqrtD(std::fabs(a) + 0.01));
    p("powD", powD(std::fabs(a), b * 0.2f));
    p("round", zeus::round(double(a)));
    p("rad2deg", radToDeg(a)); p("deg2rad", degToRad(a));
    p("clamp", clamp(-1.f, a, 1.f));
    p("vmin", zeus::min(A, B)); p("vmax", zeus::max(A, B));
  }
  for (int x : {0, 1, 2, 3, 5, 7, 8, 9, 100, 1023, 1024, 1025, -4})
    std::printf("pow2 %d %d %d\n", x, floorPowerOfTwo(x), ceilingPowerOfTwo(x));
  std::printf("popcount %d %d\n", PopCount(0xF0F0u), PopCount(uint64_t(0x8000000000000001ull)));
  for (float e : {0.f, 1e-8f, 1.1e-7f, 1.2e-7f, 1e-6f, 1e-3f})
    std::printf("close %g %d %d %d %d\n", e, close_enough(1.f, 1.f + e), close_enough(1.f, 1.f + e, 1e-3),
                close_enough(CVector3f(1.f), CVector3f(1.f + e)), close_enough(CVector2f(1.f), CVector2f(1.f + e)));

  // CVector2f
  for (int i = 0; i < 4; ++i) {
    CVector2f a(rnd(), rnd()), b(rnd(), rnd());
    float t = rnd(0, 1);
    p("v2 add", a + b); p("v2 mul", a * b); p("v2 div", a / b); p("v2 sdiv", 2.f / a); p("v2 neg", -a);
    p("v2 norm", a.normalized()); p("v2 perp", a.perpendicularVector()); p("v2 cross", a.cross(b));
    p("v2 dot", a.dot(b)); p("v2 mag", a.magnitude()); p("v2 angle", CVector2f::getAngleDiff(a, b));
    p("v2 lerp", CVector2f::lerp(a, b, t)); p("v2 nlerp", CVector2f::nlerp(a, b, t));
    p("v2 slerp", CVector2f::slerp(a.normalized(), b.normalized(), t));
    pb("v2 cmp", a < b); pb("v2 cmp2", a <= b); pb("v2 cmp3", a > b); pb("v2 cmp4", a >= b);
    pb("v2 isNorm", a.isNormalized()); pb("v2 isNorm2", a.normalized().isNormalized());
  }
  pb("v2 zero", CVector2f(0.f).isZero()); pb("v2 canNorm", CVector2f(1e-7f, 0).canBeNormalized());
  pb("v2 canNorm2", CVector2f(1e-3f, 0).canBeNormalized());
  p("v2 normzero", CVector2f(0.f).normalized());
  pb("v2 isEqu", CVector2f(1.f).isEqu(CVector2f(1.f + 1e-6f)));

  // CVector3f
  for (int i = 0; i < 6; ++i) {
    CVector3f a = rv(), b = rv();
    float t = rnd(0, 1);
    p("v3 cross", a.cross(b)); p("v3 dot", a.dot(b)); p("v3 mag", a.magnitude()); p("v3 norm", a.normalized());
    p("v3 sdiv", 3.f / a); p("v3 ssub", 3.f - a); p("v3 angle", CVector3f::getAngleDiff(a, b));
    p("v3 lerp", CVector3f::lerp(a, b, t)); p("v3 nlerp", CVector3f::nlerp(a, b, t));
    p("v3 slerp", CVector3f::slerp(a, b, CRelAngle(t)));
    p("v3 slerpn", CVector3f::slerp(a.normalized(), b.normalized(), CRelAngle(t * 3.f)));
    p("v3 scaled", a.scaledToLength(2.5f)); p("v3 toVec2", a.toVec2f()); p("v3 r2d", CVector3f::radToDeg(a)); p("v3 d2r", CVector3f::degToRad(a));
    CVector3f c = a; c.scaleToLength(0.5f); p("v3 scaleTo", c);
    pb("v3 isNorm", a.normalized().isNormalized()); pb("v3 eq", a == b); pb("v3 ne", a != a);
    pb("v3 safe", a.isMagnitudeSafe()); pb("v3 nan", a.isNaN()); pb("v3 notinf", a.isNotInf());
    p("v3 fromv2", CVector3f(a.toVec2f(), 7.f)); p("v3 fromd", CVector3f(CVector3d(a)));
  }
  for (float s : {0.f, 1e-8f, 1e-7f, 1e-6f, 1e-5f, 1e-4f, 1e-3f, 1e-2f}) {
    CVector3f v(s, 0.f, 0.f);
    std::printf("v3 thresholds %g zero=%d can=%d safe=%d norm=%d\n", s, v.isZero(), v.canBeNormalized(),
                v.isMagnitudeSafe(), CVector3f(1.f + s, 0, 0).isNormalized());
    p("v3 normsmall", v.normalized());
    pb("v3 isEqu", CVector3f(1.f).isEqu(CVector3f(1.f + s)));
    pb("v3 isEquE", CVector3f(1.f).isEqu(CVector3f(1.f + s), 1e-5f));
  }
  p("v3 slerp same", CVector3f::slerp(skUp, skUp, CRelAngle(0.5f)));
  p("v3 slerp opp", CVector3f::slerp(skUp, skDown, CRelAngle(0.5f)));
  p("v3 angle zero", CVector3f::getAngleDiff(skZero3f, skUp));
  {
    CVector3f v(1.f, 2.f, 3.f);
    v.x() = 9.f; v[1] = 8.f; p("v3 assign", v);
    v.splat(4.f); p("v3 splat", v); v.zeroOut(); p("v3 zero", v);
    v.assign(1, 2, 3); v += CVector3f(1.f); v -= CVector3f(0.5f); v *= CVector3f(2.f); v /= CVector3f(4.f); p("v3 ops", v);
    float arr[3] = {5, 6, 7}; p("v3 ptr", CVector3f(arr));
  }

  // CVector4f
  for (int i = 0; i < 3; ++i) {
    CVector4f a(rnd(), rnd(), rnd(), rnd()), b(rnd(), rnd(), rnd(), rnd());
    p("v4 norm", a.normalized()); p("v4 dot", a.dot(b)); p("v4 lerp", CVector4f::lerp(a, b, 0.3f));
    p("v4 nlerp", CVector4f::nlerp(a, b, 0.3f)); p("v4 tov3", a.toVec3f());
    p("v4 fromv3", CVector4f(a.toVec3f())); p("v4 fromv3w", CVector4f(a.toVec3f(), 0.f));
    p("v4 clip", CVector4f::ToClip(a.toVec3f(), 2.f));
    pb("v4 cmp", a < b); pb("v4 cmp2", a >= b);
  }

  // CColor
  {
    CColor c(0x11223344u);
    p("col u32", c); std::printf("col toRGBA %08x\n", c.toRGBA());
    Comp8 bytes[4] = {0x10, 0x80, 0xff, 0x40};
    p("col bytes", CColor(bytes));
    std::printf("col COLOR %08x\n", COLOR(0x11223344));
    for (int i = 0; i < 8; ++i) {
      CColor a(rnd(0, 1), rnd(0, 1), rnd(0, 1), rnd(0, 1));
      Comp8 r, g, b, al;
      a.toRGBA8(r, g, b, al);
      std::printf("col rgba8 %d %d %d %d rgb5a3 %04x rgba %08x\n", r, g, b, al, a.toRGB5A3(), a.toRGBA());
      float h, s, v, l;
      a.toHSV(h, s, v); std::printf("col hsv %.6g %.6g %.6g\n", h, s, v);
      CColor back; back.fromHSV(h, s, v, a.a()); p("col fromHSV", back);
      a.toHSL(h, s, l); std::printf("col hsl %.6g %.6g %.6g\n", h, s, l);
      back.fromHSL(h, s, l, a.a()); p("col fromHSL", back);
      p("col gray", a.toGrayscale()); p("col norm", a.normalized()); p("col lerp", CColor::lerp(a, skWhite, 0.25f));
      p("col nlerp", CColor::nlerp(a, skBlue, 0.25f)); p("col rgbdot", a.rgbDot(skGrey));
      CColor big = a * 3.f - 1.f; big.Clamp(); p("col clamp", big);
      CColor fr; fr.fromRGBA8(r, g, b, al); p("col fromRGBA8", fr);
      fr.fromRGBA32(a.toRGBA()); p("col fromRGBA32", fr);
      p("col mag", a.magnitude());
      std::printf("col hash %d\n", int(std::hash<CColor>()(a) == std::hash<CColor>()(a)));
    }
    for (float hh : {0.f, 0.1f, 0.3f, 0.5f, 0.7f, 0.9f, 1.f}) {
      CColor x; x.fromHSV(hh, 0.8f, 0.6f); p("col hsvsweep", x);
      x.fromHSL(hh, 0.8f, 0.6f); p("col hslsweep", x);
      x.fromHSL(hh, 0.5f, 0.3f); p("col hslsweep2", x);
    }
    CColor g(0.5f, 0.5f, 0.5f, 1.f); float h, s, v; g.toHSV(h, s, v); std::printf("col grayhsv %.6g %.6g %.6g\n", h, s, v);
    g.toHSL(h, s, v); std::printf("col grayhsl %.6g %.6g %.6g\n", h, s, v);
    p("col rgbA", CColor(0.25f, 0.5f)); p("col v4", CColor(CVector4f(1, 2, 3, 4)));
    CVector4f v4 = skOrange; p("col tov4", v4);
    CColor sp; sp.splat(0.3f, 0.7f); p("col splat", sp);
  }

  // CMatrix3f
  {
    CMatrix3f m(1, 2, 3, 4, 5, 6, 7, 8, 10);
    p("m3 ctor", m); p("m3 vec", m * CVector3f(1, 2, 3)); p("m3 T", m.transposed()); p("m3 inv", m.inverted());
    p("m3 det", m.determinant()); p("m3 mul", m * m.transposed()); p("m3 ortho", m.orthonormalized());
    p("m3 rows", CMatrix3f(CVector3f(1, 2, 3), CVector3f(4, 5, 6), CVector3f(7, 8, 9)));
    p("m3 scale", CMatrix3f(CVector3f(2, 3, 4))); p("m3 scalef", CMatrix3f(5.f)); p("m3 zero", CMatrix3f(true));
    p("m3 ident", CMatrix3f()); p("m3 rx", CMatrix3f::RotateX(0.5f)); p("m3 ry", CMatrix3f::RotateY(0.5f));
    p("m3 rz", CMatrix3f::RotateZ(0.5f));
    CMatrix3f n = m; n.addScaledMatrix(CMatrix3f(), 2.f); p("m3 addscaled", n);
    for (int i = 0; i < 4; ++i) { CQuaternion q = rq(); p("m3 fromq", CMatrix3f(q)); p("q fromm3", CQuaternion(CMatrix3f(q))); }
    pb("m3 eq", m == m); pb("m3 ne", m != m.transposed());
  }
  // CMatrix4f
  {
    CMatrix4f m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17);
    p("m4 ctor", m); p("m4 vec", m * CVector4f(1, 2, 3, 4)); p("m4 T", m.transposed()); p("m4 mul", m * m);
    p("m4 fromm3", CMatrix4f(CMatrix3f(1, 2, 3, 4, 5, 6, 7, 8, 9))); p("m4 scale", CMatrix4f(CVector3f(2, 3, 4)));
    float w; p("m4 oow", m.multiplyOneOverW(CVector3f(1, 2, 3))); p("m4 oow2", m.multiplyOneOverW(CVector3f(1, 2, 3), w)); p("m4 w", w);
  }
  // CTransform
  for (int i = 0; i < 5; ++i) {
    CTransform a = rxf(), b = rxf();
    CVector3f v = rv();
    p("xf mul", a * b); p("xf inv", a.inverse()); p("xf qinv", a.quickInverse()); p("xf vec", a * v);
    p("xf rot", a.rotate(v)); p("xf trot", a.transposeRotate(v)); p("xf getrot", a.getRotation());
    p("xf mit", a.multiplyIgnoreTranslation(b)); p("xf up", a.upVector()); p("xf front", a.frontVector());
    p("xf right", a.rightVector()); p("xf m4", a.toMatrix4f());
    float c[4][4] = {}; a.toCStyleMatrix(c);
    std::printf("xf cstyle"); for (int ri = 0; ri < 3; ++ri) for (float f : c[ri]) std::printf(" %.6g", f); std::printf("\n");
    CTransform s = a; s.scaleBy(2.f); p("xf scaleBy", s);
    s = a; s.rotateLocalX(0.3f); p("xf rlx", s); s.rotateLocalY(0.4f); p("xf rly", s); s.rotateLocalZ(0.5f); p("xf rlz", s);
    s = a * CTransform::Scale(1.f, 2.f, 3.f); s.orthonormalize(); p("xf ortho", s);
    p("xf plus", a + v); p("xf minus", a - v);
    s = a; s.setRotation(b.basis); p("xf setrot", s); s.setRotation(b); p("xf setrotx", s);
    p("xf lookat", lookAt(v, rv())); p("xf lookatup", lookAt(v, rv(), rn()));
    p("xf lookatvert", lookAt(skZero3f, CVector3f(0, 0, 5)));
    p("xf euler", CTransformFromEditorEuler(v)); p("xf eulers", CTransformFromEditorEulers(v, rv()));
    p("xf axisangle", CTransformFromAxisAngle(rn(), rnd(-3, 3))); p("xf scalevec", CTransformFromScaleVector(v));
    p("xf basedY", CTransform::MakeRotationsBasedOnY(CUnitVector3f(rn())));
    p("xf cols", CTransform(rv(), rv(), rv(), rv()));
    pb("xf eq", a == a); pb("xf ne", a != b);
    p("xf rx", CTransform::RotateX(0.7f)); p("xf scale3", CTransform::Scale(rv())); p("xf scale1", CTransform::Scale(3.f));
    p("xf translate", CTransform::Translate(1.f, 2.f, 3.f));
  }
  p("xf basedY up", CTransform::MakeRotationsBasedOnY(CUnitVector3f(skUp)));
  p("xf basedY fwd", CTransform::MakeRotationsBasedOnY(CUnitVector3f(skForward)));
  // CQuaternion
  for (int i = 0; i < 6; ++i) {
    CQuaternion a = rq(), b = rq();
    CVector3f v = rv();
    double t = rnd(0, 1);
    p("q mul", a * b); p("q div", a / b); p("q inv", a.inverse()); p("q transform", a.transform(v));
    p("q rotate", CQuaternion::rotate(a, CAxisAngle(v))); p("q log", a.log()); p("q exp", a.log().exp());
    p("q lerp", CQuaternion::lerp(a, b, t)); p("q slerp", CQuaternion::slerp(a, b, t));
    p("q slerpShort", CQuaternion::slerpShort(a, b, t)); p("q nlerp", CQuaternion::nlerp(a, b, t));
    p("q slerpneg", CQuaternion::slerp(a, -a * 0.999f, t));
    p("q roll", a.roll()); p("q pitch", a.pitch()); p("q yaw", a.yaw()); p("q equiv", a.buildEquivalent());
    p("q imag", a.getImaginary()); p("q anglefrom", float(a.angleFrom(b)));
    p("q euler", CQuaternion(v.x(), v.y(), v.z())); p("q eulerv", CQuaternion(v)); p("q wv", CQuaternion(0.5f, v));
    CQuaternion e; e.fromVector3f(v); p("q fromVector3f", e);
    p("q axisangle", CQuaternion::fromAxisAngle(CUnitVector3f(rn()), CRelAngle(rnd(-6, 6))));
    CQuaternion r = a; r.rotateX(CRelAngle(0.3f)); p("q rx", r); r.rotateY(CRelAngle(0.4f)); p("q ry", r);
    r.rotateZ(CRelAngle(0.5f)); p("q rz", r);
    CVector3f s = rn(), d = rn();
    p("q lookAt", CQuaternion::lookAt(CUnitVector3f(s), CUnitVector3f(d), CRelAngle(0.5f)));
    p("q lookAtBig", CQuaternion::lookAt(CUnitVector3f(s), CUnitVector3f(d), CRelAngle(4.f)));
    p("q arc", CQuaternion::shortestRotationArc(v, rv()));
    p("q clampedRotateTo", CQuaternion::clampedRotateTo(CUnitVector3f(s), CUnitVector3f(d), CRelAngle(0.3f)));
    p("q vecvec", CQuaternion(s, d)); p("q toxf", a.toTransform()); p("q toxfo", a.toTransform(v));
    p("q dot", a.dot(b)); p("q add", a + b); p("q sub", a - b); p("q scale", a * 2.f); p("q sdiv", a / 2.f);
    p("q fromv4", CQuaternion(CVector4f(1, 2, 3, 4)));
    p("q lmul", 2.f * a); p("q ladd", 2.f + a); p("q lsub", 2.f - a);
    CQuaternion x = a; x.setImaginary(v); p("q setimag", x); x.invert(); p("q invert", x);
    CNUQuaternion nu(a); nu = nu * 2.f; p("nu mul", nu * CNUQuaternion(b)); p("nu norm", nu.normalized());
    p("nu frommat", CNUQuaternion(CMatrix3f(a))); p("nu axis", CNUQuaternion::fromAxisAngle(CUnitVector3f(s), CRelAngle(1.f)));
    p("nu toq", CQuaternion::fromNUQuaternion(nu)); p("nu lmul", 3.f * nu);
    CNUQuaternion acc(1, 0, 0, 0); acc += nu; p("nu acc", acc); p("nu wv", CNUQuaternion(0.3f, v));
    p("ea fromq", CEulerAngles(a)); p("ea fromxf", CEulerAngles(a.toTransform())); p("ea v", CEulerAngles(v));
    p("aa angle", CAxisAngle(v).angle()); p("aa unit", CAxisAngle(CUnitVector3f(s), 2.f));
  }
  p("q arc opp", CQuaternion::shortestRotationArc(skUp, skDown));
  p("q arc same", CQuaternion::shortestRotationArc(skUp, skUp));
  p("q vecvec opp", CQuaternion(skUp, skDown));
  p("q lookAt same", CQuaternion::lookAt(CUnitVector3f(skUp), CUnitVector3f(skUp), CRelAngle(1.f)));
  p("q lookAt opp", CQuaternion::lookAt(CUnitVector3f(skUp), CUnitVector3f(skDown), CRelAngle(1.f)));
  p("q slerp same", CQuaternion::slerp(CQuaternion(), CQuaternion(), 0.5));
  p("q ident log", CQuaternion().log());
  p("ea gimbal", CEulerAngles(CQuaternion(M_PIF / 2, 0.f, 0.f)));
  p("ea gimbal2", CEulerAngles(CTransform::RotateX(M_PIF / 2) * CTransform::RotateZ(0.3f)));

  // CRelAngle
  for (float a : {-7.f, -3.2f, -0.1f, 0.f, 1.f, 3.2f, 6.2831855f, 7.f, 13.f}) {
    CRelAngle r(a);
    std::printf("rel %g make=%.6g asRel=%.6g deg=%.6g acos=%.6g\n", a, CRelAngle::MakeRelativeAngle(a),
                float(r.asRel()), r.asDegrees(), CRelAngle(a * 0.1f).arcCosine());
    r.makeRel(); p("rel makeRel", float(r));
    CRelAngle q = CRelAngle::FromDegrees(a * 30.f); q += 1.f; q -= CRelAngle(0.5f); q *= 2.f; q /= 3.f; p("rel ops", float(q));
  }
  // CAABox
  for (int i = 0; i < 4; ++i) {
    CVector3f c = rv(), e = rv(3.f);
    CAABox a(c - CVector3f(std::fabs(e.x()), std::fabs(e.y()), std::fabs(e.z())),
             c + CVector3f(std::fabs(e.x()), std::fabs(e.y()), std::fabs(e.z())));
    CAABox b(a.min + rv(2.f), a.max + rv(2.f));
    CTransform x = rxf();
    CSphere s(rv(), rnd(0.5f, 4.f));
    pb("aabb intersects", a.intersects(b)); pb("aabb intsphere", a.intersects(s));
    p("aabb intRadius", a.intersectionRadius(s)); p("aabb boolInt", a.booleanIntersection(b));
    pb("aabb inside", a.inside(b)); pb("aabb insidePlane", a.insidePlane(CPlane(rn(), rnd())));
    p("aabb center", a.center()); p("aabb extents", a.extents()); p("aabb vol", a.volume());
    for (int k = 0; k < 12; ++k) { CLineSeg l = a.getEdge(CAABox::EBoxEdgeId(k)); p("aabb edge.s", l.x0_start); p("aabb edge.d", l.xc_dir); p("aabb edge.e", l.x18_end); }
    for (int k = 0; k < 6; ++k) for (int w = 0; w < 2; ++w) { auto t = a.getTri(CAABox::EBoxFaceId(k), w); p("aabb tri.p", t.x0_plane); p("aabb tri.0", t.x10_verts[0]); p("aabb tri.1", t.x10_verts[1]); p("aabb tri.2", t.x10_verts[2]); }
    p("aabb xf", a.getTransformedAABox(x));
    CAABox acc = skInvertedBox; acc.accumulateBounds(rv()); acc.accumulateBounds(rv()); acc.accumulateBounds(b); p("aabb acc", acc);
    CVector3f pt = rv();
    pb("aabb ptIn", a.pointInside(pt)); p("aabb closest", a.closestPointAlongVector(pt)); p("aabb furthest", a.furthestPointAlongVector(pt));
    p("aabb dist", a.distanceBetween(b)); for (int k = 0; k < 8; ++k) p("aabb point", a.getPoint(k));
    p("aabb clampTo", a.clampToBox(pt));
    CAABox n, q; a.splitX(n, q); p("aabb splitXn", n); p("aabb splitXp", q); a.splitY(n, q); p("aabb splitY", n); a.splitZ(n, q); p("aabb splitZ", q);
    pb("aabb invalid", a.invalid()); pb("aabb invalid2", skInvertedBox.invalid()); p("aabb [4]", a[4]); p("aabb [0]", a[0]);
    pb("aabb eq", a == a); pb("aabb ne", a != b);
    CMatrix4f mvp = CProjection(SProjPersp(1.f, 1.3f, 0.1f, 100.f)).getCachedMatrix() * lookAt(rv(30.f), c).inverse().toMatrix4f();
    pb("aabb projTest", a.projectedPointTest(mvp, CVector2f(rnd(-1, 1), rnd(-1, 1))));
    COBBox o = COBBox::FromAABox(a, x);
    p("obb aabb", o.calculateAABox()); p("obb aabbxf", o.calculateAABox(rxf())); p("obb xf", o.transform); p("obb ext", o.extents);
    COBBox o2(rxf(), rv(3.f)); pb("obb obb", o.OBBIntersectsBox(o2)); pb("obb aab", o.AABoxIntersectsBox(b));
    COBBox o3(a); p("obb fromaabb", o3.transform); p("obb fromaabb.e", o3.extents);
    CMRay ray(rv(), rn(), 5.f); p("ray end", ray.end); p("ray delta", ray.delta); p("ray len", ray.length); p("ray inv", ray.invLength); p("ray dir", ray.dir);
    CMRay r2 = ray.getInvUnscaledTransformRay(x); p("ray2 start", r2.start); p("ray2 end", r2.end); p("ray2 dir", r2.dir); p("ray2 len", r2.length);
    CMRay r3(rv(), rv(), 2.f, 0.5f); p("ray3 dir", r3.dir); p("ray3 delta", r3.delta); p("ray3 end", r3.end);
    p("sphere normal", s.getSurfaceNormal(pt)); pb("sphere int", s.intersects(CSphere(rv(), 3.f)));
    CPlane pl(rv(), rv(), rv()); p("plane 3pt", pl); p("plane normal", pl.normal());
    p("plane dist", pl.pointToPlaneDist(pt)); CVector3f hit; pb("plane ray", pl.rayPlaneIntersection(rv(), rv(), hit)); p("plane hit", hit);
    p("plane clip", pl.clipLineSegment(rv(), rv())); CPlane p4(1, 2, 3, 4); p4.normalize(); p("plane normalize", p4);
    p("plane ptd", CPlane(rn(), 2.f));
    CLineSeg ls(rv(), rv()); p("lineseg dir", ls.xc_dir);
  }
  // CFrustum and CProjection
  {
    CProjection persp(SProjPersp(degToRad(60.f), 1.5f, 0.2f, 500.f));
    p("proj persp", persp.getCachedMatrix());
    CProjection ortho(SProjOrtho(2.f, -3.f, -4.f, 5.f, 0.5f, 50.f));
    p("proj ortho", ortho.getCachedMatrix());
    std::printf("proj type %d %d %d\n", int(persp.getType()), int(ortho.getType()), int(CProjection().getType()));
    p("proj default", CProjection().getCachedMatrix());
    CProjection cp = persp; cp.setOrtho(SProjOrtho()); p("proj setOrtho", cp.getCachedMatrix());
    p("proj getPersp", persp.getPersp().fov); p("proj getOrtho", ortho.getOrtho().left);
    SProjPersp dp; std::printf("persp default %.6g %.6g %.6g %.6g\n", dp.fov, dp.aspect, dp.znear, dp.zfar);
    SProjOrtho dor; std::printf("ortho default %.6g %.6g %.6g %.6g %.6g %.6g\n", dor.top, dor.bottom, dor.left, dor.right, dor.znear, dor.zfar);
    for (int i = 0; i < 4; ++i) {
      CTransform view = lookAt(rv(20.f), rv(5.f));
      CFrustum f; f.updatePlanes(view, persp);
      CFrustum g; g.updatePlanes(view.toMatrix4f(), persp.getCachedMatrix());
      CFrustum h; h.updatePlanes(view, ortho);
      for (int k = 0; k < 20; ++k) {
        CVector3f c = rv(30.f); CAABox b(c - CVector3f(1.f), c + CVector3f(2.f)); CSphere s(c, 2.f);
        std::printf("frustum %d %d %d %d %d %d %d\n", f.aabbFrustumTest(b), f.sphereFrustumTest(s), f.pointFrustumTest(c),
                    g.aabbFrustumTest(b), g.pointFrustumTest(c), h.aabbFrustumTest(b), h.sphereFrustumTest(s));
      }
    }
    CFrustum un; CAABox b(-1.f, 1.f);
    // default-constructed frustum behavior is not probed (uninitialized in some builds)
    (void)un; (void)b;
  }
  // misc small types
  {
    CRectangle r(1, 2, 3, 4); pb("rect contains", r.contains(CVector2f(2, 3))); pb("rect contains2", r.contains(CVector2f(5, 3)));
    pb("rect int", r.intersects(CRectangle(3.5f, 5.5f, 1, 1))); pb("rect int2", r.intersects(CRectangle(4.5f, 6.5f, 1, 1)));
    CVector2i i(3, -4); CVector2i j(CVector2f(2.7f, -1.6f)); std::printf("v2i %d %d %d %d\n", j.x, j.y, (i * 2).x, (i / CVector2i(2, 2)).y);
    p("v2i tof", i.toVec2f());
    CVector3d d(1.5, 2.5, 3.5); p("v3d norm", d.asNormalized().asCVector3f()); p("v3d cross", d.cross(CVector3d(3, 2, 1)).asCVector3f());
    p("v3d mag", d.magnitude());
    CVector2d e(3, 4); std::printf("v2d %.6g %.6g %.6g\n", e.magnitude(), double(e.normalized().x()), e.dot(CVector2d(1, 0)));
    CVector4d f(1, 2, 3, 4); std::printf("v4d %.6g %.6g\n", f.magnitude(), double(f.normalized().w()));
    CUnitVector3f u(3, 4, 0); p("unit", u); CUnitVector3f un(3, 4, 0, false); p("unit raw", un);
    CLine line(CVector3f(1, 2, 3), CVector3f(0, 1, 0)); p("line", line.dir);
  }
  return 0;
}
