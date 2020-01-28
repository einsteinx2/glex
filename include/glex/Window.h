#pragma once
#include "glex/common/gl.h"

#include <string>

#if GLFW
//void key(GLFWwindow* window, int k, int s, int action, int mods);
void _sizeCallback(GLFWwindow* window, int width, int height);
#endif

class Window {
public:
    float screenScale = 1.0; // GLFW only, to handle scaled displays (i.e. macOS Retina)
    bool vsyncEnabled = true; // By default, lock to 60fps (or whatever refresh rate the monitor is)
    const int& width = _width;
    const int& height = _height;
    
    Window() {};
    void createWindow(std::string windowName, int width, int height);
    void closeWindow();
    void reshapeFrustum();
    void reshapeOrtho(float scale);
    void clear();
    void swapBuffers();
    int windowShouldClose();
    std::string windowName() { return _windowName; }

private:
#ifdef GLFW
    GLFWwindow* _window;
#endif
    std::string _windowName = "";
    int _width = 0;
    int _height = 0;

    Window(Window const&);         // Prevent copies
    void operator=(Window const&); // Prevent assignments
    void _updateWindowSize();
    void _reshapeFrustum(int width, int height);
    void _reshapeOrtho(int width, int height);

#ifdef GLFW
    friend void _sizeCallback(GLFWwindow* window, int width, int height);
#endif
};
