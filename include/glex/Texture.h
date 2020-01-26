#pragma once
#include "glex/common/gl.h"

#include <string>

class Texture {
public:
    GLuint id = 0;
    const int& width = _width;
    const int& height = _height;

    ~Texture();
    bool loadRGBA(std::string path);
    bool loadRGBA(GLsizei textureWidth, GLsizei textureHeight, const unsigned char* rgbaData);
    bool loadRGB(std::string path);
    bool loadRGB(GLsizei textureWidth, GLsizei textureHeight, const unsigned char* rgbData);
    bool loadExisting(GLsizei textureWidth, GLsizei textureHeight, GLuint textureId);
    void unload();
    bool isLoaded();

private:
    GLsizei _width = 0;
    GLsizei _height = 0;

    bool _loadTextureFromFile(std::string path, int numberOfColorComponents, bool flipVertically = true);
};
