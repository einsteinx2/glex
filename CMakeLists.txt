# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                                   #
# NOTES:                                                                            #
# ======                                                                            #
#                                                                                   #
# GCC build flags:                                                                  #
# ----------------                                                                  #
# CMAKE_C_FLAGS and CMAKE_CXX_FLAGS are added to all builds, while                  #
# CMAKE_C_FLAGS_DEBUG and CMAKE_CXX_FLAGS_DEBUG are added to Debug                  #
# builds and CMAKE_C_FLAGS_RELEASE and CMAKE_CXX_FLAGS_RELEASE are                  #
# added to Release builds, etc (the last word is just the build type                #
# in all caps, and there can be custom build types: see Common.cmake)               #
#                                                                                   #
# GCC optimization levels:                                                          #
# ------------------------                                                          #
# For complete information, see https://linux.die.net/man/1/gcc and                 #
# search for "Options That Control Optimization" to find the section.               #
# A shorter overview can be found here: https://stackoverflow.com/a/1778700/299262  #
#                                                                                   #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#variable_watch(CMAKE_BUILD_TYPE)

cmake_minimum_required(VERSION 3.5.1)
project(glex)

if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Must set a toolchain file by setting -DCMAKE_TOOLCHAIN_FILE when running cmake")
endif()

# Setup build configurations
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BuildConfigurations.cmake)

# Add the glex project files
add_library(glex STATIC 
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
    src/Application.cpp      include/glex/Application.h
    src/Cube.cpp             include/glex/Cube.h
    src/Image.cpp            include/glex/Image.h
    src/Mesh.cpp             include/glex/Mesh.h
    src/MeshLoader.cpp       include/glex/MeshLoader.h
    src/Texture.cpp          include/glex/Texture.h
    src/Triangle.cpp         include/glex/Triangle.h 
    src/Text.cpp             include/glex/Text.h 
    include/glex/input/InputHandler.h
    src/input/KeyboardInputHandler.cpp include/glex/input/KeyboardInputHandler.h
)
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/deps/shared")

# Add platform specific files
if(USE_GLFW)
    # GLFW (i.e. Mac, Linux, Windows)
    target_sources(glex PRIVATE 
        src/Application_glfw.cpp
        src/input/KeyboardInputHandler_glfw.cpp
    )
elseif(DREAMCAST_BUILD)
    # Dreamcast
    target_sources(glex PRIVATE 
        src/Application_dc.cpp
        src/input/KeyboardInputHandler_dc.cpp
    )
endif()

# Add correct OpenGL library
if(USE_GLFW)
    # Add the glad dependency required by glfw
    target_sources(glex PRIVATE deps/pc/glad_gl.cpp)

    # Build and include the glfw submodule so it's not necessary to build and install to system
    add_subdirectory("${CMAKE_SOURCE_DIR}/deps/pc/glfw")
    include_directories("${CMAKE_SOURCE_DIR}/deps/pc/glfw/include")
    include_directories("${CMAKE_SOURCE_DIR}/deps/pc/glfw/deps")
    target_link_libraries(glex glfw)
    add_dependencies(glex glfw)
elseif(USE_GLDC)
    # Build and include the GLdc submodule so it's not necessary to build and install to system
    add_custom_target(GLdc
        # NOTE: Instead of just calling `make build` and setting WORKING_DIRECTORY
        #       we must call make in a single command so that we can add the + to
        #       the beginning so that the -j flag passed into the top level `make` works
        COMMAND +make -C "${CMAKE_SOURCE_DIR}/deps/dc/GLdc" build
    )
    target_link_libraries(glex "${CMAKE_SOURCE_DIR}/deps/dc/GLdc/libGLdc.a" "-lm")
    add_dependencies(glex GLdc)
endif()

# Create and build example programs
add_executable(playground-example 
    examples/playground/main.cpp 
)
target_link_libraries(playground-example glex)

if(MACOS_BUILD)
    # Copy meshes
    file(GLOB OBJ_MESHES "${CMAKE_SOURCE_DIR}/examples/playground/meshes/*.obj" )
    file(COPY ${OBJ_MESHES} DESTINATION "${CMAKE_BINARY_DIR}/meshes")

    # Copy images
    file(GLOB IMG_FILES "${CMAKE_SOURCE_DIR}/examples/playground/images/*")
    file(COPY ${IMG_FILES} DESTINATION "${CMAKE_BINARY_DIR}/images")

    # Link macOS libraries
    target_link_libraries(playground-example "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo")
elseif(DREAMCAST_BUILD)
    # Dreamcast-only program
    add_executable(audiotest-example
        examples/audiotest/main.cpp 
    )
    target_link_libraries(audiotest-example glex)

    # Build CDIs
    set(DC_EXECUTABLES # Names without .elf extension
        playground-example
        audiotest-example
    )
    #message(STATUS "DC_EXECUTABLES: ${DC_EXECUTABLES}")

    foreach(EXE ${DC_EXECUTABLES})
    get_filename_component(EXE_FILENAME ${EXE} NAME)
    add_custom_target(
        ${EXE_FILENAME}.bin ALL
        COMMAND $ENV{KOS_OBJCOPY} -R .stack -O binary -S -g ${PROJECT_BINARY_DIR}/${EXE}.elf ${PROJECT_BINARY_DIR}/${EXE}.bin
        COMMAND rm -rf ${PROJECT_BINARY_DIR}/${EXE}
        COMMAND mkdir -p ${PROJECT_BINARY_DIR}/${EXE}
        COMMAND $ENV{KOS_BASE}/utils/scramble/scramble ${PROJECT_BINARY_DIR}/${EXE}.bin ${PROJECT_BINARY_DIR}/${EXE}/1ST_READ.BIN
        COMMAND IP_TEMPLATE_FILE=${CMAKE_SOURCE_DIR}/deps/dc/IP.TMPL $ENV{KOS_BASE}/utils/makeip/makeip ${CMAKE_SOURCE_DIR}/deps/dc/ip.txt ${PROJECT_BINARY_DIR}/${EXE}/IP.BIN
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/examples/playground/meshes ${CMAKE_BINARY_DIR}/${EXE}/
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/examples/playground/images ${CMAKE_BINARY_DIR}/${EXE}/
        # Generate CDI disc image
        COMMAND mkisofs -C 0,11702 -V ${EXE_FILENAME} -G ${PROJECT_BINARY_DIR}/${EXE}/IP.BIN -joliet -rock -l -o ${PROJECT_BINARY_DIR}/${EXE}.iso ${PROJECT_BINARY_DIR}/${EXE}
        COMMAND $ENV{KOS_BASE}/utils/img4dc/cdi4dc/cdi4dc ${PROJECT_BINARY_DIR}/${EXE}.iso ${PROJECT_BINARY_DIR}/${EXE}.cdi
        COMMAND rm ${PROJECT_BINARY_DIR}/${EXE}.iso
        # Generate data iso for use with dcload
        COMMAND mkisofs -V ${EXE_FILENAME} -joliet -rock -o ${PROJECT_BINARY_DIR}/${EXE}_dcload.iso ${PROJECT_BINARY_DIR}/${EXE}
        DEPENDS ${EXE}
    )
    endforeach()
endif()

