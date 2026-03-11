cmake_minimum_required(VERSION 3.23)

project(openGlProject)

find_package( OpenGL REQUIRED )

add_subdirectory(extern/glfw)


add_executable(renderer)

target_link_libraries(renderer OpenGL::GL)
target_link_libraries(renderer glfw)


target_sources(renderer
  PRIVATE
    main.cpp
)