#ifndef DREAMCAST

#include "glex/PCWindow.h"
#include "glex/common/gl.h"
#include "glex/common/log.h"

void PCWindow::createWindow(std::string windowName, int width, int height) {
    _windowName = windowName;
    _width = width;
    _height = height;

    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    _window = glfwCreateWindow(_width, _height, _windowName.c_str(), NULL, NULL);
    if (!_window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, _sizeCallback);
    //glfwSetKeyCallback(_window, key);

    // Lock to (probably) 60fps if vsyncEnabled, or unlock framerate
    glfwMakeContextCurrent(_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(vsyncEnabled ? 1 : 0);

    // Setup the frame buffer and view port size
    _reshapeFrustum(_window, _width, _height);
    //_reshapeOrtho(_window, _width, _height);

    // Detect screen scale by comparing window size to framebuffer size
    int frameBufferWidth;
    glfwGetFramebufferSize(_window, &frameBufferWidth, NULL);
    screenScale = frameBufferWidth / _width;
    DEBUG_PRINTLN("screen scale detected: %f", screenScale);
}

void PCWindow::closeWindow() {
    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
}

void PCWindow::swapBuffers() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

int PCWindow::windowShouldClose() {
    return glfwWindowShouldClose(_window);
}

#endif