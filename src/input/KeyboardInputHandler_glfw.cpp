#ifdef GLFW

#include "glex/input/KeyboardInputHandler.h"
#include "glex/common/log.h"
#include "glex/Application.h"

namespace {
    void _internalCallback(GLFWwindow* window, int k, int s, int action, int mods) {
        // For now, only key presses/holds are processed
        // TODO: Add way to pass the action to do things on key up (release)
        if (action == GLFW_RELEASE) {
            return;
        }

        Application* app = (Application*)glfwGetWindowUserPointer(window);
        for (auto handler : app->inputHandlers()) {
            if (handler->type() == InputType::Keyboard) {
                dynamic_cast<KeyboardInputHandler*>(handler)->keyPressed(k);
            }
        }
    }
}

void KeyboardInputHandler::added(GLFWwindow *window) {
    glfwSetKeyCallback(window, _internalCallback);
}

void KeyboardInputHandler::removed(GLFWwindow *window) {
    glfwSetKeyCallback(window, NULL);
}

void KeyboardInputHandler::poll() {
    // Not used for GLFW since we use callbacks instead
}

#endif