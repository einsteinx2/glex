#include "glex/common/log.h"
#include "glex/DreamcastWindow.h"
#include "glex/PCWindow.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
#include "glex/Image.h"
#include "glex/Text.h"
#include "glex/MeshLoader.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>

#ifdef DREAMCAST
#include <dc/maple/keyboard.h>
#endif

std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
int16_t timeDiff = 0;
uint32_t nbFrames = 0;
float frameTime = 0.0;
float fps = 0.0;

/* program entry */
int main(int argc, char *argv[])
{
    DEBUG_PRINTLN("Application started!");
    int width = 640;
    int height = 480;
#ifdef DREAMCAST
    DreamcastWindow window;
#else
    PCWindow window;
#endif
    window.createWindow("GLEX Playground", width, height);

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

    DEBUG_PRINTLN("window.width: %d  window.height %d  window.screenScale: %f", window.width, window.height, window.screenScale);

    Texture grayBrickTexture;
    grayBrickTexture.loadRGB("images/gray_brick_512.jpg");
    Image grayBrickImage(&grayBrickTexture, 0, window.height, Image::Z_BACKGROUND, window.width, window.height, window.screenScale);

    Texture woodTexture;
    woodTexture.loadRGB("images/wood1.bmp");
    Image woodImage(&woodTexture, 250, 420, 10, Image::Z_HUD, 100, window.screenScale);

    MeshData *houseMesh = MeshLoader::loadObjMesh("meshes/house.obj");
    Texture houseTexture;
    houseTexture.loadRGBA("images/house_512.png");
    Mesh mesh(houseMesh, &houseTexture, 0.3);

    FontColor darkBlue = (FontColor) { .r = 21, .g = 1, .b = 148 };
    FontFace fontFace = window.screenScale > 1.0 ? FontFace::arial_28 : FontFace::arial_16;
    Text fpsCounter(fontFace, "", darkBlue, 20, 20, Image::Z_HUD, window.screenScale);
    fpsCounter.createTexture();

#ifdef DREAMCAST
    int key = -1;
    kbd_set_queue(1); // Enable keyboard queue
#endif

    // Main loop
    while (!window.windowShouldClose()) {
        // Measure speed
        nbFrames++;
        currentTime = std::chrono::steady_clock::now();
        timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        if (timeDiff >= 1000) {
            frameTime = float(timeDiff) / float(nbFrames);
            fps = nbFrames;
            //DEBUG_PRINTLN("%f ms/frame\n", frameTime);
            nbFrames = 0;
            lastTime = currentTime;
        }

#ifdef DREAMCAST
        // Check for escape key press on Dreamcast
        key = kbd_get_key();
        if (key != -1) {
            DEBUG_PRINTLN("Key pressed  int: %d  char: %c", key, key);
            if (key == 27) {
                DEBUG_PRINTLN("Escape key pressed, exiting");
                exit(EXIT_SUCCESS);
            }
        }
#endif

        // Clear the buffer to draw the prepare frame
        window.clear();

        // Draw the background image
        window.reshapeOrtho(1.0);        
        grayBrickImage.draw();

        // Draw the 3d rotating house
        window.reshapeFrustum();
        mesh.draw();
        mesh.rotationX += 0.75;
        mesh.rotationY += 0.75;
        mesh.rotationZ += 0.75;

        // Draw the foreground 2d image
        window.reshapeOrtho(1.0);
        woodImage.draw();

        // Draw the FPS counter HUD text
        window.reshapeOrtho(fpsCounter.scale);
        static char outputString[50];
        sprintf(&outputString[0], "frame time: %.2f ms  fps: %.2f", frameTime, fps);
        fpsCounter.text = outputString;
        fpsCounter.draw();

        // Swap buffers to display the current frame
        window.swapBuffers();
    }

    window.closeWindow();
}
