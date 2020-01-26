cmake_minimum_required(VERSION 3.5.1)
project(glex)

# Disable optimizations when building debug version
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_C_FLAGS_DEBUG "-O0")

# Set C and C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message(STATUS "Building for macOS")
    set(MACOS_BUILD true)
    set(USE_GLFW true)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/mac")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/mac")
elseif(${DREAMCAST_BUILD})
    message(STATUS "Building for Dreamcast")
    # Currently adding the toolchain file doesn't work from here because it's the toolchain file that sets DREAMCAST_BUILD,
    # so instead it's set in the build script
    #set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchains/Dreamcast.cmake")
    message(STATUS "DREAMCAST_BUILD: ${DREAMCAST_BUILD}")
    SET(CMAKE_BUILD_TYPE Release)
    #SET(CMAKE_BUILD_TYPE Debug)
    SET(THREADS_PREFER_PTHREAD_FLAG OFF)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/dc")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/dc")
    SET(OpenGL_GL_PREFERENCE "LEGACY")

    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -DTIXML_USE_STL -Wdelete-non-virtual-dtor")
    #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DTIXML_USE_STL -Wdelete-non-virtual-dtor")
    #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
    SET(CMAKE_ASM_FLAGS "")

    # We force disable debugging information in release builds - this is so we override settings in the
    # kallistios gnu-wrappers which sometimes pick up -g through the default env vars
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -s -fomit-frame-pointer -g0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -fomit-frame-pointer -g0")
    #set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -s -g0")
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -g0")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -Wextra -Wno-unused-parameter")
else()
    message(FATAL_ERROR "Your build system '${CMAKE_SYSTEM_NAME}' is not supported")
endif()

# Add the project files
set(glex_SOURCES
    # Common
    include/glex/common/font.h
    include/glex/common/gl.h
    include/glex/common/log.h
    include/glex/common/mesh.h
    include/glex/common/path.h
    deps/shared/stb/stb_image.h

    # Fonts
    src/fonts/arial_16pt.cpp  include/glex/fonts/arial_16pt.h
    src/fonts/arial_28pt.cpp  include/glex/fonts/arial_28pt.h
    src/fonts/arial_32pt.cpp  include/glex/fonts/arial_32pt.h 

    # GLEX
    src/Cube.cpp             include/glex/Cube.h
    src/Image.cpp            include/glex/Image.h
    src/Mesh.cpp             include/glex/Mesh.h
    src/MeshLoader.cpp       include/glex/MeshLoader.h
    src/Texture.cpp          include/glex/Texture.h
    src/Triangle.cpp         include/glex/Triangle.h 
    src/Text.cpp             include/glex/Text.h
    src/Window.cpp           include/glex/Window.h
    src/DreamcastWindow.cpp  include/glex/DreamcastWindow.h
    src/PCWindow.cpp         include/glex/PCWindow.h
)
if(${USE_GLFW})
     # Add the glad file
     set(glex_SOURCES ${glex_SOURCES} deps/pc/glad_gl.cpp)
endif()
add_library(glex STATIC ${glex_SOURCES})
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/deps/shared")

# Add corrent OpenGL library
if(${USE_GLFW})
    # Build and include glfw submodule so it's not necessary to build and install to system
    add_subdirectory("${CMAKE_SOURCE_DIR}/deps/pc/glfw")
    include_directories("${CMAKE_SOURCE_DIR}/deps/pc/glfw/include")
    include_directories("${CMAKE_SOURCE_DIR}/deps/pc/glfw/deps")
    target_link_libraries(glex glfw)
elseif(${DREAMCAST_BUILD})
    # Example to use pre-compiled GLdc library instead of compiling the submodule
    #target_link_libraries(glex ${GLDC})
    #message(STATUS "Linking pre-built libGLdc.a")
    #target_link_libraries(glex "${CMAKE_SOURCE_DIR}/deps/dc/libGLdc.a")

    # Build and include GLdc submodule so it's not necessary to build and install to system
    file(GLOB_RECURSE GLDC "${CMAKE_SOURCE_DIR}/deps/dc/GLdc/GL/*.c")
    list(REMOVE_ITEM GLDC "${CMAKE_SOURCE_DIR}/deps/dc/GLdc/samples/*")
    include_directories("${CMAKE_SOURCE_DIR}/deps/dc/GLdc/include")

    # Create GLdc library target
    add_custom_target(GLdc
        COMMAND make build
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/deps/dc/GLdc"
    )
    target_link_libraries(glex "${CMAKE_SOURCE_DIR}/deps/dc/GLdc/libGLdc.a" "-lm")
    add_dependencies(glex GLdc)
