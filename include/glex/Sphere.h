#pragma once
#include "../common_gl.h"

class Sphere {
    public:
        Sphere(float radius);
        void draw();
    private:
        float _radius;
        void _drawFace(int p_recurse, GLfloat* a, GLfloat* b, GLfloat* c);
        void _drawList();
        // GLint _list;
        // GLfloat _anglex;
        // GLfloat _angley;
        // GLfloat _anglez;
};
