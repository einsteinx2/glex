cmake_minimum_required(VERSION 3.13)
project(cpptest)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)
set (LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

# Include GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

# Add the project files
add_library(myLib "")
target_link_libraries(myLib ${GLFW_STATIC_LIBRARIES})
add_subdirectory(src/fonts)
add_subdirectory(src/meshes)
add_subdirectory(src/textures)
add_subdirectory(src/library)

add_executable(cpptest "src/main.cpp" "src/glad_gl.cpp" "src/common_gl.h")
target_link_libraries(cpptest "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo")
target_link_libraries(cpptest myLib)
