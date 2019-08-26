#pragma once
#include "../common_gl.h"
#include "Texture.h"

class Sphere {
    public:
        Sphere(float radius);
        void draw();
    private:
        float _radius;
        GLfloat _anglex;
        GLfloat _angley;
        GLfloat _anglez;
        Texture _texture;
        void _drawFace(int p_recurse, GLfloat* a, GLfloat* b, GLfloat* c);
        void _drawList();      
};
