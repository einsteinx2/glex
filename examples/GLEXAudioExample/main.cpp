#include "glex/common/log.h"
#include "glex/Application.h"
#include "glex/audio/AudioStream.h"
#include "glex/audio/AudioSoundEffect.h"
#include "glex/input/GamepadInputHandler.h"

void cleanExit(Application* app) {
    app->closeWindow();
}

int main(int argc, char *argv[]) {
#if DEBUG_LOGGING
    // Cause printf (including DEBUG_PRINT/LN) to print immediately
    setbuf(stdout, NULL);
#endif

    DEBUG_PRINTLN("Application started!");
    int width = 640;
    int height = 480;
    Application app;
    app.createWindow("GLEX Audio Example", width, height);

    AudioSoundEffect clap("samples/clap3.wav");
    clap.load();

    AudioStream song("samples/song1.mp3", AudioStreamFileType::MP3);
    // AudioStream song("samples/song1.ogg", AudioStreamFileType::OGG);
    song.load();

    // Basic gamepad handling
    GamepadState lastGamepad1State;
    GamepadInputHandler gamepad1(GamepadIndex::FIRST);
    gamepad1.registerRawStateCallback([&app, &lastGamepad1State, &song, &clap](GamepadState gamepadState) {
        // Check if buttons are pressed
        if (lastGamepad1State.buttons[GamepadButton::A] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::A] == GamepadButtonState::PRESSED) {
            clap.play();
        }
        if (lastGamepad1State.buttons[GamepadButton::B] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::B] == GamepadButtonState::PRESSED) {
            clap.stop();
        }
        if (lastGamepad1State.buttons[GamepadButton::X] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::X] == GamepadButtonState::PRESSED) {
            song.play();
        }
        if (lastGamepad1State.buttons[GamepadButton::Y] == GamepadButtonState::RELEASED && gamepadState.buttons[GamepadButton::Y] == GamepadButtonState::PRESSED) {
            song.stop();
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

    ERROR_PRINTLN("\n\nWARNING: If testing this using dcload, make sure to burn a dcload disc that includes the samples folder or there will be problems playing the music and sound effects together.");
    ERROR_PRINTLN("\n\nPress A to trigger sound effect, B to stop sound effect, X to start background music, Y to stop background music, or L + R + Start to exit...");

    // Main loop
    while (!app.windowShouldClose()) {
        // Handle input
        app.handleInput();
    }

    cleanExit(&app);
}
