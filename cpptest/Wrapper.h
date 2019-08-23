#pragma once
#include "../common_gl.h"

class Wrapper {
    public:
        Wrapper() {}; // Default constructor
        GLFWwindow* window;
        void createWindow();
        void closeWindow();
        void clear();
        void swapBuffers();
        //void reshape(int w, int h);
    private:
        Wrapper(Wrapper const&);        // prevent copies
        void operator=(Wrapper const&); // prevent assignments
};