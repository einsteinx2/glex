#include "glex/common/log.h"
#include "glex/common/gl.h"
#include "glex/common/font.h"
#include "glex/fonts/arial_16pt.h"
#include "glex/fonts/arial_28pt.h"
#include "glex/fonts/arial_32pt.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
#include "glex/Image.h"
#include "glex/Text.h"
#include "glex/MeshLoader.h"

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <chrono>

 // NOTE: zNear is set to -100 and zFar to 100 in glOrtho, but it seems to work backwards...
//       -100 is the farthest away (e.g. behind everything) while 100 is the closet (draws over everything)

// Use this as the z value to draw behind everything (e.g. a background image)
static constexpr float Z_BACKGROUND = -100;
// Use this as the z value to draw on top of everything (e.g. a HUD or FPS counter)
static constexpr float Z_HUD = 100;

std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
uint16_t timeDiff = 0;
uint16_t nbFrames = 0;
float frameTime = 0.0;
uint16_t fps = 0;

bool vsyncEnabled = true; 
int _windowWidth = 0;
int _windowHeight = 0;
std::string _windowName = "";
float screenScale = 1.0;
#ifdef GLFW
    GLFWwindow* _window;
#endif

void _reshapeFrustum(int width, int height) {
    GLfloat h = (GLfloat)height / (GLfloat)width;
    GLfloat xmax, znear, zfar;

    znear = 5.0f;
    zfar  = 30.0f;
    xmax  = znear * 0.5f;

    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-xmax, xmax, -xmax * h, xmax * h, znear, zfar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.5, 0.5, -20.0);
}

void reshapeFrustum() {
    _reshapeFrustum((int)((float)_windowWidth * screenScale), (int)((float)_windowHeight * screenScale));
}

void _reshapeOrtho(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -100.1, 100.1); // Added .1 to znear and zfar to allow using the full -100 - 100 range
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);
}

void reshapeOrtho(float scale) {
    _reshapeOrtho(_windowWidth, _windowHeight);
    glViewport(0, 0, (int)((float)_windowWidth * scale * screenScale), (int)((float)_windowHeight * screenScale));
}

#ifdef GLFW
void _updateWindowSize() {
    glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);
}

void _sizeCallback(GLFWwindow* window, int width, int height) {
    DEBUG_PRINTLN("entered _sizeCallback");
    _updateWindowSize();
    _reshapeFrustum(width, height);
    _reshapeOrtho(width, height);
}
#endif

