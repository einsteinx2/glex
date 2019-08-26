#include "glex/Sphere.h"
#include "debug_log.h"
#include "glex/Texture.h"

#include <cmath>

Sphere::Sphere(float radius) {
    _radius = radius;

    #ifndef DREAMCAST
    _texture.loadBmpTexture(256, 256, "examples/playground/worldmap1.bmp");
    #endif
}

void Sphere::_drawList() {
   GLfloat a[] = {1, 0, 0};
   GLfloat b[] = {0, 0, -1};
   GLfloat c[] = {-1, 0, 0};
   GLfloat d[] = {0, 0, 1};
   GLfloat e[] = {0, 1, 0};
   GLfloat f[] = {0, -1, 0};

   int recurse = 5;
   _drawFace(recurse, d, a, e);
   _drawFace(recurse, a, b, e);
   _drawFace(recurse, b, c, e);
   _drawFace(recurse, c, d, e);
   _drawFace(recurse, a, d, f);
   _drawFace(recurse, b, a, f);
   _drawFace(recurse, c, b, f);
   _drawFace(recurse, d, c, f);
}

void Sphere::draw() {
   #ifndef DREAMCAST
   // Enable texture
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, _texture.textureId);
   #endif

   // Enable depth test
   glEnable(GL_DEPTH_TEST);
   // Accept fragment if it closer to the camera than the former one
   glDepthFunc(GL_LESS);

   glPushMatrix();
   //glRotatef(_anglez, 0.0f, 0.0f, 1.0f);
   glRotatef(_angley, 0.0f, 1.0f, 0.0f);
   //glRotatef(_anglex, 1.0f, 0.0f, 0.0f);
   
   glScalef(3.0, 3.0, 3.0);
   
   _drawList();
   glPopMatrix();

   //_anglez += 0.75;
   _angley += 1.0;
   //_anglex += 0.75;

   #ifndef DREAMCAST
   glDisable(GL_TEXTURE_2D);
   #endif
}

inline void normalize(GLfloat *v) {
   GLfloat len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] /= len;
   v[1] /= len;
   v[2] /= len;
}

void Sphere::_drawFace(int recurse, GLfloat* a, GLfloat* b, GLfloat* c) {
   if (recurse > 1) {
      // Compute vectors halfway between the passed vectors
      GLfloat d[3] = {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
      GLfloat e[3] = {b[0] + c[0], b[1] + c[1], b[2] + c[2]};
      GLfloat f[3] = {c[0] + a[0], c[1] + a[1], c[2] + a[2]};

      normalize(d);
      normalize(e);
      normalize(f);

      _drawFace(recurse - 1, a, d, f);
      _drawFace(recurse - 1, d, b, e);
      _drawFace(recurse - 1, f, e, c);
      _drawFace(recurse - 1, f, d, e);
   }

   glBegin(GL_TRIANGLES);
   #ifdef DREAMCAST
   glNormal3fv(a);
   glVertex3f(a[0] * _radius, a[1] * _radius, a[2] * _radius);

   glNormal3fv(b);
   glVertex3f(b[0] * _radius, b[1] * _radius, b[2] * _radius);

   glNormal3fv(c);
   glVertex3f(c[0] * _radius, c[1] * _radius, c[2] * _radius);
   glEnd();
   #else
   float tx1 = atan2(a[0], a[2]) / (2. * M_PI) + 0.5;
   float ty1 = asin(a[1]) / M_PI + .5;
   glTexCoord2f(tx1, ty1);
   glNormal3fv(a);
   glVertex3f(a[0] * _radius, a[1] * _radius, a[2] * _radius);

   float tx2 = atan2(b[0], b[2]) / (2. * M_PI) + 0.5; 
   float ty2 = asin(b[1]) / M_PI + .5;
   if (tx2 < 0.75 && tx1 > 0.75)
      tx2 += 1.0;
   else if (tx2 > 0.75 && tx1 < 0.75)
      tx2 -= 1.0;
   glTexCoord2f(tx2, ty2);   
   glNormal3fv(b);
   glVertex3f(b[0] * _radius, b[1] * _radius, b[2] * _radius);

   float tx3 = atan2(c[0], c[2]) / (2. * M_PI) + 0.5; 
   float ty3 = asin(c[1]) / M_PI + .5;
   if (tx3 < 0.75 && tx2 > 0.75)
      tx3 += 1.0;
   else if (tx3 > 0.75 && tx2 < 0.75)
      tx3 -= 1.0;
   glTexCoord2f(tx3, ty3);
   glNormal3fv(c);
   glVertex3f(c[0] * _radius, c[1] * _radius, c[2] * _radius);
   glEnd();
   #endif
}
