#include "debug_log.h"
#include "library/Wrapper.hpp"
#include "library/Triangle.hpp"
#include "library/Cube.hpp"
#include "library/Mesh.hpp"
#include "library/Sphere.hpp"
#include "library/Font.hpp"

#include "meshes/houseFilledTrianglesMesh.hpp"
#include "textures/houseMesh_RGBA_512x512.h"

double currentTime = 0;
double lastTime = glfwGetTime();
double timeDiff = 0.0;
int nbFrames = 0;
double frameTime = 0.0;
double fps = 0.0;

/* program entry */
int main(int argc, char *argv[])
{
    DEBUG_PRINTLN("Application started!");
    Wrapper wrapper("C++ Test");
    wrapper.createWindow();

    // Main loop
    //Triangle triangle;
    //Cube cube;
    Texture houseTexture;
    houseTexture.loadRgbaTexture(512, 512, houseMesh_RGBA_512x512);
    Mesh mesh(&houseFilledTrianglesMesh, &houseTexture, 0.3);
    //Sphere sphere(1.5);
    Font font(FontFace::arial_32);
    font.createTexture();
    while (!glfwWindowShouldClose(wrapper.window)) {
        // Measure speed
        currentTime = glfwGetTime();
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
        mesh.rotationX = wrapper.rotationX;
        mesh.rotationY = wrapper.rotationY;
        mesh.rotationZ = wrapper.rotationZ;
        //sphere.draw();

        // Draw the text
        glClear(GL_DEPTH_BUFFER_BIT);
        wrapper.reshapeOrtho(font.scale);
        static std::string outputString;
        outputString = "frame time: " + std::to_string(frameTime) + "ms  fps: " + std::to_string(fps); 
        font.draw(20, 20, outputString);

        // Swap buffers to display the current frame
        wrapper.swapBuffers();
    }

    wrapper.closeWindow();
}
