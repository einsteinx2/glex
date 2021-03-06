#
## Common Build Settings
#

# Ensure CMAKE_SYSTEM_NAME is set
if(NOT DEFINED CMAKE_SYSTEM_NAME)
    set(CMAKE_SYSTEM_NAME Generic)
endif()

# Set C and C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

# Disable optimizations when building Debug version
# Set C and C++ compiler flags for this platform
# NOTE: `CACHE STRING "" FORCE` must be used when inside toolchain file
#        See this SO answer for more info: https://stackoverflow.com/a/30217088/299262
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(OPTIMIZATION_LEVEL "/Od")
else()
    set(OPTIMIZATION_LEVEL "-O0")
endif()
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${OPTIMIZATION_LEVEL}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OPTIMIZATION_LEVEL}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${OPTIMIZATION_LEVEL} -DNDEBUG -DRELEASE" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPTIMIZATION_LEVEL} -DNDEBUG -DRELEASE" CACHE STRING "" FORCE)
