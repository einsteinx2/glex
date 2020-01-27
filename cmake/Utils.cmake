#
## Utility Functions
#

# Dumps all cmake variables for inspection
function(print_all_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        message("${_variableName}=${${_variableName}}")
    endforeach()
endfunction(print_all_cmake_variables)
