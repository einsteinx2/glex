#include "Mesh.h"
#include "../debug_log.h"

#include "meshes/cubeMesh.h"
#include "textures/cubeMesh_RGBA_512x512.h"
#include <stdio.h>

Mesh::Mesh() {
    _texture.loadRgbaTexture(512, 512, cubeMesh_RGBA_512x512);

    _list = glGenLists(1);
    glNewList(_list, GL_COMPILE);

    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _texture.textureId);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < cubeMesh.numVertices; i++) {
        glColor3f(1.0, 1.0, 1.0);
        glNormal3fv(cubeMesh.normals[i]);
        glTexCoord2fv(cubeMesh.textureCoordinates[i]);
        glVertex3fv(cubeMesh.vertices[i]);
    }
    glEnd();

    glEndList();
}

void Mesh::draw() {
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glPushMatrix();
    glRotatef(_anglez, 0.0f, 0.0f, 1.0f);
    //glRotatef(_angley, 0.0f, 1.0f, 0.0f);
    glRotatef(_anglex, 1.0f, 0.0f, 0.0f);
    
    glScalef(3.0, 3.0, 3.0);
    
    glCallList(_list);
    glPopMatrix();

    _anglez += 0.75;
    _angley += 1.0;
    _anglex += 0.75;
}