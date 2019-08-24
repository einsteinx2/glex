#pragma once
#include "../common_gl.h"

#include <string>

class Wrapper {
    public:
        float rotationX;
        float rotationY;
        float rotationZ;
        Wrapper(std::string windowName);
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
        Wrapper(Wrapper const&);        // Prevent copies
        void operator=(Wrapper const&); // Prevent assignments
};