#include "glex/common/log.h"
#include "glex/Application.h"
#include "glex/input/KeyboardInputHandler.h"
#include "glex/input/MouseInputHandler.h"
#include "glex/input/GamepadInputHandler.h"

// Silence annoying printf float warning on Dreamcast 
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wformat"
#endif

void cleanExit(Application* app) {
    app->closeWindow();
}

int main(int argc, char *argv[]) {
    DEBUG_PRINTLN("Application started!");
    int width = 640;
    int height = 480;
    Application app;
    app.createWindow("GLEX Input Example", width, height);

    // Basic keyboard handling
    KeyboardInputHandler keyboard;
    keyboard.registerCallback([&app](KeyCode code) {
        DEBUG_PRINTLN("Keyboard key pressed: %d", code);

        // Exit the application when escape key is pressed
        if (code == KeyCode::Escape) {
            DEBUG_PRINTLN("Escape pressed...Exiting!");
            cleanExit(&app);
        }
    });
    app.addInputHandler(std::shared_ptr<KeyboardInputHandler>(&keyboard));

    // Basic mouse handling
    MouseState lastMouseState;
    MouseInputHandler mouse;
    mouse.registerRawStateCallback([&lastMouseState](MouseState mouseState) {
        // Check if mouse moved
        if (mouseState.posDeltaX != 0.0 || mouseState.posDeltaY != 0.0) {
            DEBUG_PRINTLN("Mouse moved - pos: (x: %f, y: %f) delta: (dX: %f, dY: %f)", mouseState.posX, mouseState.posY, mouseState.posDeltaX, mouseState.posDeltaY);
        }
        
        // Check if mouse scrolled
        if (mouseState.scrollDeltaX != 0.0 || mouseState.scrollDeltaY != 0.0) {
            DEBUG_PRINTLN("Mouse scrolled - delta: (dX: %f, dY: %f)", mouseState.scrollDeltaX, mouseState.scrollDeltaY);
        }

        // Check if buttons are pressed
        if (lastMouseState.leftButton == MouseButtonState::RELEASED && mouseState.leftButton == MouseButtonState::PRESSED) {
            DEBUG_PRINTLN("Mouse left button pressed");
        }
        if (lastMouseState.centerButton == MouseButtonState::RELEASED && mouseState.centerButton == MouseButtonState::PRESSED) {
            DEBUG_PRINTLN("Mouse center button pressed");
        }
        if (lastMouseState.rightButton == MouseButtonState::RELEASED && mouseState.rightButton == MouseButtonState::PRESSED) {
            DEBUG_PRINTLN("Mouse right button pressed");
        }

        // Check if buttons are released
        if (lastMouseState.leftButton == MouseButtonState::PRESSED && mouseState.leftButton == MouseButtonState::RELEASED) {
            DEBUG_PRINTLN("Mouse left button released");
        }
        if (lastMouseState.centerButton == MouseButtonState::PRESSED && mouseState.centerButton == MouseButtonState::RELEASED) {
            DEBUG_PRINTLN("Mouse center button released");
        }
        if (lastMouseState.rightButton == MouseButtonState::PRESSED && mouseState.rightButton == MouseButtonState::RELEASED) {
            DEBUG_PRINTLN("Mouse right button released");
        }

        // Store last state
        lastMouseState = mouseState;
    });
    app.addInputHandler(std::shared_ptr<MouseInputHandler>(&mouse));

    // Basic gamepad handling
    GamepadState lastGamepad1State;
    GamepadInputHandler gamepad1(GamepadIndex::FIRST);
    gamepad1.registerRawStateCallback([&app, &lastGamepad1State](GamepadState gamepadState) {
        // Check if buttons are pressed
        if (lastGamepad1State.buttons[GamepadButton::A] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::A] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("A pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::B] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::B] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("B pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::X] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::X] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("X pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::Y] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::Y] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("Y pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::START] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::START] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("Start pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::L_SHOULDER] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::L_SHOULDER] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("Left Shoulder pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::R_SHOULDER] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::R_SHOULDER] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("Right Shoulder pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::L_STICK] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::L_STICK] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("Left Stick pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::R_STICK] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::R_STICK] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("Right Stick pressed");
        }

        // Check if buttons are released
        if (lastGamepad1State.buttons[GamepadButton::A] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::A] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("A released");
        }
        if (lastGamepad1State.buttons[GamepadButton::B] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::B] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("B released");
        }
        if (lastGamepad1State.buttons[GamepadButton::X] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::X] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("X released");
        }
        if (lastGamepad1State.buttons[GamepadButton::Y] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::Y] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("Y released");
        }
        if (lastGamepad1State.buttons[GamepadButton::START] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::START] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("Start released");
        }
        if (lastGamepad1State.buttons[GamepadButton::L_SHOULDER] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::L_SHOULDER] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("Left Shoulder released");
        }
        if (lastGamepad1State.buttons[GamepadButton::R_SHOULDER] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::R_SHOULDER] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("Right Shoulder released");
        }
        if (lastGamepad1State.buttons[GamepadButton::L_STICK] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::L_STICK] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("Left Stick released");
        }
        if (lastGamepad1State.buttons[GamepadButton::R_STICK] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::R_STICK] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("Right Stick released");
        }

        // Check if dpad directions are pressed
        if (lastGamepad1State.buttons[GamepadButton::DPAD_DOWN] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::DPAD_DOWN] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("DPAD Down pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::DPAD_LEFT] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::DPAD_LEFT] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("DPAD Left pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::DPAD_RIGHT] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::DPAD_RIGHT] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("DPAD Right pressed");
        }
        if (lastGamepad1State.buttons[GamepadButton::DPAD_UP] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::DPAD_UP] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("DPAD Up pressed");
        }

        // Check if dpad directions are released
        if (lastGamepad1State.buttons[GamepadButton::DPAD_DOWN] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::DPAD_DOWN] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("DPAD Down released");
        }
        if (lastGamepad1State.buttons[GamepadButton::DPAD_LEFT] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::DPAD_LEFT] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("DPAD Left released");
        }
        if (lastGamepad1State.buttons[GamepadButton::DPAD_RIGHT] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::DPAD_RIGHT] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("DPAD Right released");
        }
        if (lastGamepad1State.buttons[GamepadButton::DPAD_UP] == GamepadButtonState::PRESSED && gamepadState.buttons[GamepadButton::DPAD_UP] == GamepadButtonState::RELEASED) {
            DEBUG_PRINTLN("DPAD Up released");
        }

        // Check analog sticks and triggers
        if (lastGamepad1State.analog[GamepadAnalog::L_STICK_X] != gamepadState.analog[GamepadAnalog::L_STICK_X] || lastGamepad1State.analog[GamepadAnalog::L_STICK_Y] != gamepadState.analog[GamepadAnalog::L_STICK_Y]) {
            DEBUG_PRINTLN("Left Stick: (x: %f, y: %f)", gamepadState.analog[GamepadAnalog::L_STICK_X], gamepadState.analog[GamepadAnalog::L_STICK_Y]);
        }
         if (lastGamepad1State.analog[GamepadAnalog::R_STICK_X] != gamepadState.analog[GamepadAnalog::R_STICK_X] || lastGamepad1State.analog[GamepadAnalog::R_STICK_Y] != gamepadState.analog[GamepadAnalog::R_STICK_Y]) {
            DEBUG_PRINTLN("Right Stick: (x: %f, y: %f)", gamepadState.analog[GamepadAnalog::R_STICK_X], gamepadState.analog[GamepadAnalog::R_STICK_Y]);
        }
        if (lastGamepad1State.analog[GamepadAnalog::L_TRIGGER] != gamepadState.analog[GamepadAnalog::L_TRIGGER]) {
            DEBUG_PRINTLN("Left Trigger: %f", gamepadState.analog[GamepadAnalog::L_TRIGGER]);
        }
        if (lastGamepad1State.analog[GamepadAnalog::R_TRIGGER] != gamepadState.analog[GamepadAnalog::R_TRIGGER]) {
            DEBUG_PRINTLN("Right Trigger: %f", gamepadState.analog[GamepadAnalog::R_TRIGGER]);
        }

        // Store last state
        lastGamepad1State = gamepadState;

        // Check for L + R + Start button combo to exit the app
        if (gamepadState.analog[GamepadAnalog::L_TRIGGER] > 0.9 && gamepadState.analog[GamepadAnalog::R_TRIGGER] > 0.9 && gamepadState.buttons[GamepadButton::START] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("L + R + Start pressed...Exiting!");
            cleanExit(&app);
        }
    });
    app.addInputHandler(std::shared_ptr<GamepadInputHandler>(&gamepad1));

    DEBUG_PRINTLN("\n\nPress L + R + Start or ESC to exit...");

    // Main loop
    while (!app.windowShouldClose()) {
        // Handle input
        app.handleInput();
    }

    cleanExit(&app);
}
