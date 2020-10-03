#ifdef GLFW

#include "glex/input/MouseInputHandler.h"
#include "glex/common/log.h"
#include "glex/Application.h"

namespace {
    void _internalMouseMovedCallback(GLFWwindow* window, double xpos, double ypos) {
        Application* app = (Application*)glfwGetWindowUserPointer(window);
        for (auto handler : app->inputHandlers()) {
            if (handler->type() == InputType::Mouse) {
                std::dynamic_pointer_cast<MouseInputHandler>(handler)->mouseMoved(xpos, ypos);
            }
        }
    }

    void _internalMouseScrolledCallback(GLFWwindow* window, double xoffset, double yoffset) {
        Application* app = (Application*)glfwGetWindowUserPointer(window);
        for (auto handler : app->inputHandlers()) {
            if (handler->type() == InputType::Mouse) {
                std::dynamic_pointer_cast<MouseInputHandler>(handler)->mouseScrolled(xoffset, yoffset);
            }
        }
    }

    void _internalMouseButtonPressedCallback(GLFWwindow* window, int button, int action, int mods) {
        Application* app = (Application*)glfwGetWindowUserPointer(window);
        for (auto handler : app->inputHandlers()) {
            if (handler->type() == InputType::Mouse) {
                std::dynamic_pointer_cast<MouseInputHandler>(handler)->mouseButtonPressed(button, action, mods);
            }
        }
    }
}

void MouseInputHandler::added(GLFWwindow *window) {
    // Hide the mouse cursor inside the window to mimic console behavior
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Setup internal callbacks
    glfwSetCursorPosCallback(window, _internalMouseMovedCallback);
    glfwSetScrollCallback(window, _internalMouseScrolledCallback);
    glfwSetMouseButtonCallback(window, _internalMouseButtonPressedCallback);
}

void MouseInputHandler::removed(GLFWwindow *window) {
    // Remove internal callbacks
    glfwSetCursorPosCallback(window, NULL);
    glfwSetScrollCallback(window, NULL);
    glfwSetMouseButtonCallback(window, NULL);
}

void MouseInputHandler::mouseMoved(double xpos, double ypos) {
    _currentState.posX = xpos;
    _currentState.posY = ypos;
    _currentState.posDeltaX = xpos - _lastX;
    _currentState.posDeltaY = ypos - _lastY;
    _lastX = xpos;
    _lastY = ypos;
    _mouseMoved = true;
}

void MouseInputHandler::mouseScrolled(double xoffset, double yoffset) {
    _currentState.scrollDeltaX = xoffset;
    _currentState.scrollDeltaY = yoffset;
    _mouseScrolled = true;
}

void MouseInputHandler::mouseButtonPressed(int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_1: _currentState.leftButton = action; break;
    case GLFW_MOUSE_BUTTON_2: _currentState.rightButton = action; break;
    case GLFW_MOUSE_BUTTON_3: _currentState.centerButton = action; break;
    default: break;
    }
    _mouseButtonChanged = true;
}

void MouseInputHandler::poll() {
    // We don't actually poll, so just use this as the trigger to call the callback
    if (_rawCallback != NULL && (_mouseMoved || _mouseScrolled || _mouseButtonChanged)) {
        _rawCallback(_currentState);
    }

    // Reset state booleans
    _mouseMoved = _mouseScrolled = _mouseButtonChanged = false;

    // Reset state deltas
    _currentState.posDeltaX = _currentState.posDeltaY = _currentState.scrollDeltaX = _currentState.scrollDeltaY = 0.0;
}

#endif