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

cmake_minimum_required(VERSION 3.12.0)
project(GLEX)

# If no toolchain is specified, automatically build for the host OS
if(NOT CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_SOURCE_DIR}/cmake/OSDetection.cmake)
endif()

# Setup build configurations
include(${CMAKE_SOURCE_DIR}/cmake/BuildConfigurations.cmake)

# Add the GLEX project files
add_library(GLEX STATIC 
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
    src/Application.cpp                 include/glex/Application.h
    include/glex/audio/Audio.h
    src/graphics/Cube.cpp               include/glex/graphics/Cube.h
    src/graphics/Image.cpp              include/glex/graphics/Image.h
    src/graphics/Mesh.cpp               include/glex/graphics/Mesh.h
    src/graphics/MeshLoader.cpp         include/glex/graphics/MeshLoader.h
    src/graphics/Texture.cpp            include/glex/graphics/Texture.h
    src/graphics/Triangle.cpp           include/glex/graphics/Triangle.h 
    src/graphics/Text.cpp               include/glex/graphics/Text.h 
    include/glex/input/InputHandler.h 
    src/input/GamepadState.cpp          include/glex/input/GamepadInputHandler.h
    src/input/KeyboardInputHandler.cpp  include/glex/input/KeyboardInputHandler.h
    src/input/MouseState.cpp            include/glex/input/MouseInputHandler.h 
)
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/deps/shared")

# Add platform specific files
if(USE_GLFW)
    # GLFW (i.e. Mac, Linux, Windows)
    target_sources(GLEX PRIVATE 
        src/Application_glfw.cpp
        src/input/KeyboardInputHandler_glfw.cpp
        src/input/MouseInputHandler_glfw.cpp
        src/input/GamepadInputHandler_glfw.cpp
    )
elseif(DREAMCAST_BUILD)
    # Dreamcast
    target_sources(GLEX PRIVATE 
        src/Application_dc.cpp
        src/audio/Audio_dc.cpp
        src/input/KeyboardInputHandler_dc.cpp
        src/input/MouseInputHandler_dc.cpp
        src/input/GamepadInputHandler_dc.cpp
    )
endif()

# Add correct OpenGL library
if(USE_GLFW)
    # Add the glad dependency required by glfw
    target_sources(GLEX PRIVATE deps/pc/glad_gl.cpp)

    # Build and include the glfw submodule so it's not necessary to build and install to system
    add_subdirectory("${CMAKE_SOURCE_DIR}/deps/pc/glfw")
    include_directories("${CMAKE_SOURCE_DIR}/deps/pc/glfw/include")
    include_directories("${CMAKE_SOURCE_DIR}/deps/pc/glfw/deps")
    target_link_libraries(GLEX glfw)
    add_dependencies(GLEX glfw)
elseif(USE_GLDC)
    # Build and include the GLdc submodule so it's not necessary to build and install to system
    add_custom_target(GLdc
        # Build the GLdc library
        # NOTE: We must use "make" instead of "+make" or it will fail to build
        #       though we will not get multithreaded building
        COMMAND make -C "${CMAKE_SOURCE_DIR}/deps/dc/GLdc" build
        # Move the compiled library to the build directory
        COMMAND mv "${CMAKE_SOURCE_DIR}/deps/dc/GLdc/libGLdc.a" "${CMAKE_BINARY_DIR}/libGLdc.a"
        # Clean the build artifacts
        COMMAND make -C "${CMAKE_SOURCE_DIR}/deps/dc/GLdc" clean
        VERBATIM
    )
    target_link_libraries(GLEX "${CMAKE_BINARY_DIR}/libGLdc.a" "-lm")
    add_dependencies(GLEX GLdc)

    # DreamHAL
    add_library(DreamHAL STATIC 
        deps/dc/DreamHAL/perfctr.c
    )
    include_directories("${CMAKE_SOURCE_DIR}/deps/dc/DreamHAL")
endif()

 # Dreamcast-only new audio driver test
if(DREAMCAST_BUILD)
    add_custom_target(NSound
        # Build the custom sound driver based off of the KOS driver
        # NOTE: We must use "make" instead of "+make" or it will fail to build
        #       though we will not get multithreaded building
        COMMAND make -C "${CMAKE_SOURCE_DIR}/deps/dc/nsound" build
        # Move the compiled sound driver file to the build directory
        COMMAND mv "${CMAKE_SOURCE_DIR}/deps/dc/nsound/arm/nstream.drv" "${CMAKE_BINARY_DIR}/nstream.drv"
        # Move the compiled library to the build directory
        COMMAND mv "${CMAKE_SOURCE_DIR}/deps/dc/nsound/libNSound.a" "${CMAKE_BINARY_DIR}/libNSound.a"
        # Clean the build artifacts
        COMMAND make -C "${CMAKE_SOURCE_DIR}/deps/dc/nsound" clean
    )
endif()

# GLEXAudioExample
if(DREAMCAST_BUILD)
    add_executable(GLEXAudioExample 
        examples/GLEXAudioExample/main.cpp
    )
    add_dependencies(GLEXAudioExample GLEX)
    target_link_libraries(GLEXAudioExample GLEX)
endif()

