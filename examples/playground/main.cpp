#include "debug_log.h"
#include "glex/Window.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
//#include "glex/Sphere.h"
#include "glex/Font.h"

//#include "meshes/cubeMesh.h"
//#include "textures/cubeMesh_RGBA_512x512.h"
#include "meshes/houseMesh.h"
#include "textures/houseMesh_RGBA_512x512.h"

#include <cstdio>
#include <cstdlib>
#include <string>

double currentTime = 0;
#ifdef DREAMCAST
double lastTime = 0;
#else
double lastTime = glfwGetTime();
#endif
double timeDiff = 0.0;
int nbFrames = 0;
double frameTime = 0.0;
double fps = 0.0;

/* program entry */
int main(int argc, char *argv[])
{
    DEBUG_PRINTLN("Application started!");
    Window wrapper;
    wrapper.createWindow("C++ Test", 640, 480);

    // Main loop
    //Triangle triangle;
    //Cube cube;
    // Texture cubeTexture;
    // cubeTexture.loadRgbaTexture(512, 512, cubeMesh_RGBA_512x512);
    // Mesh mesh(&cubeMesh, &cubeTexture, 3.0);
    Texture houseTexture;
    houseTexture.loadRgbaTexture(512, 512, houseMesh_RGBA_512x512);
    Mesh mesh(&houseMesh, &houseTexture, 0.3);
    //Mesh mesh(&cubeMesh, NULL, 3.0);
    //Sphere sphere(1.5);
    Font font(FontFace::arial_32);
    font.createTexture();
    while (!wrapper.windowShouldClose()) {
        // Measure speed
        #ifdef DREAMCAST
        currentTime = 0;
        #else
        currentTime = glfwGetTime();
        #endif
        nbFrames++;
        timeDiff = currentTime - lastTime;
        if (timeDiff >= 1.0) {
            frameTime = (timeDiff * 1000.0) / double(nbFrames);
            fps = nbFrames;
            //DEBUG_PRINTLN("%f ms/frame\n", frameTime);
            nbFrames = 0;
            lastTime += timeDiff;
        }

        // Clear the buffer to draw the prepare frame
        wrapper.clear();

        // Draw the 3d object(s)
        wrapper.reshapeFrustum();
        //triangle.draw();
        //cube.draw();
        mesh.draw();
        //mesh.rotationX = wrapper.rotationX;
        //mesh.rotationY = wrapper.rotationY;
        //mesh.rotationZ = wrapper.rotationZ;
        //sphere.draw();

        // Draw the text
        glClear(GL_DEPTH_BUFFER_BIT);
        wrapper.reshapeOrtho(font.scale);
        static std::string outputString;
        #ifdef DREAMCAST
        // static char* outputCString;
        // sprintf(outputCString, "frame time: %f ms  fps: %f", frameTime, fps);
        // outputString = outputCString; 
        outputString = "Running on Dreamcast";
        #else
        outputString = "frame time: " + std::to_string(frameTime) + "ms  fps: " + std::to_string(fps); 
        #endif
        font.draw(20, 20, outputString);

        // Swap buffers to display the current frame
        wrapper.swapBuffers();
    }

    wrapper.closeWindow();
}
