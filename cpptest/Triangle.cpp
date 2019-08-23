#include "Triangle.h"
#include "../debug_log.h"
#include <stdio.h>

Triangle::Triangle() {
    _list = glGenLists(1);
    glNewList(_list, GL_COMPILE);
    
    // Create the triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f( 0.0,  1.0, 0.0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f( 1.0, -1.0, 0.0);
    glEnd();

    glEndList();
}

void Triangle::draw() {
    glPushMatrix();
    glScalef(5.0, 5.0, 0.0);
    
    glCallList(_list);
    glPopMatrix();
}