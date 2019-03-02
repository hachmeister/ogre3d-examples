include(LibFindMacros)

libfind_pkg_detect(OGRE OGRE
  FIND_PATH Ogre.h HINTS $ENV{OGRE_HOME}/include PATH_SUFFIXES OGRE
  FIND_LIBRARY OgreMain HINTS $ENV{OGRE_HOME}/lib
)

libfind_pkg_detect(OGRE_PBS OGRE
  FIND_LIBRARY OgreHlmsPbs HINTS $ENV{OGRE_HOME}/lib
)

libfind_pkg_detect(OGRE_UNLIT OGRE
  FIND_LIBRARY OgreHlmsUnlit HINTS $ENV{OGRE_HOME}/lib
)

find_path(OGRE_PLUGIN_DIR
  NAMES RenderSystem_GL3Plus.so HINTS $ENV{OGRE_HOME}/lib PATH_SUFFIXES OGRE
)
