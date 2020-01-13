#pragma once
#include "../common_gl.h"

#include <string>

class Texture {
public:
    GLuint id = 0;
    const int& width = _width;
    const int& height = _height;

    ~Texture();
    bool loadBmpTexture(std::string path);
    void loadBgrTexture(GLsizei textureWidth, GLsizei textureHeight, const char* bgrData);
    void loadRgbaTexture(GLsizei textureWidth, GLsizei textureHeight, const unsigned char* rgbaData);
    void loadExistingTexture(GLsizei textureWidth, GLsizei textureHeight, GLuint textureId);
    void unloadTexture();
    bool isTextureLoaded();

private:
    GLsizei _width = 0;
    GLsizei _height = 0;
};
