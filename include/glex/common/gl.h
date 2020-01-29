#pragma once

#ifdef DREAMCAST
    #include "../deps/dc/GLdc/include/gl.h"
    //#include "../deps/dc/GLdc/include/glu.h"
    #include "../deps/dc/GLdc/include/glkos.h"
    #include "../deps/dc/GLdc/include/glext.h"
#else
    #include <glad/gl.h>
    #define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>
#endif
