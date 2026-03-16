cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(openGlProject)

find_package( OpenGL REQUIRED )

add_executable(renderer main.cpp)

add_subdirectory(extern/glfw)
# add_subdirectory(src)

add_library(glad)
target_sources(glad
  PRIVATE 
    src/glad.c
  
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/glad/glad.h
    )

# target_include_directories(renderer PRIVATE include)

target_link_libraries(renderer glad)
target_link_libraries(renderer OpenGL::GL)
target_link_libraries(renderer glfw)
