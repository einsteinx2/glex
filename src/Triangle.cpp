#include "glex/Triangle.h"
#include "glex/common/log.h"

#define IMMEDIATE_MODE false

void Triangle::_drawList() {
    // Create the triangle
    #if IMMEDIATE_MODE
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f( 0.0,  1.0, 0.0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f( 1.0, -1.0, 0.0);
    glEnd();
    
    #else
    float colors[] { 1.0, 0.0, 0.0,
                     0.0, 1.0, 0.0,
                     0.0, 0.0, 1.0 };
    float verts[] = { -1.0, -1.0, 0.0,
                       0.0,  1.0, 0.0,
                       1.0, -1.0, 0.0 };
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, verts);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    #endif
}

void Triangle::draw() {
    glPushMatrix();
    glScalef(5.0, 5.0, 0.0);
    
    _drawList();
    glPopMatrix();
}
