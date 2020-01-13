#pragma once
#include "../common_gl.h"

#include <string>

#ifdef DREAMCAST
// TODO: Implement input handling for Dreamcast
void _reshapeFrustum(int width, int height);
void _reshapeOrtho(int width, int height);
void _sizeCallback(int width, int height);
#else
void key(GLFWwindow* window, int k, int s, int action, int mods);
void _reshapeFrustum(GLFWwindow* window, int width, int height);
void _reshapeOrtho(GLFWwindow* window, int width, int height);
void _sizeCallback(GLFWwindow* window, int width, int height);
#endif

class Window {
public:
    float screenScale = 1.0; // GLFW only, to handle scaled displays (i.e. macOS Retina)
    float rotationX = 1.0;
    float rotationY = 1.0;
    float rotationZ = 1.0;
    bool vsyncEnabled = true; // By default, lock to 60fps (or whatever refresh rate the monitor is)
    const int& width = _width;
    const int& height = _height;
    
    Window() {};
    virtual void createWindow(std::string windowName, int width, int height) = 0;
    virtual void closeWindow() = 0;
    void reshapeFrustum();
    void reshapeOrtho(float scale);
    void clear();
    virtual void swapBuffers() = 0;
    virtual int windowShouldClose() = 0;
    std::string windowName() { return _windowName; }

protected:
#ifndef DREAMCAST
    GLFWwindow* _window;
#endif
    std::string _windowName = "";
    int _width = 0;
    int _height = 0;

    Window(Window const&);        // Prevent copies
    void operator=(Window const&); // Prevent assignments
    void _updateWindowSize();

#ifndef DREAMCAST
    friend void _sizeCallback(GLFWwindow* window, int width, int height);
#endif
};
