#include "Mesh.hpp"
#include "../debug_log.h"

Mesh::Mesh(MeshData* meshData, Texture* texture, GLfloat scale) {
    _meshData = meshData;
    _texture = texture;
    _scale = scale;

    _list = glGenLists(1);
    glNewList(_list, GL_COMPILE);

    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _texture->textureId);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < meshData->numVertices; i+=1) {
        glColor3f(1.0, 1.0, 1.0);
        glNormal3f(meshData->normals[i][0], meshData->normals[i][1], meshData->normals[i][2]);
        glTexCoord2f(meshData->textureCoordinates[i][0], meshData->textureCoordinates[i][1]);
        glVertex3f(meshData->vertices[i][0], meshData->vertices[i][1], meshData->vertices[i][2]);
        //glNormal3fv(meshData->normals[i]);
        //glTexCoord2fv(meshData->textureCoordinates[i]);
        //glVertex3fv(meshData->vertices[i]);
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
    //glEnable(GL_LIGHTING); 
    //glEnable(GL_LIGHT0);
    //GLfloat lightpos0[] = {1., 1., 1., 0.}; 
    //glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
    //glEnable(GL_LIGHT1);
    // GLfloat lightpos1[] = {-1., 1., 1., 0.}; 
    // glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glPushMatrix();

    glScalef(_scale, _scale, _scale);

    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    // rotationZ += 0.75;
    // rotationY += 0.75;
    // rotationX += 0.75;
    
    //glTranslatef(1.0, -2.0, 1.0);
    
    glCallList(_list);
    glPopMatrix();
}