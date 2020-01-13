#ifdef DREAMCAST

#include "glex/DreamcastWindow.h"
#include "common_gl.h"
#include "debug_log.h"

void DreamcastWindow::createWindow(std::string windowName, int width, int height) {
    _windowName = windowName;
    _width = width;
    _height = height;

    glKosInit();
    _reshapeFrustum(_width, _height);
}

void DreamcastWindow::closeWindow() {
    // Not implemented for Dreamcast
}

void DreamcastWindow::swapBuffers() {
    glKosSwapBuffers();
}

int DreamcastWindow::windowShouldClose() {
    return 0;
}

#endif