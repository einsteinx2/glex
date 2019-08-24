#include "Mesh.h"
#include "../debug_log.h"

//#include "meshes/cubeMesh.h"
//#include "textures/cubeMesh_RGBA_512x512.h"
//#include "meshes/rockMesh.h"
//#include "textures/rockMesh_RGBA_512x512.h"
#include "meshes/houseFilledTrianglesMesh.h"
#include "textures/houseMesh_RGBA_512x512.h"
//#include "meshes/monkeyTrianglesMesh.h"

#include <stdio.h>

Mesh::Mesh() {
    _texture.loadRgbaTexture(512, 512, houseMesh_RGBA_512x512);

    _list = glGenLists(1);
    glNewList(_list, GL_COMPILE);

    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _texture.textureId);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < houseFilledTrianglesMesh.numVertices; i+=1) {
        glColor3f(1.0, 1.0, 1.0);
        glNormal3fv(houseFilledTrianglesMesh.normals[i]);
        glTexCoord2fv(houseFilledTrianglesMesh.textureCoordinates[i]);
        glVertex3fv(houseFilledTrianglesMesh.vertices[i]);
    }
    glEnd();

    glEndList();
}

void Mesh::draw() {
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Cull backfacing polygons 
    glCullFace(GL_BACK); 
    glEnable(GL_CULL_FACE);
    // Enable lighting
    // glEnable(GL_LIGHTING); 
    // glEnable(GL_LIGHT0);
    // GLfloat lightpos0[] = {0., 0., 1., 0.}; 
    // glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
    //glEnable(GL_LIGHT1);
    // GLfloat lightpos1[] = {-1., 1., 1., 0.}; 
    // glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glPushMatrix();
    //glTranslatef(1.0, -2.0, 1.0);

    glRotatef(anglez, 0.0f, 0.0f, 1.0f);
    glRotatef(angley, 0.0f, 1.0f, 0.0f);
    glRotatef(anglex, 1.0f, 0.0f, 0.0f);
    
    //glScalef(2.0, 2.0, 2.0);
    glScalef(0.3, 0.3, 0.3);
    
    glCallList(_list);
    glPopMatrix();

    anglez += 0.75;
    angley += 0.75;
    anglex += 0.75;
}