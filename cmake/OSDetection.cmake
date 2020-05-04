#
## OS Detection
#

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(OS_LINUX TRUE)
    include(${CMAKE_SOURCE_DIR}/cmake/toolchains/Linux.cmake)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(OS_MAC TRUE)
    include(${CMAKE_SOURCE_DIR}/cmake/toolchains/MacOS.cmake)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(OS_WINDOWS TRUE)
    include(${CMAKE_SOURCE_DIR}/cmake/toolchains/Windows.cmake)
endif()
