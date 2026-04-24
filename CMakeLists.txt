cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(openGlProject)

find_package( OpenGL REQUIRED )

# add glad as library
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include)

#cmake target
add_executable(renderer main.cpp)
target_include_directories(renderer PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
# link glfw extern 
add_subdirectory(extern/glfw)

target_link_libraries(renderer glad)
target_link_libraries(renderer OpenGL::GL)
target_link_libraries(renderer glfw)

#copys files in the asset folder to the output(build) folder
add_custom_command(TARGET renderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:renderer>/assets
)
