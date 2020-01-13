#include "glex/Texture.h"
#include "debug_log.h"

#include <iostream>
#include <fstream>

Texture::~Texture() {
    if (isTextureLoaded()) {
        unloadTexture();
    }
}

bool Texture::loadBmpTexture(std::string path) {
    // Data read from the header of the BMP file
    const unsigned int headerSize = 54;              // Each BMP file begins by a 54-bytes header
    char header[headerSize];          
    unsigned int dataPos;                   // Position in the file where the actual data begins
    unsigned int headerWidth, headerHeight;
    unsigned int imageSize;                 // = width * height * 3
    char* bgrData;

    // Open BMP file
    DEBUG_PRINTLN("bmp path: %s", path.c_str());
    std::ifstream inBMP(path, std::ios::binary);
    if (inBMP && !inBMP.good()) {
        DEBUG_PRINTLN("Image could not be opened"); 
        return false;
    }

    // Read BMP header
    if (!inBMP.read(header, headerSize).good()) {
        DEBUG_PRINTLN("Image could not be opened"); 
        return false;
    }
    
    // Check if header contains valid BMP signature
    if (header[0] != 'B' || header[1] != 'M') {
        DEBUG_PRINTLN("Not a correct BMP file");
        return false;
    }

    // Read ints from the byte array
    dataPos      = *(int*)&(header[0x0A]);
    imageSize    = *(int*)&(header[0x22]);
    headerWidth  = *(int*)&(header[0x12]);
    headerHeight = *(int*)&(header[0x16]);
    DEBUG_PRINTLN("BMP headerWidth: %u  headerHeight: %u", headerWidth, headerWidth);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) {
        // 3 : one byte for each Red, Green and Blue component
        imageSize = headerWidth * headerHeight * 3;
    }
    if (dataPos == 0) {
        // The BMP header is done that way
        dataPos = headerSize; 
    }

    // Seek to the data position in case it's not the default
    if (dataPos != headerSize) {
        inBMP.clear();
        if (inBMP.seekg(dataPos).good()) {
            DEBUG_PRINTLN("Unable to seek to BMP data position: %d", dataPos);
            return false;
        }
    }
    
    // Create a buffer
    bgrData = new char[imageSize];

    // Read the actual data from the file into the buffer
    inBMP.read(bgrData, imageSize);
    unsigned int actualSize = (unsigned int)inBMP.gcount();
    if (imageSize != actualSize) {
        DEBUG_PRINTLN("Failed to read BMP file, imageSize: %u actualSize: %zu", imageSize, actualSize);
    }

    // Everything is in memory now, the file can be closed
    inBMP.close();

    // Create the OpenGL texture
    DEBUG_PRINTLN("headerWidth: %u  headerHeight: %u", headerWidth, headerHeight);
    loadBgrTexture(headerWidth, headerHeight, bgrData);

    return true;
}

void Texture::loadBgrTexture(GLsizei textureWidth, GLsizei textureHeight, const char* bgrData) {
    if (isTextureLoaded()) {
        unloadTexture();
    }

#ifdef DREAMCAST
    // TODO: Move this to GLdc
    // Convert BGR data to RGB
    GLsizei length = textureWidth * textureHeight;
    char* data = new char[length];
    for (int i = 0; i < length; i+=3) {
        data[i]   = bgrData[i+2];
        data[i+1] = bgrData[i+1];
        data[i+2] = bgrData[i];
    }
#endif

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef DREAMCAST
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bgrData);
#endif

    _width = textureWidth;
    _height = textureHeight;
}

void Texture::loadRgbaTexture(GLsizei textureWidth, GLsizei textureHeight, const unsigned char* rgbaData) {
    if (isTextureLoaded()) {
        unloadTexture();
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);

    _width = textureWidth;
    _height = textureHeight;
}

void Texture::loadExistingTexture(GLsizei textureWidth, GLsizei textureHeight, GLuint textureId) {
    _width = textureWidth;
    _height = textureHeight;
    id = textureId;
}

void Texture::unloadTexture() {
    if (isTextureLoaded()) {
        glDeleteTextures(1, &id);
        id = 0;
    }
}

bool Texture::isTextureLoaded() {
    return id != 0;
}