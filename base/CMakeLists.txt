project(base)

add_library(${PROJECT_NAME} SHARED
  Game.cc
  mygui/MyGUI_Ogre2DataManager.cpp
  mygui/MyGUI_Ogre2DataStream.cpp
  mygui/MyGUI_Ogre2GuiRenderable.cpp
  mygui/MyGUI_Ogre2RenderManager.cpp
  mygui/MyGUI_Ogre2RTTexture.cpp
  mygui/MyGUI_Ogre2Texture.cpp
  mygui/MyGUI_Ogre2VertexBuffer.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${OGRE_INCLUDE_DIRS}
    ${MyGUI_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${SDL2_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${MyGUI_LIBRARIES}
)
