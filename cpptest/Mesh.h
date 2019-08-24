#pragma once
#include "../common_gl.h"
#include "Texture.h"

class Mesh {
    public:
        GLfloat anglex;
        GLfloat angley;
        GLfloat anglez;
        Mesh();
        void draw();
    private:
        GLint _list;
        Texture _texture;
};