#include "Cube.h"
#include "debug_log.h"
#include <stdio.h>

Cube::Cube() {
    DEBUG_WRITELN("Cube constructor called");

    _list = glGenLists(1);
    glNewList(_list, GL_COMPILE);
    DEBUG_WRITELN("_list: %d", _list);
    
    // // Create the triangle
    // glBegin(GL_TRIANGLES);
    // glColor3f(1.0, 0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
    // glColor3f(0.0, 1.0, 0.0); glVertex3f( 0.0,  1.0, 0.0);
    // glColor3f(0.0, 0.0, 1.0); glVertex3f( 1.0, -1.0, 0.0);
    // glEnd();

    // cube ///////////////////////////////////////////////////////////////////////
    //    v7----- v4
    //   /|      /|
    //  v3------v0|
    //  | |     | |
    //  | |v6---|-|v5
    //  |/      |/
    //  v2------v1


    GLfloat v0[3] = {  1.0f,  1.0f,  1.0f };
    GLfloat v1[3] = {  1.0f, -1.0f,  1.0f };
    GLfloat v2[3] = { -1.0f, -1.0f,  1.0f };
    GLfloat v3[3] = { -1.0f,  1.0f,  1.0f };
    GLfloat v4[3] = {  1.0f,  1.0f, -1.0f };
    GLfloat v5[3] = {  1.0f, -1.0f, -1.0f };
    GLfloat v6[3] = { -1.0f, -1.0f, -1.0f };
    GLfloat v7[3] = { -1.0f,  1.0f, -1.0f };

    glBegin(GL_QUADS);  // draw a cube with 6 quads

    glColor3f(1.0, 0.0, 0.0); 
    glVertex3fv(v0);    // front face
    glVertex3fv(v3);
    glVertex3fv(v2);
    glVertex3fv(v1);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3fv(v4);    // top face
    glVertex3fv(v7);
    glVertex3fv(v3);
    glVertex3fv(v0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3fv(v3);    // left face
    glVertex3fv(v7);
    glVertex3fv(v6);
    glVertex3fv(v2);

    glColor3f(1.0, 0.0, 0.0); 
    glVertex3fv(v4);    // back face
    glVertex3fv(v7);
    glVertex3fv(v6);
    glVertex3fv(v5);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3fv(v4);    // right face
    glVertex3fv(v0);
    glVertex3fv(v1);
    glVertex3fv(v5);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3fv(v5);    // bottom face
    glVertex3fv(v6);
    glVertex3fv(v2);
    glVertex3fv(v1);
    
    glEnd();

    glEndList();
}

void Cube::draw() {
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glPushMatrix();
    glRotatef(_anglez, 0.0f, 0.0f, 0.5f);
    glRotatef(_angley, 0.0f, 1.0f, 0.0f);
    glRotatef(_anglex, 1.0f, 0.0f, 0.0f);
    
    
    glScalef(5.0, 5.0, 5.0);
    
    glCallList(_list);
    glPopMatrix();

    _anglez += 0.5;
    _angley -= 0.5;
    _anglex += 0.5;
}