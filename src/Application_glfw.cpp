#ifdef GLFW

#include "glex/Application.h"
#include "glex/common/gl.h"
#include "glex/common/log.h"

#include <unistd.h>
#include <cstdlib>

void Application::createWindow(std::string windowName, int width, int height) {
    _windowName = windowName;
    _windowWidth = width;
    _windowHeight = height;

    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    _window = glfwCreateWindow(width, height, _windowName.c_str(), NULL, NULL);
    if (!_window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, _sizeCallback);

    // Lock to (probably) 60fps if vsyncEnabled, or unlock framerate
    glfwMakeContextCurrent(_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(vsyncEnabled ? 1 : 0);

    // Setup the frame buffer and view port size
    _reshapeFrustum(width, height);
    //_reshapeOrtho(width, height); // Don't seem to need to set ortho size here...

    // Detect screen scale by comparing window size to framebuffer size
    int frameBufferWidth;
    glfwGetFramebufferSize(_window, &frameBufferWidth, NULL);
    screenScale = frameBufferWidth / width;
    DEBUG_PRINTLN("screen scale detected: %f", screenScale);
}

void Application::closeWindow() {
    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
}

void Application::swapBuffers() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

int Application::windowShouldClose() {
    return glfwWindowShouldClose(_window);
}

void Application::_updateWindowSize() {
    glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);
}

void _sizeCallback(GLFWwindow* window, int width, int height) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    app->_updateWindowSize();
    app->_reshapeFrustum(width, height);
    app->_reshapeOrtho(width, height);
}

void Application::addInputHandler(InputHandler* inputHandler) {
    _inputHandlers.push_back(inputHandler);
    inputHandler->added(_window);
}

void Application::removeInputHandler(InputHandler* inputHandler) {
    auto first = std::remove(_inputHandlers.begin(), _inputHandlers.end(), inputHandler);
    _inputHandlers.erase(first, _inputHandlers.end());
    inputHandler->removed(_window);
}

#endif