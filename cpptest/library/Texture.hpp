#pragma once
#include "../../common_gl.h"

#include <string>

class Texture {
    public:
        GLuint textureId;
        GLsizei textureWidth;
        GLsizei textureHeight;
        ~Texture();
        bool loadBmpTexture(GLsizei textureWidth_, GLsizei textureHeight_, std::string path);
        void loadBrgTexture(GLsizei textureWidth_, GLsizei textureHeight_, const unsigned char* bgrData);
        void loadRgbaTexture(GLsizei textureWidth_, GLsizei textureHeight_, const unsigned char* rgbaData);
        void unloadTexture();
        bool isTextureLoaded();
};