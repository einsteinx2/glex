#pragma once
#include "../common_gl.h"

class Wrapper {
    public:
        Wrapper() {}; // Default constructor
        GLFWwindow* window;
        void createWindow();
        void closeWindow();
        void reshapeFrustum();
        void reshapeOrtho(float scale);
        void clear();
        void swapBuffers();
    private:
        int _width;
        int _height;
        Wrapper(Wrapper const&);        // prevent copies
        void operator=(Wrapper const&); // prevent assignments
};