#ifdef DREAMCAST

#include "glex/Application.h"
#include "glex/common/gl.h"
#include "glex/common/log.h"
#include "glex/input/KeyboardInputHandler.h"

#include <dc/maple/keyboard.h>

#include <cstdlib>
#include <algorithm>

void Application::createWindow(std::string windowName, int width, int height) {
    _windowName = windowName;
    _windowWidth = width;
    _windowHeight = height;

    glKosInit();
    _reshapeFrustum(width, height);
}

void Application::closeWindow() {
    exit(EXIT_SUCCESS);
}

void Application::swapBuffers() {
    glKosSwapBuffers();
}

void Application::handleInput() {
    // On dreamcast, all input is done via polling
    for (auto handler : _inputHandlers) {
        handler->poll();
    }
}

int Application::windowShouldClose() {
    return 0;
}

void Application::_updateWindowSize() {
    // Not implemented for Dreamcast
}

void Application::addInputHandler(std::shared_ptr<InputHandler> inputHandler) {
    _inputHandlers.push_back(inputHandler);
    inputHandler->added();
}

void Application::removeInputHandler(std::shared_ptr<InputHandler> inputHandler) {
    auto first = std::remove(_inputHandlers.begin(), _inputHandlers.end(), inputHandler);
    _inputHandlers.erase(first, _inputHandlers.end());
    inputHandler->removed();
}

#endif