cmake_minimum_required(VERSION 3.5)

project(ogre3d-examples)

set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

find_package(SDL2 REQUIRED QUIET)
find_package(OGRE REQUIRED QUIET)
find_package(MyGUI REQUIRED QUIET)

add_subdirectory(base)
add_subdirectory(cube)
add_subdirectory(gui)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg.sample ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg)
