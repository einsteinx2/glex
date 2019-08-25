#pragma once
#include "../common_gl.h"

#include <string>

class Texture {
    public:
        GLuint textureId;
        GLsizei textureWidth;
        GLsizei textureHeight;
        ~Texture();
        #ifndef DREAMCAST
        bool loadBmpTexture(GLsizei textureWidth_, GLsizei textureHeight_, std::string path);
        void loadBrgTexture(GLsizei textureWidth_, GLsizei textureHeight_, const unsigned char* bgrData);
        #endif
        void loadRgbaTexture(GLsizei textureWidth_, GLsizei textureHeight_, const unsigned char* rgbaData);
        void unloadTexture();
        bool isTextureLoaded();
};
