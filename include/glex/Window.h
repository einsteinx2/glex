#pragma once
#include "../common_gl.h"

#include <string>

class Window {
public:
    float rotationX;
    float rotationY;
    float rotationZ;
    bool vsyncEnabled = true; // By default, lock to 60fps (or whatever refresh rate the monitor is)
    Window(){};
    void createWindow(std::string windowName, int width, int height);
    void closeWindow();
    void reshapeFrustum();
    void reshapeOrtho(float scale);
    void clear();
    void swapBuffers();
    int windowShouldClose();
    std::string windowName() { return _windowName; }
private:
    #ifndef DREAMCAST
        GLFWwindow* _window;
    #endif
    std::string _windowName;
    int _width;
    int _height;
    Window(Window const&);        // Prevent copies
    void operator=(Window const&); // Prevent assignments
};
