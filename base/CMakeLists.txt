project(base)

add_library(${PROJECT_NAME} SHARED
  Game.cc
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${OGRE_PREFIX_PATH}/include/OGRE
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    SDL2
    OgreMain
    OgreHlmsPbs
)
