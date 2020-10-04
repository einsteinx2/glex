#ifdef DREAMCAST

#include "glex/input/GamepadInputHandler.h"
#include "glex/common/log.h"

#include <dc/maple/controller.h>
#include <dc/maple.h>

void GamepadInputHandler::added() {
    // Get the first connected mouse
    // TODO: Support more than one mouse
    // TODO: Support plugging in a mouse after the DC boots
    _mapleDevice = maple_enum_type(_gamepadIndex, MAPLE_FUNC_CONTROLLER);
    if (_mapleDevice == NULL) {
        ERROR_PRINTLN("Error: GamepadInputHandler failed to find controller device at index %d, no controller connected", _gamepadIndex);
        return;
    }

    DEBUG_PRINTLN("GamepadInputHandler found controller device %d plugged into port: %d", _gamepadIndex, _mapleDevice->port);
}

void GamepadInputHandler::removed() {
    // TODO: Need to free() this first?
    _mapleDevice = NULL;
}

void GamepadInputHandler::poll() {
    if (_mapleDevice != NULL) {
        cont_state_t *contState = (cont_state_t *)maple_dev_status(_mapleDevice);
        if (contState != NULL) {
            // Process all controller changes
            GamepadState glexState;
            glexState.buttons[GamepadButton::A]          = (bool)(contState->buttons & CONT_A);
            glexState.buttons[GamepadButton::B]          = (bool)(contState->buttons & CONT_B);
            glexState.buttons[GamepadButton::X]          = (bool)(contState->buttons & CONT_X);
            glexState.buttons[GamepadButton::Y]          = (bool)(contState->buttons & CONT_Y);
            glexState.buttons[GamepadButton::START]      = (bool)(contState->buttons & CONT_START);
            glexState.buttons[GamepadButton::DPAD_UP]    = (bool)(contState->buttons & CONT_DPAD_UP);
            glexState.buttons[GamepadButton::DPAD_RIGHT] = (bool)(contState->buttons & CONT_DPAD_RIGHT);
            glexState.buttons[GamepadButton::DPAD_DOWN]  = (bool)(contState->buttons & CONT_DPAD_DOWN);
            glexState.buttons[GamepadButton::DPAD_LEFT]  = (bool)(contState->buttons & CONT_DPAD_LEFT);

            // Constrain L stick to -1.0 - 1.0 range
            glexState.analog[GamepadAnalog::L_STICK_X]   = (((float)contState->joyx + 128.0) / 128.0) - 1.0;
            glexState.analog[GamepadAnalog::L_STICK_Y]   = -((((float)contState->joyy + 128.0) / 128.0) - 1.0);

            // Constrain L and R triggers to 0.0 - 1.0 range
            glexState.analog[GamepadAnalog::L_TRIGGER]   = (float)contState->ltrig / 255.0;
            glexState.analog[GamepadAnalog::R_TRIGGER]   = (float)contState->rtrig / 255.0;

            // Check for changes
            if (_rawCallback != NULL && _currentState != glexState) {
                _currentState = glexState;
                _rawCallback(_currentState);
            } else {
                _currentState = glexState;
            }
        }
    }
}

#endif