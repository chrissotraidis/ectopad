# SPDX-License-Identifier: GPL-3.0-or-later
# EctoPad's GameCube memory-card library (drop-in for AxioDL's unlicensed
# kabufuda; see README.md).
cmake_minimum_required(VERSION 3.10)
project(kabufuda CXX)
add_library(kabufuda src/Card.cpp include/kabufuda/Card.hpp)
target_include_directories(kabufuda PUBLIC include)
target_compile_features(kabufuda PUBLIC cxx_std_17)

if (PROJECT_IS_TOP_LEVEL)
  add_executable(kabufuda_tests tests/card_tests.cpp)
  target_link_libraries(kabufuda_tests kabufuda)
endif ()
