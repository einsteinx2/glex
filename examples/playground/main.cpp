#include "debug_log.h"
#include "glex/Window.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
#include "glex/Sphere.h"
#include "glex/Font.h"

//#include "meshes/cubeMesh.h"
//#include "textures/cubeMesh_RGBA_512x512.h"
#include "meshes/houseMesh.h"
#include "textures/houseMesh_RGBA_512x512.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>

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
    Window wrapper;
    wrapper.createWindow("C++ Test", 640, 480);

    // Main loop
    //Triangle triangle;
    Cube cube;
    // Texture cubeTexture;
    // cubeTexture.loadRgbaTexture(512, 512, cubeMesh_RGBA_512x512);
    // Mesh mesh(&cubeMesh, &cubeTexture, 3.0);
    //Texture houseTexture;
    //houseTexture.loadRgbaTexture(512, 512, houseMesh_RGBA_512x512);
    //Mesh mesh(&houseMesh, &houseTexture, 0.3);
    //Mesh mesh(&cubeMesh, NULL, 3.0);
    Sphere sphere(1.5);
    #ifdef DREAMCAST
    Font font(FontFace::arial_16);
    #else
    Font font(FontFace::arial_32);
    #endif
    font.createTexture();
    while (!wrapper.windowShouldClose()) {
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

        // Clear the buffer to draw the prepare frame
        wrapper.clear();

        // Draw the 3d object(s)
        wrapper.reshapeFrustum();
        //triangle.draw();
        //cube.draw();
        //mesh.draw();
        //mesh.rotationX = wrapper.rotationX;
        //mesh.rotationY = wrapper.rotationY;
        //mesh.rotationZ = wrapper.rotationZ;
        sphere.draw();

        // Draw the text
        glClear(GL_DEPTH_BUFFER_BIT);
        wrapper.reshapeOrtho(font.scale);
        static char outputString[50];
        sprintf(&outputString[0], "frame time: %.2f ms  fps: %.2f", frameTime, fps);
        font.draw(20, 20, outputString);

        // Swap buffers to display the current frame
        wrapper.swapBuffers();
    }

    wrapper.closeWindow();
}
