project(cube)

add_executable(${PROJECT_NAME}
  main.cc
  Cube.cc
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    base
)
