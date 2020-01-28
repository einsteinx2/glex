#ifdef DREAMCAST

#include "glex/Application.h"
#include "glex/common/gl.h"
#include "glex/common/log.h"
#include "glex/input/KeyboardInputHandler.h"

#include <dc/maple/keyboard.h>

#include <cstdlib>

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

    // Handle global keyboard input
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    int key = kbd_get_key();
    if (key != -1) {
        for (auto handler : _inputHandlers) {
            if (handler->type() == InputType::Keyboard) {
                static_cast<KeyboardInputHandler*>(handler)->keyPressed(key);
            }
        }
    }
#pragma GCC diagnostic pop
}

int Application::windowShouldClose() {
    return 0;
}

void Application::_updateWindowSize() {
    // Not implemented for Dreamcast
}

void Application::addInputHandler(InputHandler* inputHandler) {
    _inputHandlers.push_back(inputHandler);
    inputHandler->added();
}

void Application::removeInputHandler(InputHandler* inputHandler) {
    // TODO: Doesn't compile on GCC 4.7
    // error: cannot convert 'std::vector<InputHandler*>::iterator {aka __gnu_cxx::__normal_iterator<InputHandler**, std::vector<InputHandler*> >}' to 'const char*' for argument '1' to 'int remove(const char*)'
    //
    // auto first = std::remove(_inputHandlers.begin(), _inputHandlers.end(), inputHandler);
    // _inputHandlers.erase(first, _inputHandlers.end());
    // inputHandler->removed();
}

#endif