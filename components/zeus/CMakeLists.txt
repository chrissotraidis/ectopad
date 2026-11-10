# SPDX-License-Identifier: GPL-3.0-or-later
# EctoPad's math library: a drop-in for AxioDL's unlicensed zeus (see README.md).
cmake_minimum_required(VERSION 3.21)
project(zeus CXX)

add_library(zeus
    src/CAABox.cpp
    src/CColor.cpp
    src/CEulerAngles.cpp
    src/CFrustum.cpp
    src/CMatrix3f.cpp
    src/CMatrix4f.cpp
    src/COBBox.cpp
    src/CPlane.cpp
    src/CProjection.cpp
    src/CQuaternion.cpp
    src/CTransform.cpp
    src/CVector2f.cpp
    src/CVector3f.cpp
    src/Math.cpp)
target_include_directories(zeus PUBLIC include)
# Keep products and sums separately rounded, like the original's SIMD code.
target_compile_options(zeus PRIVATE -ffp-contract=off)
target_compile_features(zeus PUBLIC cxx_std_20)

if (PROJECT_IS_TOP_LEVEL)
  add_executable(zeus_probe tests/zeus_probe.cpp)
  target_link_libraries(zeus_probe zeus)
endif ()

