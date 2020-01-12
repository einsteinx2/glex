#include "glex/Texture.h"
#include "debug_log.h"

#include <iostream>
#include <fstream>

Texture::~Texture() {
    if (isTextureLoaded()) {
        unloadTexture();
    }
}

bool Texture::isTextureLoaded() {
    return textureId != 0;
}

#ifndef DREAMCAST
bool Texture::loadBmpTexture(GLsizei textureWidth_, GLsizei textureHeight_, std::string path) {
    // Data read from the header of the BMP file
    char header[54];          // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width * height * 3
    char* bgrData;

    // Open BMP file
    DEBUG_PRINTLN("bmp path: %s", path.c_str());
    std::ifstream inBMP(path, std::ios::binary);
    if (inBMP && !inBMP.good()) {
        DEBUG_PRINTLN("Image could not be opened"); 
        return false;
    }

    // Read BMP header
    if (!inBMP.read(header, 54).good()) {
        DEBUG_PRINTLN("Image could not be opened"); 
        return false;
    }
    
    // Check if header contains valid BMP signature
    if (header[0] != 'B' || header[1] != 'M') {
        DEBUG_PRINTLN("Not a correct BMP file");
        return false;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) {
        // 3 : one byte for each Red, Green and Blue component
        imageSize = width * height * 3;
    }
    if (dataPos == 0) {
        // The BMP header is done that way
        dataPos = 54; 
    }  

    // Create a buffer
    bgrData = new char[imageSize];

    // Read the actual data from the file into the buffer
    inBMP.read(bgrData, imageSize);
    std::streamsize actualSize = inBMP.gcount();
    if (imageSize != actualSize) {
        DEBUG_PRINTLN("Failed to read BMP file, imageSize: %u actualSize: %zu", imageSize, actualSize);
    }

    // Everything is in memory now, the file can be closed
    inBMP.close();

    // Create the OpenGL texture
    loadBrgTexture(textureWidth_, textureHeight_, bgrData);

    return true;
}

void Texture::loadBrgTexture(GLsizei textureWidth_, GLsizei textureHeight_, const char* bgrData) {
    if (isTextureLoaded()) {
        unloadTexture();
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth_, textureHeight_, 0, GL_BGR, GL_UNSIGNED_BYTE, bgrData);

    textureWidth = textureWidth_;
    textureHeight = textureHeight_;
}
#endif

void Texture::loadRgbaTexture(GLsizei textureWidth_, GLsizei textureHeight_, const unsigned char* rgbaData) {
    if (isTextureLoaded()) {
        unloadTexture();
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth_, textureHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);

    textureWidth = textureWidth_;
    textureHeight = textureHeight_;
}

void Texture::unloadTexture() {
    if (isTextureLoaded()) {
        glDeleteTextures(1, &textureId);
        textureId = 0;
    }
}
