#pragma once
#include "../../common_gl.h"

class Sphere {
    public:
        Sphere(double radius);
        void draw();
    private:
        double _radius;
        GLint _list;
        void _drawFace(int p_recurse, GLdouble* a, GLdouble* b, GLdouble* c);
        // GLint _list;
        // GLfloat _anglex;
        // GLfloat _angley;
        // GLfloat _anglez;
};