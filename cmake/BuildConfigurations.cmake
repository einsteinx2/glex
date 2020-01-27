#
## Setup Default Build Configuration
#

# NOTE: Must be included in the main CMakeLists.txt file NOT in 
#       the toolchain files or CMAKE_BUILD_TYPE isn't always set.

# Info from https://stackoverflow.com/questions/31546278/where-to-set-cmake-configuration-types-in-a-project-with-subprojects
if(NOT SET_UP_CONFIGURATIONS_DONE)
    set(SET_UP_CONFIGURATIONS_DONE TRUE)

    # No reason to set CMAKE_CONFIGURATION_TYPES if it's not a multiconfig generator
    # Also no reason mess with CMAKE_BUILD_TYPE if it's a multiconfig generator.
    # New way of checking for multi config from cmake maintainer: 
    # https://gitlab.kitware.com/cmake/cmake/issues/17645#note_367457
    get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(isMultiConfig)
        message(STATUS "This is a multi config (i.e. GUI tool) build")
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE) 
    else()
        message(STATUS "This is a single config (i.e. command line tool) build")
        if(NOT CMAKE_BUILD_TYPE)
            message(STATUS "No built type set, so defaulting to Debug build")
            set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
        endif()
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY HELPSTRING "Choose the type of build")
        # set the valid options for cmake-gui drop-down list
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release")
    endif()
    
endif()