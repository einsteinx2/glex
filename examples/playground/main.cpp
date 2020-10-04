#include "glex/common/log.h"
#include "glex/Application.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
#include "glex/Image.h"
#include "glex/Text.h"
#include "glex/MeshLoader.h"
#include "glex/input/KeyboardInputHandler.h"
#include "glex/input/MouseInputHandler.h"
#include "glex/input/GamepadInputHandler.h"

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <chrono>

#include <iostream>

#ifdef DREAMCAST
extern "C" {
    #include "perfctr.h"
}
#define PERF_COUNTER_WHICH 1
    // No idea why the values are different depending on the GCC version, but it seems the counter is slightly slower on GCC 4 builds
    #if __GNUC__ > 4 && !defined(__clang__)
    // 1,000,000 ms in a ns, each count is approx 6.08333ns, so divide by (1,000,000 / 6.08333) = ~164,383 to get the ms
    #define PERF_COUNTER_NS_TO_MS 164383ULL
    #else
    // 1,000,000 ms in a ns, each count is approx 5.83333ns, so divide by (1,000,000 / 5.83333) = ~171,428 to get the ms
    #define PERF_COUNTER_NS_TO_MS 171428ULL
    #endif
#endif

// Silence annoying printf float warning on Dreamcast 
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wformat"
#endif

// FPS counter
#ifdef DREAMCAST
unsigned long long currentTimeMs = 0;
unsigned long long lastTimeMs = 0;
#else
auto currentTime = std::chrono::high_resolution_clock::now();
auto lastTime = std::chrono::high_resolution_clock::now();
#endif
uint16_t timeDiff = 0;
uint16_t nbFrames = 0;
float frameTime = 0.0;
uint16_t fps = 0;

void cleanExit(Application* app) {
#ifdef DREAMCAST
    PMCR_Disable(PERF_COUNTER_WHICH);
#endif
    app->closeWindow();
}

/* program entry */
int main(int argc, char *argv[]) {
    DEBUG_PRINTLN("Application started!");
    int width = 640;
    int height = 480;
    Application app;
    app.createWindow("GLEX Playground", width, height);

    // Basic keyboard handling
    int lastKeyCode = -1;
    KeyboardInputHandler keyboard;
    keyboard.registerCallback([&app, &lastKeyCode](KeyCode code) {
        lastKeyCode = code;

        // Exit the application when escape key is pressed
        if (code == KeyCode::Escape) {
            DEBUG_PRINTLN("Escape pressed");
            cleanExit(&app);
        } else if (code == KeyCode::A) {
            DEBUG_PRINTLN("A keyboard key pressed");
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
            DEBUG_PRINTLN("L + R + Start pressed!");
            cleanExit(&app);
        }
    });
    app.addInputHandler(std::shared_ptr<GamepadInputHandler>(&gamepad1));

    // NOTE: Both Image and Mesh classes can use textures loaded from JPG, PNG, or BMP.
    //       All PNG files have been run through pngcrush.
    //
    //       I have not yet done profiling to determine the performance and memory 
    //       implications of using each file format. However keep in mind that only PNG
    //       fully supports RGBA as the others do not have alpha channels. The below test
    //       code demostrates loading all 3 types. You can also change the extensions to 
    //       try the other formats yourself for testing/profiling. 
    //
    //       In my limited testing with dcload only, the file size seems to have a much larger
    //       impact on loading time than the format. So I found JPG files load significantly
    //       faster than either PNG or BMP with no noticeable loss in quality (though no alpha).

    Texture grayBrickTexture;
    grayBrickTexture.loadRGB("images/gray_brick_512.jpg");
    Image grayBrickImage(&grayBrickTexture, 0, (float)app.windowHeight(), Image::Z_BACKGROUND, (float)app.windowWidth(), (float)app.windowHeight(), app.screenScale);

    Texture woodTexture;
    woodTexture.loadRGB("images/wood1.bmp");
    Image woodImage(&woodTexture, 250, 420, 10, Image::Z_HUD, 100, app.screenScale);

    Triangle triangle(250, 220, 10, Image::Z_HUD, 100, app.screenScale);

    MeshData *houseMesh = MeshLoader::loadObjMesh("meshes/house.obj");
    Texture houseTexture;
    houseTexture.loadRGBA("images/house_512.png");
    Mesh mesh(houseMesh, &houseTexture, 0.3f);

    Cube cube;

    FontColor darkBlue{21, 1, 148};
    FontFace fontFace = app.screenScale > 1.0 ? FontFace::arial_28 : FontFace::arial_16;
    Text fpsCounter(fontFace, "", darkBlue, 20, 20, Image::Z_HUD, app.screenScale);
    fpsCounter.createTexture();

#ifdef DREAMCAST
    // Init perf counter
    PMCR_Init(PERF_COUNTER_WHICH, PMCR_ELAPSED_TIME_MODE, PMCR_COUNT_CPU_CYCLES);
    lastTimeMs = PMCR_Read(PERF_COUNTER_WHICH) / PERF_COUNTER_NS_TO_MS;
#endif

    // Main loop
    while (!app.windowShouldClose()) {
        // Measure speed
        nbFrames++;

#ifdef DREAMCAST
        currentTimeMs = PMCR_Read(PERF_COUNTER_WHICH) / PERF_COUNTER_NS_TO_MS;
        timeDiff = currentTimeMs - lastTimeMs;
        if (timeDiff >= 1000) {
            frameTime = float(timeDiff) / float(nbFrames);
            fps = nbFrames;
            nbFrames = 0;
            lastTimeMs = currentTimeMs;
        }
#else
        currentTime = std::chrono::high_resolution_clock::now();
        timeDiff = (uint16_t)std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        if (timeDiff >= 1000) {
            frameTime = float(timeDiff) / float(nbFrames);
            fps = nbFrames;
            nbFrames = 0;
            lastTime = currentTime;
        }
#endif

        // Handle input
        app.handleInput();

        // Clear the buffer to draw the prepare frame
        app.clear();

        // Draw the background image
        app.reshapeOrtho(1.0);        
        grayBrickImage.draw();

        // Draw the 3d rotating house
        app.reshapeFrustum();
        mesh.draw();
        mesh.rotationX += 0.75;
        mesh.rotationY += 0.75;
        mesh.rotationZ += 0.75;
        // cube.draw();

        // Draw the foreground 2d image
        app.reshapeOrtho(1.0);
        woodImage.draw();
        triangle.draw();

        // Draw the FPS counter HUD text
        app.reshapeOrtho(fpsCounter.scale);
        static char outputString[50];
        snprintf(&outputString[0], 50, "frame time: %.2f ms  fps: %d  key: %d", frameTime, fps, lastKeyCode);
        fpsCounter.text = outputString;
        fpsCounter.draw();

        // Swap buffers to display the current frame
        app.swapBuffers();
    }

    app.closeWindow();
}
