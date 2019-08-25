cmake_minimum_required(VERSION 3.13)
project(glex)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message(STATUS "Building for macOS")
    set(USE_GLFW true)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/mac")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/mac")
elseif(ENV{KOS_ARCH} STREQUAL "dreamcast")
    message(FATAL_ERROR "Building for Dreamcast")
    set(USE_GLDC true)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/dc")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/dc")
else()
    message(FATAL_ERROR "Your build system '${CMAKE_SYSTEM_NAME}' is not supported")
endif()

# Include GLFW
if(${USE_GLFW})
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})
endif()

# Add the project files
add_library(glex STATIC
    # Common
    include/common_gl.h
    include/debug_log.h

    # Fonts
    include/fonts/common_font.h
    src/fonts/arial_16pt.cpp  include/fonts/arial_16pt.h
    src/fonts/arial_28pt.cpp  include/fonts/arial_28pt.h
    src/fonts/arial_32pt.cpp  include/fonts/arial_32pt.h 
    
    # Meshes
    include/meshes/common_mesh.h  
    src/meshes/cubeMesh.cpp    include/meshes/cubeMesh.h
    src/meshes/houseMesh.cpp   include/meshes/houseMesh.h
    src/meshes/monkeyMesh.cpp  include/meshes/monkeyMesh.h

    # Textures
    src/textures/cubeMesh_RGBA_512x512.cpp   include/textures/cubeMesh_RGBA_512x512.h
    src/textures/houseMesh_RGBA_512x512.cpp  include/textures/houseMesh_RGBA_512x512.h

    # GLEX
    src/glex/Cube.cpp      include/glex/Cube.h
    src/glex/Font.cpp      include/glex/Font.h
    src/glex/Mesh.cpp      include/glex/Mesh.h
    src/glex/Sphere.cpp    include/glex/Sphere.h
    src/glex/Texture.cpp   include/glex/Texture.h
    src/glex/Triangle.cpp  include/glex/Triangle.h  
    src/glex/Window.cpp    include/glex/Window.h
)
include_directories("${PROJECT_SOURCE_DIR}/include")
if(${USE_GLFW})
target_link_libraries(glex ${GLFW_STATIC_LIBRARIES})
endif()

add_executable(playground-example 
    examples/playground/main.cpp 
    examples/playground/glad_gl.cpp
)
target_link_libraries(playground-example glex)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
target_link_libraries(playground-example "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo")
endif()