void createWindow(std::string windowName, int width, int height) {
#ifdef GLFW
    _windowName = windowName;
    _windowWidth = width;
    _windowHeight = height;

    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    _window = glfwCreateWindow(width, height, _windowName.c_str(), NULL, NULL);
    if (!_window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(_window, _sizeCallback);

    // Lock to (probably) 60fps if vsyncEnabled, or unlock framerate
    glfwMakeContextCurrent(_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(vsyncEnabled ? 1 : 0);

    // Setup the frame buffer and view port size
    _reshapeFrustum(width, height);
    //_reshapeOrtho(width, height); // Don't seem to need to set ortho size here...

    // Detect screen scale by comparing window size to framebuffer size
    int frameBufferWidth;
    glfwGetFramebufferSize(_window, &frameBufferWidth, NULL);
    screenScale = (float)frameBufferWidth / (float)width;
    DEBUG_PRINTLN("screen scale detected: %f", screenScale);
#else
    _windowName = windowName;
    _windowWidth = width;
    _windowHeight = height;

    glKosInit();
    _reshapeFrustum(width, height);
#endif
}

void handleInput() {
#ifdef GLFW
    // Calls all GLFW event handler callback functions
    glfwPollEvents();
#endif
}

void clear() {
    // Set the background color
    glClearColor(0.0, 0.0, 0.0, 0.75);

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void swapBuffers() {
#ifdef GLFW
    glfwSwapBuffers(_window);
#else
    glKosSwapBuffers();
#endif
}

int windowShouldClose() {
#ifdef GLFW
    return glfwWindowShouldClose(_window);
#else
    return false;
#endif
}

void closeWindow() {
#ifdef GLFW
    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
#else
    exit(EXIT_SUCCESS);
#endif
}

Texture grayBrickTexture;
int grayBrickTextureId = 0;
Texture woodTexture;
int woodTextureId = 0;
Texture houseTexture;
int houseTextureId = 0;
void loadTextures() {
    grayBrickTexture.loadRGB("images/gray_brick_512.jpg");
    grayBrickTextureId = grayBrickTexture.id;

    woodTexture.loadRGB("images/wood1.bmp");
    woodTextureId = woodTexture.id;

    houseTexture.loadRGBA("images/house_512.png");
    houseTextureId = houseTexture.id;
}

void drawImage(int textureId_, float x_, float y_, float z_, float width_, float height_, float windowScale_, float scale_ = 1.0) {
    // Set OpenGL draw settings
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Perform scaling and rotation
    glScalef(scale_, scale_, 1.0);
    // glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    // glRotatef(rotationX, 1.0f, 0.0f, 0.0f);

    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    // Draw the image
    glBegin(GL_TRIANGLES);
    glColor4f(1.0, 1.0, 1.0, 1.0);

    // Left triangle
    glTexCoord2f(0, 1); glVertex3f(x_,          y_            , z_);  // Top Left
    glTexCoord2f(0, 0); glVertex3f(x_,          y_ - height_  , z_);  // Bottom Left
    glTexCoord2f(1, 0); glVertex3f(x_ + width_, y_ - height_  , z_);  // Buttom Right
    
    // Right triangle
    glTexCoord2f(0, 1); glVertex3f(x_,          y_            , z_);  // Top Left
    glTexCoord2f(1, 0); glVertex3f(x_ + width_, y_ - height_  , z_);  // Bottom Right
    glTexCoord2f(1, 1); glVertex3f(x_ + width_, y_            , z_);  // Top Right
    
    glEnd();

    // Unset OpenGL draw settings
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glPopMatrix();
}

MeshData *_meshData = MeshLoader::loadObjMesh("meshes/house.obj");
float meshRotationX = 0.0;
float meshRotationY = 0.0;
float meshRotationZ = 0.0;
float meshScale = 0.3f;
void drawMesh(int meshTextureId) {
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Cull backfacing polygons 
    glCullFace(GL_BACK); 
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glPushMatrix();

    glScalef(meshScale, meshScale, meshScale);

    glRotatef(meshRotationZ, 0.0f, 0.0f, 1.0f);
    glRotatef(meshRotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(meshRotationX, 1.0f, 0.0f, 0.0f);
    
    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, meshTextureId);

    glEnableClientState(GL_VERTEX_ARRAY); //enable vertex array
    glEnableClientState(GL_NORMAL_ARRAY); //enable normal array
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); //enable texcoord array

    glVertexPointer(3, GL_FLOAT, 0, &_meshData->vertices[0]); //give vertex array to OGL
    glTexCoordPointer(2, GL_FLOAT, 0, &_meshData->textureCoordinates[0]); //same with texcoord array
    glNormalPointer(GL_FLOAT, 0, &_meshData->normals[0]); //and normal array

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_meshData->numVertices);

    glDisableClientState(GL_VERTEX_ARRAY); //disable the client states again
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

FontColor darkBlue{21, 1, 148};
FontFace fontFace = screenScale > 1.0 ? FontFace::arial_28 : FontFace::arial_16;
texture_font_t _font;
Texture _fontTexture;
int _fontTextureId = 0;

void createTextTexture(FontFace face_, FontColor color_) {
    switch(face_) {
    case FontFace::arial_16:  _font = arial_16pt; break;
    case FontFace::arial_28:  _font = arial_28pt; break;
    case FontFace::arial_32:  _font = arial_32pt; break;
    default:
        DEBUG_PRINTLN("Unsupported font type");
        exit(EXIT_FAILURE);
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Optimization for white text
    if (color_ == FONT_COLOR_WHITE) {
        // Use 8bit alpha only texture to save memory
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei)_font.tex_width, (GLsizei)_font.tex_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &_font.tex_data[0]);
    } else {
        // Convert the texture data to 32bit RGBA
        size_t currentSize = _font.tex_data.size();
        size_t newSize = _font.tex_data.size() * 4;
        uint8_t* rgbData = new uint8_t[newSize];
        for (size_t i = 0; i < currentSize; i+=1) {
            rgbData[(i*4)]   = color_.r;
            rgbData[(i*4)+1] = color_.g;
            rgbData[(i*4)+2] = color_.b;
            rgbData[(i*4)+3] = _font.tex_data[i];
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)_font.tex_width, (GLsizei)_font.tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbData);
    }

    _fontTexture.loadExisting((GLsizei)_font.tex_width, (GLsizei)_font.tex_height, textureId);
    _fontTextureId = _fontTexture.id;
}

