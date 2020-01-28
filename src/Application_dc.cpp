#ifdef DREAMCAST

#include "glex/Application.h"
#include "glex/common/gl.h"
#include "glex/common/log.h"

void Application::createWindow(std::string windowName, int width, int height) {
    _windowName = windowName;
    _windowWidth = width;
    _windowHeight = height;

    glKosInit();
    _reshapeFrustum(width, height);
}

void Application::closeWindow() {
    // Not implemented for Dreamcast
}

void Application::swapBuffers() {
    glKosSwapBuffers();
}

int Application::windowShouldClose() {
    return 0;
}

void Application::_updateWindowSize() {
    // Not implemented for Dreamcast
}

#endif