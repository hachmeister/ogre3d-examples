cmake_minimum_required(VERSION 3.5)

project(ogre3d-examples)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OGRE_PREFIX_PATH "" CACHE PATH "Ogre3D prefix path.")

link_directories(
  ${OGRE_PREFIX_PATH}/lib
)

add_subdirectory(base)
add_subdirectory(cube)
