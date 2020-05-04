#include "glex/common/log.h"
#include "glex/Application.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
#include "glex/Image.h"
#include "glex/Text.h"
#include "glex/MeshLoader.h"
#include "glex/input/KeyboardInputHandler.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>

std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
uint16_t timeDiff = 0;
uint16_t nbFrames = 0;
float frameTime = 0.0;
uint16_t fps = 0;

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
            app.closeWindow();
        }
    });
    app.addInputHandler(&keyboard);

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

    MeshData *houseMesh = MeshLoader::loadObjMesh("meshes/house.obj");
    Texture houseTexture;
    houseTexture.loadRGBA("images/house_512.png");
    Mesh mesh(houseMesh, &houseTexture, 0.3f);

    FontColor darkBlue{21, 1, 148};
    FontFace fontFace = app.screenScale > 1.0 ? FontFace::arial_28 : FontFace::arial_16;
    Text fpsCounter(fontFace, "", darkBlue, 20, 20, Image::Z_HUD, app.screenScale);
    fpsCounter.createTexture();

    // Main loop
    while (!app.windowShouldClose()) {
        // Measure speed
        nbFrames++;
        currentTime = std::chrono::steady_clock::now();
        timeDiff = (uint16_t)std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        if (timeDiff >= 1000) {
            frameTime = float(timeDiff) / float(nbFrames);
            fps = nbFrames;
            nbFrames = 0;
            lastTime = currentTime;
        }

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

        // Draw the foreground 2d image
        app.reshapeOrtho(1.0);
        woodImage.draw();

        // Draw the FPS counter HUD text
        app.reshapeOrtho(fpsCounter.scale);
        static char outputString[50];
        // NOTE: Due to an old GCC bug, we must manually cast floats to double in order to use %f without a warning 
        sprintf_s(&outputString[0], 50, "frame time: %.2f ms  fps: %.2f  key: %d", (double)frameTime, (double)fps, lastKeyCode);
        fpsCounter.text = outputString;
        fpsCounter.draw();

        // Swap buffers to display the current frame
        app.swapBuffers();
    }

    app.closeWindow();
}
