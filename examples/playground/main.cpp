#include "glex/common/log.h"
#include "glex/DreamcastWindow.h"
#include "glex/PCWindow.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
#include "glex/Image.h"
#include "glex/Font.h"
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

    Texture grayBrickTexture;
    grayBrickTexture.loadRGB("images/gray_brick_512.jpg");
    Image grayBrickImage(&grayBrickTexture);

    Texture woodTexture;
    woodTexture.loadRGB("images/wood1.bmp");
    Image woodImage(&woodTexture);

    MeshData *houseMesh = MeshLoader::loadObjMesh("meshes/house.obj");
    Texture houseTexture;
    houseTexture.loadRGBA("images/house_512.jpg");
    Mesh mesh(houseMesh, &houseTexture, 0.3);

    //FontColor darkBlue = (FontColor) { .r = 21, .g = 1, .b = 148 };
    Font font(FontFace::arial_16, FONT_COLOR_BLACK);
    font.createTexture();

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
                exit(0);
            }
        }
#endif

        // Clear the buffer to draw the prepare frame
        window.clear();

        // TODO: Get this drawing behind the 3d mesh on Dreamcast
#ifndef DREAMCAST
        // Draw the background image
        window.reshapeOrtho(1.0);
        //woodImage.draw(0, window.height, -0.9999999, window.width, window.height, window.screenScale);
        grayBrickImage.draw(0, window.height, window.width, window.height, window.screenScale);
#endif

        // Draw the 3d object(s)
        window.reshapeFrustum();
        mesh.draw();
        mesh.rotationX += 0.75;
        mesh.rotationY += 0.75;
        mesh.rotationZ += 0.75;

        // Draw the text
        window.reshapeOrtho(font.scale);
        static char outputString[50];
        sprintf(&outputString[0], "frame time: %.2f ms  fps: %.2f", frameTime, fps);
        font.draw(20, 20, outputString, window.screenScale);

        // Draw the image
        window.reshapeOrtho(1.0);
        woodImage.draw(10, 420, 100, 100, window.screenScale);

        // Swap buffers to display the current frame
        window.swapBuffers();
    }

    window.closeWindow();
}
