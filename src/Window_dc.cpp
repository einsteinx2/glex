#ifdef DREAMCAST

#include "glex/Window.h"
#include "glex/common/gl.h"
#include "glex/common/log.h"

void Window::createWindow(std::string windowName, int width, int height) {
    _windowName = windowName;
    _width = width;
    _height = height;

    glKosInit();
    _reshapeFrustum(_width, _height);
}

void Window::closeWindow() {
    // Not implemented for Dreamcast
}

void Window::swapBuffers() {
    glKosSwapBuffers();
}

int Window::windowShouldClose() {
    return 0;
}

void Window::_updateWindowSize() {
    // Not implemented for Dreamcast
}

#endif