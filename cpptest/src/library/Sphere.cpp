#include "Sphere.hpp"
#include "../debug_log.h"
#include "Texture.hpp"

#include <cmath>

Sphere::Sphere(double radius) {
    _radius = radius;
    _list = glGenLists(1);
    glNewList(_list, GL_COMPILE);

    GLdouble a[] = {1, 0, 0};
    GLdouble b[] = {0, 0, -1};
    GLdouble c[] = {-1, 0, 0};
    GLdouble d[] = {0, 0, 1};
    GLdouble e[] = {0, 1, 0};
    GLdouble f[] = {0, -1, 0};

    int recurse = 5;
    _drawFace(recurse, d, a, e);
    _drawFace(recurse, a, b, e);
    _drawFace(recurse, b, c, e);
    _drawFace(recurse, c, d, e);
    _drawFace(recurse, a, d, f);
    _drawFace(recurse, b, a, f);
    _drawFace(recurse, c, b, f);
    _drawFace(recurse, d, c, f);

    glEndList();
}

void Sphere::draw() {
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glPushMatrix();
    //glRotatef(_anglez, 0.0f, 0.0f, 1.0f);
    //glRotatef(_angley, 0.0f, 1.0f, 0.0f);
    //glRotatef(_anglex, 1.0f, 0.0f, 0.0f);
    
    glScalef(3.0, 3.0, 3.0);
    
    glCallList(_list);
    glPopMatrix();

    //_anglez += 0.75;
    //_angley += 1.0;
    //_anglex += 0.75;
}

inline void normalize(GLdouble *v) {
   GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] /= len;
   v[1] /= len;
   v[2] /= len;
}

void Sphere::_drawFace(int recurse, GLdouble* a, GLdouble* b, GLdouble* c) {
   if (recurse > 1) {
      // Compute vectors halfway between the passed vectors
      GLdouble d[3] = {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
      GLdouble e[3] = {b[0] + c[0], b[1] + c[1], b[2] + c[2]};
      GLdouble f[3] = {c[0] + a[0], c[1] + a[1], c[2] + a[2]};

      normalize(d);
      normalize(e);
      normalize(f);

      _drawFace(recurse - 1, a, d, f);
      _drawFace(recurse - 1, d, b, e);
      _drawFace(recurse - 1, f, e, c);
      _drawFace(recurse - 1, f, d, e);
   }

   glBegin(GL_TRIANGLES);
      glNormal3dv(a);
      glVertex3d(a[0] * _radius, a[1] * _radius, a[2] * _radius);

      glNormal3dv(b);
      glVertex3d(b[0] * _radius, b[1] * _radius, b[2] * _radius);

      glNormal3dv(c);
      glVertex3d(c[0] * _radius, c[1] * _radius, c[2] * _radius);
   glEnd();
}
