#
## MacOS Specific Build Settings
#

include(${CMAKE_CURRENT_LIST_DIR}/Common.cmake)

# Let the main script know we're building for Dreamcast
set(MACOS_BUILD true)

# Use the glfw OpenGL library
set(USE_GLFW true)
