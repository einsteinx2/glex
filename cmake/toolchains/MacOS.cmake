#
## MacOS Specific Build Settings
#

include(${CMAKE_CURRENT_LIST_DIR}/Common.cmake)

# Let the main script know we're building for MacOS
set(MACOS_BUILD true)

# Use the glfw OpenGL library
set(USE_GLFW true)

# Add compiler flags shared by all languages
add_definitions(
    -DGLFW
    -DMACOS
)
