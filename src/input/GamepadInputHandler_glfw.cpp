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

        glexState.analog[GamepadAnalog::L_STICK_X] = glfwState.axes[GamepadAnalog::L_STICK_X];
        glexState.analog[GamepadAnalog::R_STICK_X] = glfwState.axes[GamepadAnalog::R_STICK_X];

        // Invert L and R sticks Y-axis so that up is positive
        glexState.analog[GamepadAnalog::L_STICK_Y] = -(glfwState.axes[GamepadAnalog::L_STICK_Y]);
        glexState.analog[GamepadAnalog::R_STICK_Y] = -(glfwState.axes[GamepadAnalog::R_STICK_Y]);
        
        // Convert L and R triggers to 0.0 - 1.0 range
        glexState.analog[GamepadAnalog::L_TRIGGER] = (glfwState.axes[GamepadAnalog::L_TRIGGER] + 1.0) / 2.0;
        glexState.analog[GamepadAnalog::R_TRIGGER] = (glfwState.axes[GamepadAnalog::R_TRIGGER] + 1.0) / 2.0;

        _currentState = glexState;

        // Call the raw state callback if it exists
        if (_rawCallback) {
            _rawCallback(_currentState);
        }
    }
}

#endif