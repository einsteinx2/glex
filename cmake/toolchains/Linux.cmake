#
## Linux Specific Build Settings
#

include(${CMAKE_CURRENT_LIST_DIR}/Common.cmake)

# Let the main script know we're building for PC
set(PC_BUILD TRUE)

# Use the glfw OpenGL library
set(USE_GLFW TRUE)

# Add compiler flags shared by all languages
add_definitions(
    -DGLFW
    -DLINUX
)
