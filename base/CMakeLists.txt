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
    ${OGRE_PREFIX_PATH}/include/OGRE
    /usr/include/MYGUI
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    SDL2
    OgreMain
    OgreHlmsPbs
    OgreHlmsUnlit
    MyGUIEngine
)
