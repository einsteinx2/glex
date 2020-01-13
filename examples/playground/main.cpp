#include "debug_log.h"
#include "glex/Window.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
#include "glex/Image.h"
#include "glex/Font.h"
#include "glex/MeshLoader.h"

#include "meshes/cubeMesh.h"
#include "textures/cubeMesh_RGBA_512x512.h"
//#include "meshes/houseMesh.h"
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
    int width = 640;
    int height = 480;
    Window wrapper;
    wrapper.createWindow("GLEX Playground", width, height);

    Triangle triangle;
    //Cube cube;
    // MeshData *houseMesh = MeshLoader::loadObjMesh("meshes/cube.obj");
    // Texture cubeTexture;
    // cubeTexture.loadRgbaTexture(512, 512, cubeMesh_RGBA_512x512);
    // Mesh mesh(&cubeMesh, &cubeTexture, 3.0);
    #ifdef DREAMCAST
    std::string meshPath = "/cd/meshes/house.obj";
    #else
    std::string meshPath = "meshes/house.obj";
    #endif

    Texture woodTexture;
    woodTexture.loadBmpTexture("images/wood1.bmp");
    Image woodImage(&woodTexture);

    MeshData *houseMesh = MeshLoader::loadObjMesh(meshPath);
    Texture houseTexture;
    houseTexture.loadRgbaTexture(512, 512, houseMesh_RGBA_512x512);
    Mesh mesh(houseMesh, &houseTexture, 0.3);
    //Mesh mesh(houseMesh, &woodTexture, 0.3);
    //Mesh mesh(&cubeMesh, NULL, 3.0);

    Font font(FontFace::arial_16, FONT_COLOR_BLACK);
    font.createTexture();

    // Main loop
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

        // Draw the background image
        wrapper.reshapeOrtho(1.0);
        //woodImage.draw(0, wrapper.height, wrapper.width, wrapper.height, wrapper.screenScale);
        woodImage.draw(0, 480, 640, 480, wrapper.screenScale);

        // Draw the 3d object(s)
        wrapper.reshapeFrustum();
        //triangle.draw();
        //cube.draw();
        mesh.draw();
        //mesh.rotationX = wrapper.rotationX;
        //mesh.rotationY = wrapper.rotationY;
        //mesh.rotationZ = wrapper.rotationZ;

        // Draw the text
        wrapper.reshapeOrtho(font.scale);
        static char outputString[50];
        sprintf(&outputString[0], "frame time: %.2f ms  fps: %.2f", frameTime, fps);
        font.draw(20, 20, outputString, wrapper.screenScale);

        // Draw the image
        // wrapper.reshapeOrtho(1.0);
        // woodImage.draw(500, 500, 256, 256);

        // Swap buffers to display the current frame
        wrapper.swapBuffers();
    }

    wrapper.closeWindow();
}