# GLEXGraphicsExample
add_executable(GLEXGraphicsExample 
    examples/GLEXGraphicsExample/main.cpp
)
add_dependencies(GLEXGraphicsExample GLEX)
target_link_libraries(GLEXGraphicsExample GLEX)
if(USE_GLDC)
    add_dependencies(GLEXGraphicsExample DreamHAL)
    target_link_libraries(GLEXGraphicsExample DreamHAL)
endif()

# GLEXInputExample
add_executable(GLEXInputExample 
    examples/GLEXInputExample/main.cpp
)
add_dependencies(GLEXInputExample GLEX)
target_link_libraries(GLEXInputExample GLEX)

# DCAudioPlayground (Dreamcast-only new audio driver test playground)
if(DREAMCAST_BUILD)
    add_executable(DCAudioPlayground
        examples/DCAudioPlayground/main.cpp 
    )
    add_dependencies(DCAudioPlayground GLEX)
    add_dependencies(DCAudioPlayground NSound)
    target_link_libraries(DCAudioPlayground GLEX)
    target_link_libraries(DCAudioPlayground "${CMAKE_BINARY_DIR}/libNSound.a")
    target_include_directories(DCAudioPlayground PRIVATE "${CMAKE_SOURCE_DIR}/deps/dc/nsound/include")
    target_include_directories(DCAudioPlayground PRIVATE "${CMAKE_SOURCE_DIR}/deps/dc/DCAudio")
endif()

# GLEXPlayground
add_executable(GLEXPlayground 
    examples/GLEXPlayground/main.cpp
)
add_dependencies(GLEXPlayground GLEX)
target_link_libraries(GLEXPlayground GLEX)
if(USE_GLDC)
    add_dependencies(GLEXPlayground DreamHAL)
    target_link_libraries(GLEXPlayground DreamHAL)
endif()

# Link macOS libraries if needed
if(OS_MAC)
    target_link_libraries(GLEXPlayground "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo")
endif()

# Final build steps (prepare assets and disc images)
if(PC_BUILD)
    # Copy meshes
    file(GLOB OBJ_MESHES "${CMAKE_SOURCE_DIR}/examples/meshes/*.obj" )
    file(COPY ${OBJ_MESHES} DESTINATION "${CMAKE_BINARY_DIR}/meshes")

    # Copy images
    file(GLOB IMG_FILES "${CMAKE_SOURCE_DIR}/examples/images/*")
    file(COPY ${IMG_FILES} DESTINATION "${CMAKE_BINARY_DIR}/images")

    # Copy samples
    file(GLOB SAMPLE_FILES "${CMAKE_SOURCE_DIR}/examples/samples/*")
    file(COPY ${SAMPLE_FILES} DESTINATION "${CMAKE_BINARY_DIR}/samples")
elseif(DREAMCAST_BUILD)
    # Build CDIs
    set(DC_EXECUTABLES # Names without .elf extension
        GLEXAudioExample
        GLEXGraphicsExample
        GLEXInputExample
        DCAudioPlayground
        GLEXPlayground
    )
    foreach(EXE ${DC_EXECUTABLES})
    get_filename_component(EXE_FILENAME ${EXE} NAME)
    add_custom_target(
        ${EXE_FILENAME}.bin ALL
        COMMAND $ENV{KOS_OBJCOPY} -R .stack -O binary -S -g ${PROJECT_BINARY_DIR}/${EXE}.elf ${PROJECT_BINARY_DIR}/${EXE}.bin
        COMMAND rm -rf ${PROJECT_BINARY_DIR}/${EXE}
        COMMAND mkdir -p ${PROJECT_BINARY_DIR}/${EXE}
        COMMAND $ENV{KOS_BASE}/../bin/scramble ${PROJECT_BINARY_DIR}/${EXE}.bin ${PROJECT_BINARY_DIR}/${EXE}/1ST_READ.BIN
        COMMAND IP_TEMPLATE_FILE=${CMAKE_SOURCE_DIR}/deps/dc/IP.TMPL $ENV{KOS_BASE}/../bin/makeip ${CMAKE_SOURCE_DIR}/deps/dc/ip.txt ${PROJECT_BINARY_DIR}/${EXE}/IP.BIN
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/examples/meshes ${CMAKE_BINARY_DIR}/${EXE}/ || true
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/examples/images ${CMAKE_BINARY_DIR}/${EXE}/ || true
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/examples/samples ${CMAKE_BINARY_DIR}/${EXE}/ || true
        # Generate CDI disc image
        COMMAND mkisofs -C 0,11702 -V ${EXE_FILENAME} -G ${PROJECT_BINARY_DIR}/${EXE}/IP.BIN -joliet -rock -l -o ${PROJECT_BINARY_DIR}/${EXE}.iso ${PROJECT_BINARY_DIR}/${EXE}
        COMMAND $ENV{KOS_BASE}/../bin/cdi4dc ${PROJECT_BINARY_DIR}/${EXE}.iso ${PROJECT_BINARY_DIR}/${EXE}.cdi
        COMMAND rm ${PROJECT_BINARY_DIR}/${EXE}.iso
        # Generate data iso for use with dcload
        COMMAND mkisofs -V ${EXE_FILENAME} -joliet -rock -o ${PROJECT_BINARY_DIR}/${EXE}_dcload.iso ${PROJECT_BINARY_DIR}/${EXE}
        DEPENDS ${EXE}
    )
    endforeach()
endif()

