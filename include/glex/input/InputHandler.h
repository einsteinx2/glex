#pragma once
#include "glex/common/gl.h"

enum InputType {
    Keyboard
};

class InputHandler {
public:
    virtual InputType type() = 0;

#ifdef GLFW
    virtual void added(GLFWwindow *window) = 0;
    virtual void removed(GLFWwindow *window) = 0;
#else
    virtual void added() = 0;
    virtual void removed() = 0;
#endif
    virtual void poll() = 0;
};
