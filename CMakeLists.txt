cmake_minimum_required(VERSION 3.5.1)
project(glex)

# Set output directory
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message(STATUS "Building for macOS")
    set(MACOS_BUILD true)
    set(USE_GLFW true)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/mac")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/mac")
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(${DREAMCAST_BUILD})
    message(STATUS "Building for Dreamcast")
    #set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchains/Dreamcast.cmake")
    message(STATUS "DREAMCAST_BUILD: ${DREAMCAST_BUILD}")
    SET(CMAKE_BUILD_TYPE Release)
    SET(THREADS_PREFER_PTHREAD_FLAG OFF)
    #set(USE_GLDC true)
    #add_definitions(-DUSE_GLDC)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/dc")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/dc")

    SET(OpenGL_GL_PREFERENCE "LEGACY")

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -DTIXML_USE_STL -Wdelete-non-virtual-dtor")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")
    SET(CMAKE_ASM_FLAGS "")

    # We force disable debugging information in release builds - this is so we override settings in the
    # kallistios gnu-wrappers which sometimes pick up -g through the default env vars
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -s -fomit-frame-pointer -g0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -fomit-frame-pointer -g0")

    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -Wextra -Wno-unused-parameter")
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

# Include GLdc
if(${DREAMCAST_BUILD})
    FILE(GLOB_RECURSE GLDC "${CMAKE_SOURCE_DIR}/deps/dc/GLdc/GL/*.c")
    #LIST(REMOVE_ITEM GLDC "${CMAKE_SOURCE_DIR}/deps/dc/GLdc/samples/*")
    INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/deps/dc/GLdc/include")
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
    src/textures/cubeMesh_RGBA_64x64.cpp   include/textures/cubeMesh_RGBA_64x64.h
    src/textures/cubeMesh_RGBA_128x128.cpp   include/textures/cubeMesh_RGBA_128x128.h
    src/textures/cubeMesh_RGBA_512x512.cpp   include/textures/cubeMesh_RGBA_512x512.h
    src/textures/houseMesh_RGBA_128x128.cpp  include/textures/houseMesh_RGBA_128x128.h
    src/textures/houseMesh_RGBA_256x256.cpp  include/textures/houseMesh_RGBA_256x256.h
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
include_directories("${CMAKE_SOURCE_DIR}/include")
if(${USE_GLFW})
    target_link_libraries(glex ${GLFW_STATIC_LIBRARIES})
elseif(${DREAMCAST_BUILD})
    #target_link_libraries(glex ${GLDC})
    message(STATUS "Linking pre-built libGLdc.a")
    #target_link_libraries(glex "${CMAKE_SOURCE_DIR}/deps/dc/libGLdc.a")
endif()

if(${MACOS_BUILD})
    # Create final application target
    add_executable(playground-example 
        examples/playground/main.cpp 
        examples/playground/glad_gl.cpp
    )

    # Link macOS libraries
    target_link_libraries(playground-example "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo")
elseif(${DREAMCAST_BUILD})
    # Create GLdc library target
    # add_custom_target(GLdc
    #     COMMAND make
    #     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/deps/dc/GLdc"
    # )

    # Create final application target
    add_executable(playground-example 
        examples/playground/main.cpp 
    )
    
    # Link Dreamcast libraries
    #target_link_libraries(playground-example "${CMAKE_SOURCE_DIR}/deps/dc/libGLdc.a")
    target_link_libraries(playground-example "${CMAKE_SOURCE_DIR}/deps/dc/GLdc/libGLdc.a")
    # add_dependencies(playground-example GLdc)

    # Build CDI
    SET(DC_EXECUTABLES # Names without .elf extension
        playground-example
    )
    #message(STATUS "DC_EXECUTABLES: ${DC_EXECUTABLES}")

    FOREACH(EXE ${DC_EXECUTABLES})
    get_filename_component(EXE_FILENAME ${EXE} NAME)
    ADD_CUSTOM_TARGET(
        ${EXE_FILENAME}.bin ALL
        COMMENT "EXE: ${EXE}"
        COMMENT "EXE_FILENAME: ${EXE_FILENAME}"
        COMMENT "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}"
        COMMAND $ENV{KOS_OBJCOPY} -R .stack -O binary -S -g ${EXECUTABLE_OUTPUT_PATH}/${EXE}.elf ${EXECUTABLE_OUTPUT_PATH}/${EXE}.bin
        COMMAND rm -rf ${EXECUTABLE_OUTPUT_PATH}/${EXE}
        COMMAND mkdir -p ${EXECUTABLE_OUTPUT_PATH}/${EXE}
        COMMAND $ENV{KOS_BASE}/utils/scramble/scramble ${EXECUTABLE_OUTPUT_PATH}/${EXE}.bin ${EXECUTABLE_OUTPUT_PATH}/${EXE}/1ST_READ.BIN
        COMMAND IP_TEMPLATE_FILE=${CMAKE_SOURCE_DIR}/deps/dc/IP.TMPL $ENV{KOS_BASE}/utils/makeip/makeip ${CMAKE_SOURCE_DIR}/deps/dc/ip.txt ${EXECUTABLE_OUTPUT_PATH}/${EXE}/IP.BIN
        # COMMAND mkdir -p ${CMAKE_BINARY_DIR}/${EXE}/simulant
        # COMMAND cp -r ${CMAKE_SOURCE_DIR}/assets/fonts ${CMAKE_BINARY_DIR}/${EXE}/simulant
        # COMMAND cp -r ${CMAKE_SOURCE_DIR}/assets/materials ${CMAKE_BINARY_DIR}/${EXE}/simulant
        # COMMAND cp -r ${CMAKE_SOURCE_DIR}/assets/particles ${CMAKE_BINARY_DIR}/${EXE}/simulant
        # COMMAND cp -r ${CMAKE_SOURCE_DIR}/assets/textures ${CMAKE_BINARY_DIR}/${EXE}/simulant
        # COMMAND cp -r ${CMAKE_SOURCE_DIR}/samples/data ${CMAKE_BINARY_DIR}/${EXE}/sample_data
        COMMAND mkisofs -C 0,11702 -V ${EXE_FILENAME} -G ${EXECUTABLE_OUTPUT_PATH}/${EXE}/IP.BIN -joliet -rock -l -o ${EXECUTABLE_OUTPUT_PATH}/${EXE}.iso ${EXECUTABLE_OUTPUT_PATH}/${EXE}
        COMMAND $ENV{KOS_BASE}/utils/img4dc/cdi4dc/cdi4dc ${EXECUTABLE_OUTPUT_PATH}/${EXE}.iso ${EXECUTABLE_OUTPUT_PATH}/${EXE}.cdi
        DEPENDS ${EXE}
        COMMENT "Converting ${EXE}.elf to ${EXE}.bin"
        COMMENT "mkisofs -C 0,11702 -V ${EXE_FILENAME} -G ${EXECUTABLE_OUTPUT_PATH}/${EXE}/IP.BIN -joliet -rock -l -o ${EXE}.iso ${EXECUTABLE_OUTPUT_PATH}/${EXE}"
    )
    ENDFOREACH()
endif()
target_link_libraries(playground-example glex)

#TODO: in docker container - add img4dc and makeip to kos utils, apt-get install cmake and genisoimage