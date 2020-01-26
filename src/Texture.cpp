// To reduce footprint, only support JPEG, PNG, and BMP files
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#include "stb/stb_image.h"

#include "glex/Texture.h"
#include "glex/common/log.h"
#include "glex/common/path.h"

#include <fstream>
#include <cstdlib>

Texture::~Texture() {
    if (isLoaded()) {
        unload();
    }
}

bool Texture::_loadTextureFromFile(std::string path, int numberOfColorComponents, bool flipVertically) {
    if (numberOfColorComponents < 0 || numberOfColorComponents > 4) {
        DEBUG_PRINTLN("ERROR: numberOfColorComponents must be between 0 and 4");
        return false;
    }

    std::string platformPath = glex::targetPlatformPath(path);
    DEBUG_PRINTLN("Loading texture from path: %s  components: %d  flipVert: %d", platformPath.c_str(), numberOfColorComponents, flipVertically);

    // Load the image file as a texture
    int w, h, n;
    stbi_set_flip_vertically_on_load(flipVertically); // Flipping vertically matches OpenGL coordinate system
    const unsigned char *data = stbi_load(platformPath.c_str(), &w, &h, &n, numberOfColorComponents);
    if (data != NULL) {
        bool success = false;
        switch(numberOfColorComponents) {
        case STBI_rgb_alpha:
            success = loadRGBA(w, h, data);
            break;
        case STBI_rgb:
            success = loadRGB(w, h, data);
            break;
        }
        stbi_image_free((void *)data);
        return success;
    }

    ERROR_PRINTLN("ERROR: couldn't open texture image file");
    exit(EXIT_FAILURE);
    return false;
}

bool Texture::loadRGBA(std::string path) {
    return _loadTextureFromFile(path, STBI_rgb_alpha);
}

bool Texture::loadRGBA(GLsizei textureWidth, GLsizei textureHeight, const unsigned char* rgbaData) {
    if (isLoaded()) {
        unload();
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        ERROR_PRINT("Failed to load RGBA texture with GL error: %d", error);
        return false;
    }

    _width = textureWidth;
    _height = textureHeight;
    return true;
}

bool Texture::loadRGB(std::string path) {
    return _loadTextureFromFile(path, STBI_rgb);
}

bool Texture::loadRGB(GLsizei textureWidth, GLsizei textureHeight, const unsigned char* rgbData) {
    if (isLoaded()) {
        unload();
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbData);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        ERROR_PRINT("Failed to load RGBA texture with GL error: %d", error);
        return false;
    }

    _width = textureWidth;
    _height = textureHeight;
    return true;
}

bool Texture::loadExisting(GLsizei textureWidth, GLsizei textureHeight, GLuint textureId) {
    _width = textureWidth;
    _height = textureHeight;
    id = textureId;

    // TODO: Use glAreTexturesResident to check if the texture actually exists, for now always return true
    return true;
}

void Texture::unload() {
    if (isLoaded()) {
        glDeleteTextures(1, &id);
        id = 0;
    }
}

bool Texture::isLoaded() {
    return id != 0;
}