void drawText(std::string text, float x_, float y_, float z_) {
    // Set OpenGL draw settings
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _fontTextureId);

    size_t i, j;
    float ix = x_, iy = y_;
    for(i = 0; i < text.length(); ++i) {
        // Find the glyph
        texture_glyph_t* glyph = 0;
        for(j = 0; j < _font.glyphs_count; ++j) {
            if (_font.glyphs[j].charcode == text[i]) {
                glyph = &_font.glyphs[j];
                break;
            }
        }
        if(!glyph) {
            continue;
        }

        // Calculate the size and location based on the current character's glyph
        float ox = ix + glyph->offset_x;// + kerning;
        float oy = iy + glyph->offset_y;
        float w  = (float)glyph->width;
        float h  = (float)glyph->height;
        
        // Draw the letter
        glBegin(GL_TRIANGLES);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glTexCoord2f(glyph->s0, glyph->t0); glVertex3f(ox,     oy    , z_);
        glTexCoord2f(glyph->s0, glyph->t1); glVertex3f(ox,     oy - h, z_);
        glTexCoord2f(glyph->s1, glyph->t1); glVertex3f(ox + w, oy - h, z_);
        glTexCoord2f(glyph->s0, glyph->t0); glVertex3f(ox,     oy    , z_);
        glTexCoord2f(glyph->s1, glyph->t1); glVertex3f(ox + w, oy - h, z_);
        glTexCoord2f(glyph->s1, glyph->t0); glVertex3f(ox + w, oy    , z_);
        glEnd();
        
        // Advance to the next letter's position
        ix += glyph->advance_x;
        iy += glyph->advance_y;
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

/* program entry */
int main(int argc, char *argv[]) {
    DEBUG_PRINTLN("Application started!");
    createWindow("GLEX Playground", 640, 480);

    loadTextures();
    createTextTexture(fontFace, darkBlue);

    // Main loop
    while (!windowShouldClose()) {
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
        handleInput();

        // Clear the buffer to draw the prepare frame
        clear();

        // Draw the background image
        reshapeOrtho(1.0);
        drawImage(grayBrickTextureId, 0, (float)_windowHeight, Z_BACKGROUND, _windowWidth, (float)_windowHeight, screenScale);

        // Draw the 3d rotating house
        reshapeFrustum();
        drawMesh(houseTextureId);
        meshRotationX += 0.75;
        meshRotationY += 0.75;
        meshRotationZ += 0.75;

        // Draw the foreground 2d image
        reshapeOrtho(1.0);
        drawImage(woodTextureId, 250, 420, 10, Z_HUD, 100, screenScale);

        // Draw the FPS counter HUD text
        reshapeOrtho(1.0);
        static char outputString[50];
        // NOTE: Due to an old GCC bug, we must manually cast floats to double in order to use %f without a warning 
        snprintf(&outputString[0], 50, "frame time: %.2f ms  fps: %.2f", (double)frameTime, (double)fps);
        drawText(outputString, 20, 20, Z_HUD);

        // Swap buffers to display the current frame
        swapBuffers();
    }

    closeWindow();
}
