#include "glex/Texture.h"
#include "debug_log.h"

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
    DEBUG_PRINTLN("Texture: bmp path: %s", path.c_str());
    std::ifstream inBMP(path, std::ifstream::binary);
    if (inBMP && !inBMP.good()) {
        DEBUG_PRINTLN("Texture: path could not be opened"); 
        return false;
    }

    // Read BMP header
    if (!inBMP.read(header, headerSize).good()) {
        DEBUG_PRINTLN("Texture: could not read BMP header"); 
        return false;
    }
    
    // Check if header contains valid BMP signature
    if (header[0] != 'B' || header[1] != 'M') {
        DEBUG_PRINTLN("Texture: not a correct BMP file");
        return false;
    }

    // Read ints from the byte array
    const int dataPosLoc   = 0x0A;
    const int imageSizeLoc = 0x22;
    const int widthLoc     = 0x12;
    const int heightLoc    = 0x16;
    dataPos      = header[dataPosLoc] + (header[dataPosLoc + 1] << 8) + (header[dataPosLoc + 2] << 16) + (header[dataPosLoc + 3] << 24);
    imageSize    = header[imageSizeLoc] + (header[imageSizeLoc + 1] << 8) + (header[imageSizeLoc + 2] << 16) + (header[imageSizeLoc + 3] << 24);
    headerWidth  = header[widthLoc] + (header[widthLoc + 1] << 8) + (header[widthLoc + 2] << 16) + (header[widthLoc + 3] << 24);
    headerHeight = header[heightLoc] + (header[heightLoc + 1] << 8) + (header[heightLoc + 2] << 16) + (header[heightLoc + 3] << 24);
    DEBUG_PRINTLN("Texture: BMP dataPos: %u  imageSize: %u", dataPos, imageSize);
    DEBUG_PRINTLN("Texture: BMP headerWidth: %u  headerHeight: %u", headerWidth, headerWidth);

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
            DEBUG_PRINTLN("Texture: unable to seek to BMP data position: %d", dataPos);
            return false;
        }
    }
    
    // Create a buffer
    bgrData = new char[imageSize];

    // Read the actual data from the file into the buffer
    inBMP.read(bgrData, imageSize);
    unsigned int actualSize = (unsigned int)inBMP.gcount();
    if (imageSize != actualSize) {
        DEBUG_PRINTLN("Texture: failed to read BMP file, imageSize: %u actualSize: %zu", imageSize, actualSize);
    }

    // Everything is in memory now, the file can be closed
    inBMP.close();

    // Create the OpenGL texture
    DEBUG_PRINTLN("headerWidth: %u  headerHeight: %u", headerWidth, headerHeight);
    loadBgrTexture(headerWidth, headerHeight, bgrData);

    return true;
}

void Texture::loadBgrTexture(GLsizei textureWidth, GLsizei textureHeight, char* bgrData) {
    DEBUG_PRINTLN("loadBgrTexture called");
    if (isTextureLoaded()) {
        unloadTexture();
    }

#ifdef DREAMCAST
    // TODO: Move this to GLdc
    // Convert BGR data to RGB in place
    GLsizei length = textureWidth * textureHeight * 3;
    char r, g, b; 
    for (int i = 0; i < length; i+=3) {
        r = bgrData[i+2];
        g = bgrData[i+1];
        b = bgrData[i];
        bgrData[i]   = r;
        bgrData[i+1] = g;
        bgrData[i+2] = b;
    }
#endif

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef DREAMCAST
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bgrData);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bgrData);
#endif
    DEBUG_PRINTLN("loadBgrTexture called glTexImage2D");

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