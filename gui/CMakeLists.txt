project(gui)

add_executable(${PROJECT_NAME}
  main.cc
  Gui.cc
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    base
)
