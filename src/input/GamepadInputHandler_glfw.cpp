#ifdef GLFW

#include "glex/input/GamepadInputHandler.h"
#include "glex/common/log.h"
#include "glex/Application.h"

#include <cstring>

void GamepadInputHandler::added(GLFWwindow *window) {
    // Check if the gamepad exists
    if (!glfwJoystickPresent(_gamepadIndex)) {
        DEBUG_PRINTLN("GLFW joystick number %u doesn't exist!", _gamepadIndex);
        return;
    }

    // Make sure it's actually a gamepad
    if(!glfwJoystickIsGamepad(_gamepadIndex)) {
        DEBUG_PRINTLN("GLFW joystick number %u isn't a gamepad!", _gamepadIndex);
        return;
    }

    // Print the gamepad name
    _gamepadName = std::string(glfwGetGamepadName(_gamepadIndex));
    DEBUG_PRINTLN("Gamepad added: %u - %s", _gamepadIndex, _gamepadName.c_str());
}

void GamepadInputHandler::removed(GLFWwindow *window) {
    // Print the gamepad name
    DEBUG_PRINTLN("Gamepad removed: %u - %s", _gamepadIndex, _gamepadName.c_str());
}

void GamepadInputHandler::poll() {
    // Get the GLFW gamepad state
    GLFWgamepadstate glfwState;
    if (glfwGetGamepadState(_gamepadIndex, &glfwState)) {
        // Convert to GLEX state format
        GamepadState glexState;
        std::memcpy(glexState.buttons, glfwState.buttons, sizeof(unsigned char) * (GamepadButton::LAST + 1));
        std::memcpy(glexState.analog, glfwState.axes, sizeof(float) * (GamepadAnalog::LAST + 1));
        _currentState = glexState;

        // Call the raw state callback if it exists
        if (_rawCallback) {
            _rawCallback(_currentState);
        }
    }
}

#endif