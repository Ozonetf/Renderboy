cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(openGlProject)

find_package( OpenGL REQUIRED )

# add glad as library
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include)

#cmake target
add_executable(renderer main.cpp)
target_include_directories(renderer PUBLIC ${CMAKE_SOURCE_DIR}/include)

# link glfw extern 
add_subdirectory(extern/glfw)

target_link_libraries(renderer glad)
target_link_libraries(renderer OpenGL::GL)
target_link_libraries(renderer glfw)

