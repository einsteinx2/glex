#pragma once
#include "glex/common/gl.h"
#include "Texture.h"

class Cube {
public:
    Cube();
    ~Cube();
    Cube(const std::string texturePath);
    void draw();

    Texture* GetTexture();


private:
    GLfloat _anglex = 0.0;
    GLfloat _angley = 0.0;
    GLfloat _anglez = 0.0;
    Texture* _texture = nullptr;
    
    void _drawList();
    void _initTexture();
};