endif()

if(${MACOS_BUILD})
    # Create final application target
    add_executable(playground-example 
        examples/playground/main.cpp 
    )

    # Copy meshes
    file(GLOB OBJ_MESHES "${CMAKE_SOURCE_DIR}/examples/playground/meshes/*.obj" )
    file(COPY ${OBJ_MESHES} DESTINATION "${CMAKE_BINARY_DIR}/meshes")

    # Copy images
    file(GLOB IMG_FILES "${CMAKE_SOURCE_DIR}/examples/playground/images/*")
    file(COPY ${IMG_FILES} DESTINATION "${CMAKE_BINARY_DIR}/images")

    # Link macOS libraries
    target_link_libraries(playground-example "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo")
elseif(${DREAMCAST_BUILD})
    # Create final application target
    add_executable(playground-example 
        examples/playground/main.cpp 
    )

    # Build CDI
    set(DC_EXECUTABLES # Names without .elf extension
        playground-example
    )
    #message(STATUS "DC_EXECUTABLES: ${DC_EXECUTABLES}")

    foreach(EXE ${DC_EXECUTABLES})
    get_filename_component(EXE_FILENAME ${EXE} NAME)
    add_custom_target(
        ${EXE_FILENAME}.bin ALL
        COMMENT "EXE: ${EXE}"
        COMMENT "EXE_FILENAME: ${EXE_FILENAME}"
        COMMENT "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}"
        COMMAND $ENV{KOS_OBJCOPY} -R .stack -O binary -S -g ${EXECUTABLE_OUTPUT_PATH}/${EXE}.elf ${EXECUTABLE_OUTPUT_PATH}/${EXE}.bin
        COMMAND rm -rf ${EXECUTABLE_OUTPUT_PATH}/${EXE}
        COMMAND mkdir -p ${EXECUTABLE_OUTPUT_PATH}/${EXE}
        COMMAND $ENV{KOS_BASE}/utils/scramble/scramble ${EXECUTABLE_OUTPUT_PATH}/${EXE}.bin ${EXECUTABLE_OUTPUT_PATH}/${EXE}/1ST_READ.BIN
        COMMAND IP_TEMPLATE_FILE=${CMAKE_SOURCE_DIR}/deps/dc/IP.TMPL $ENV{KOS_BASE}/utils/makeip/makeip ${CMAKE_SOURCE_DIR}/deps/dc/ip.txt ${EXECUTABLE_OUTPUT_PATH}/${EXE}/IP.BIN
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/examples/playground/meshes ${CMAKE_BINARY_DIR}/${EXE}/
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/examples/playground/images ${CMAKE_BINARY_DIR}/${EXE}/
        # Generate CDI disc image
        COMMAND mkisofs -C 0,11702 -V ${EXE_FILENAME} -G ${EXECUTABLE_OUTPUT_PATH}/${EXE}/IP.BIN -joliet -rock -l -o ${EXECUTABLE_OUTPUT_PATH}/${EXE}.iso ${EXECUTABLE_OUTPUT_PATH}/${EXE}
        COMMAND $ENV{KOS_BASE}/utils/img4dc/cdi4dc/cdi4dc ${EXECUTABLE_OUTPUT_PATH}/${EXE}.iso ${EXECUTABLE_OUTPUT_PATH}/${EXE}.cdi
        COMMAND rm ${EXECUTABLE_OUTPUT_PATH}/${EXE}.iso
        # Generate data iso for use with dcload
        COMMAND mkisofs -V ${EXE_FILENAME} -joliet -rock -o ${EXECUTABLE_OUTPUT_PATH}/${EXE}_dcload.iso ${EXECUTABLE_OUTPUT_PATH}/${EXE}
        DEPENDS ${EXE}
    )
    endforeach()
endif()
target_link_libraries(playground-example glex)

#TODO: in docker container - add img4dc and makeip to kos utils, apt-get install cmake and genisoimage
