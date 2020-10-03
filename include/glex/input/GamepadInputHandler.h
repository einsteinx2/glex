#pragma once
#include "glex/input/InputHandler.h"
#include "glex/common/gl.h"
#include "glex/input/GamepadState.h"

#include <functional>
#include <string>

#ifdef DREAMCAST
#include <dc/maple.h>
#endif

/*
 * Current limitations:
 *   - Standardizes to an Xbox style controller (will add Dreamcast specific stuff later)
 *   - Currently only raw input state is available (see playground example for how to use),
 *     later additional callbacks will be added to allow watching for specific changes
 *     without writing your own logic
 */

// Gamepad indexes for instantiating GamepadInputHandlers
namespace GamepadIndex {
    constexpr uint8_t FIRST  = 0;
    constexpr uint8_t SECOND = 1;
    constexpr uint8_t THIRD  = 2;
    constexpr uint8_t FOURTH = 3;
};

typedef std::function<void(GamepadState)> GamepadRawStateCallback;

class GamepadInputHandler: public InputHandler {
    public:
        virtual InputType type() { return InputType::Gamepad; }

#ifdef GLFW
        GamepadInputHandler(uint8_t joystickNumber) { _joystickNumber = joystickNumber; }
#else
        GamepadInputHandler() {}
#endif

#ifdef GLFW
        virtual void added(GLFWwindow *window);
        virtual void removed(GLFWwindow *window);
#else
        virtual void added();
        virtual void removed();
#endif
        virtual void poll();

        void registerRawStateCallback(GamepadRawStateCallback rawCallback) { _rawCallback = rawCallback; }
        void unregisterCallback() { _rawCallback = NULL; }

        GamepadState getCurrentState() { return _currentState; }
    private:
        GamepadRawStateCallback _rawCallback = NULL;
        GamepadState _currentState;

#ifdef GLFW
        uint8_t _joystickNumber = UINT8_MAX;
        std::string _gamepadName = std::string("unknown");
#else
        maple_device_t* _mapleDevice = nullptr;
#endif
};
