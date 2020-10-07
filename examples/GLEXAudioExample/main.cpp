#include "glex/common/log.h"
#include "glex/Application.h"
#include "glex/audio/Audio.h"
#include "glex/input/GamepadInputHandler.h"

void cleanExit(Application* app) {
    app->closeWindow();
}

int main(int argc, char *argv[]) {
    DEBUG_PRINTLN("Application started!");
    int width = 640;
    int height = 480;
    Application app;
    app.createWindow("GLEX Audio Example", width, height);

    Audio clap("samples/clap3.wav");
    clap.load();

    // Basic gamepad handling
    GamepadState lastGamepad1State;
    GamepadInputHandler gamepad1(GamepadIndex::FIRST);
    gamepad1.registerRawStateCallback([&app, &lastGamepad1State, &clap](GamepadState gamepadState) {
        // Check if buttons are pressed
        if (lastGamepad1State.buttons[GamepadButton::A] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::A] == GamepadButtonState::PRESSED) {
            clap.play();
        }
        if (lastGamepad1State.buttons[GamepadButton::B] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::B] == GamepadButtonState::PRESSED) {
            clap.pause();
        }
        if (lastGamepad1State.buttons[GamepadButton::X] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::X] == GamepadButtonState::PRESSED) {
            clap.stop();
        }
        
        // Store last state
        lastGamepad1State = gamepadState;

        // Check for L + R + Start button combo to exit the app
        if (gamepadState.analog[GamepadAnalog::L_TRIGGER] > 0.9 && gamepadState.analog[GamepadAnalog::R_TRIGGER] > 0.9 && gamepadState.buttons[GamepadButton::START] == GamepadButtonState::PRESSED) {
            DEBUG_PRINTLN("L + R + Start pressed!");
            cleanExit(&app);
        }
    });
    app.addInputHandler(std::shared_ptr<GamepadInputHandler>(&gamepad1));

    DEBUG_PRINTLN("\n\nPress A to trigger sound effect or L + R + Start to exit...");

    // Main loop
    while (!app.windowShouldClose()) {
        // Handle input
        app.handleInput();
    }

    cleanExit(&app);
}
