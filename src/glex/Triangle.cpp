#include "glex/Triangle.h"
#include "debug_log.h"

void Triangle::_drawList() {
    // Create the triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f( 0.0,  1.0, 0.0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f( 1.0, -1.0, 0.0);
    glEnd();
}

void Triangle::draw() {
    glPushMatrix();
    glScalef(5.0, 5.0, 0.0);
    
    _drawList();
    glPopMatrix();
}
