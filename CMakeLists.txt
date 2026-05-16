cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# force GLM build options
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)

project(openGlProject)

find_package( OpenGL REQUIRED )

# add glad as library
add_library(glad STATIC extern/vendor/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/extern/vendor)

# add ufbx as library
add_library(ufbx STATIC extern/vendor/ufbx.c)
target_include_directories(ufbx PUBLIC ${CMAKE_SOURCE_DIR}/extern/vendor)

#cmake target
add_executable(Renderboy main.cpp)
target_include_directories(Renderboy PUBLIC ${CMAKE_SOURCE_DIR}/include)

# include all vendor headers 
target_include_directories(Renderboy PUBLIC ${CMAKE_SOURCE_DIR}/extern/vendor)
add_subdirectory(src)

# link glfw extern 
add_subdirectory(extern/glfw)
# link GLM
find_package(glm QUIET)
if(NOT glm_FOUND)
    add_subdirectory(external/glm)
endif()

target_link_libraries(Renderboy glad)
target_link_libraries(Renderboy ufbx)
target_link_libraries(Renderboy glfw)
target_link_libraries(Renderboy glm)
target_link_libraries(Renderboy OpenGL::GL)

#copys files in the asset folder to the output(build) folder
add_custom_command(TARGET Renderboy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:Renderboy>/assets
)
