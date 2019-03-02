include(LibFindMacros)

libfind_pkg_detect(SDL2 sdl2
  FIND_PATH SDL.h HINTS $ENV{SDL2_HOME}/include PATH_SUFFIXES SDL2
  FIND_LIBRARY SDL2 HINTS $ENV{SDL2_HOME}/lib
)
