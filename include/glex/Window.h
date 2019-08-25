#pragma once
#include "../common_gl.h"

#include <string>

class Window {
    public:
        float rotationX;
        float rotationY;
        float rotationZ;
        Window(std::string windowName);
        bool vsyncEnabled = true; // By default, lock to 60fps (or whatever refresh rate the monitor is)
        GLFWwindow* window;
        void createWindow();
        void closeWindow();
        void reshapeFrustum();
        void reshapeOrtho(float scale);
        void clear();
        void swapBuffers();
        std::string windowName() { return _windowName; }
    private:
        std::string _windowName;
        int _width;
        int _height;
        Window(Window const&);        // Prevent copies
        void operator=(Window const&); // Prevent assignments
};
