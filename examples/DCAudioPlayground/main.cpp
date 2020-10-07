/*
 * NOTE: This is meant for my own personal testing! Please see the "GLEXAudioExample" project to learn how to use audio with GLEX. 
 * 
 * This is a test project to work try out low level Dreamcast AICA sound chip programming.
 * It uses the DCAudio low level header library and a duplicated copy of the KOS sound code 
 * for easily testing changes.
 */

#include "glex/common/log.h"
#include "glex/common/path.h"
#include "glex/Application.h"
#include "glex/graphics/Image.h"
#include "glex/graphics/Text.h"
#include "glex/input/KeyboardInputHandler.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>

// Temp audio stuff
#include <kos.h>
#include <unistd.h>
//#include <dc/sound/sound.h>
//#include <dc/sound/sfxmgr.h>
#include <nsound.h>
#include <nsfxmgr.h>
#include <nstream.h>

std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
int16_t timeDiff = 0;
uint32_t nbFrames = 0;
float frameTime = 0.0;
float fps = 0.0;

// KOS sound stuff
sfxhnd_t kickSample;
sfxhnd_t clapSample;

/* sound thread */
void* thd_0(void *v) {
    //for (int i = 0; i < 4; i++) {
    while(true) {
        nsnd_sfx_play(kickSample, 254, 128);
        DEBUG_PRINTLN("played kick");
        thd_sleep(250);
        nsnd_sfx_play(clapSample, 254, 128);
        DEBUG_PRINTLN("played clap");
        thd_sleep(250);
    }
    return NULL;
}

/* program entry */
int main(int argc, char *argv[]) {
    DEBUG_PRINTLN("Application started!");
    int width = 640;
    int height = 480;
    Application app;
    app.createWindow("Dreamcast Audio Playground", width, height);

    // Basic keyboard handling
    int lastKeyCode = -1;
    KeyboardInputHandler keyboard;
    keyboard.registerCallback([&app, &lastKeyCode](KeyCode code) {
        lastKeyCode = code;

        // Exit the application when escape key is pressed
        if (code == KeyCode::Escape) {
            DEBUG_PRINTLN("Escape pressed");
            app.closeWindow();
        }
    });
    app.addInputHandler(std::shared_ptr<KeyboardInputHandler>(&keyboard));

    Text infoText(FontFace::arial_16, "", FONT_COLOR_WHITE, 20, app.windowHeight() - 40, Image::Z_HUD, app.screenScale);
    infoText.createTexture();

    Text fpsCounter(FontFace::arial_16, "", FONT_COLOR_WHITE, 20, 20, Image::Z_HUD, app.screenScale);
    fpsCounter.createTexture();

    // Sound test stuff
    nsnd_init();
    DEBUG_PRINTLN("Initialized KOS audio");
    kickSample = nsnd_sfx_load(glex::targetPlatformPath("samples/kick3.wav").c_str());
    DEBUG_PRINTLN("Initialized kick sample: %lu", kickSample);
    clapSample = nsnd_sfx_load(glex::targetPlatformPath("samples/clap3.wav").c_str());
    DEBUG_PRINTLN("Initialized clap sample: %lu", clapSample);
    kthread_t *soundThread = thd_create(0, thd_0, NULL);
    DEBUG_PRINTLN("Initialized sound thread: %p", soundThread);

    // Main loop
    while (!app.windowShouldClose()) {
        // // Measure speed
        // nbFrames++;
        // currentTime = std::chrono::steady_clock::now();
        // timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        // if (timeDiff >= 1000) {
        //     frameTime = float(timeDiff) / float(nbFrames);
        //     fps = nbFrames;
        //     nbFrames = 0;
        //     lastTime = currentTime;
        // }

        // Handle input
        app.handleInput();

        // // Clear the buffer to draw the prepare frame
        // app.clear();

        // // Draw the info text
        // app.reshapeOrtho(infoText.scale);
        // static char outputString[50];
        // // NOTE: Due to an old GCC bug, we must manually cast floats to double in order to use %f without a warning 
        // sprintf(&outputString[0], "Info text goes here");
        // infoText.text = outputString;
        // infoText.draw();

        // // Draw the FPS counter HUD text
        // app.reshapeOrtho(fpsCounter.scale);
        // // NOTE: Due to an old GCC bug, we must manually cast floats to double in order to use %f without a warning 
        // sprintf(&outputString[0], "frame time: %.2f ms  fps: %.2f  key: %d", (double)frameTime, (double)fps, lastKeyCode);
        // fpsCounter.text = outputString;
        // fpsCounter.draw();

        // // Swap buffers to display the current frame
        // app.swapBuffers();
    }

    app.closeWindow();
}
