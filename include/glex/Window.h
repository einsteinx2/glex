#pragma once
#include "../common_gl.h"

#include <string>

class Window {
public:
    float screenScale = 1.0; // GLFW only, to handle scaled displays (i.e. macOS Retina)
    float rotationX;
    float rotationY;
    float rotationZ;
    bool vsyncEnabled = true; // By default, lock to 60fps (or whatever refresh rate the monitor is)
    const int& width = _width;
    const int& height = _height;
    
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
    void _updateWindowSize();

    friend void _sizeCallback(GLFWwindow* window, int width, int height);
};
