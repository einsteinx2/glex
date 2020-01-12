#include "glex/Mesh.h"
#include "debug_log.h"

#define IMMEDIATE_MODE false

Mesh::Mesh(MeshData* meshData, Texture* texture, GLfloat scale) {
    _meshData = meshData;
    _texture = texture;
    _scale = scale;
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
    rotationZ += 0.75;
    rotationY += 0.75;
    rotationX += 0.75;
    
    //glTranslatef(1.0, -2.0, 1.0);
    
    _drawList();
    glPopMatrix();
}

void Mesh::_drawList() {
    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _texture->textureId);

    // Draw the mesh
#if IMMEDIATE_MODE
    glBegin(GL_TRIANGLES); 
    int c = 0;
    int t = 0;
    for (int i = 0; i < _meshData->numVertices; i+=1) {
        glNormal3f(_meshData->normals[c], _meshData->normals[c+1], _meshData->normals[c+2]);
        glTexCoord2f(_meshData->textureCoordinates[t], _meshData->textureCoordinates[t+1]);
        glVertex3f(_meshData->vertices[c], _meshData->vertices[c+1], _meshData->vertices[c+2]);
        c += 3;
        t += 2;
    }
    glEnd();
#else
    glEnableClientState(GL_VERTEX_ARRAY); //enable vertex array
    glEnableClientState(GL_NORMAL_ARRAY); //enable normal array
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); //enable texcoord array

    glVertexPointer(3, GL_FLOAT, 0, &_meshData->vertices[0]); //give vertex array to OGL
    glTexCoordPointer(2, GL_FLOAT, 0, &_meshData->textureCoordinates[0]); //same with texcoord array
    glNormalPointer(GL_FLOAT, 0, &_meshData->normals[0]); //and normal array

    glDrawArrays(GL_TRIANGLES, 0, _meshData->numVertices);

    glDisableClientState(GL_VERTEX_ARRAY); //disable the client states again
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

    glDisable(GL_TEXTURE_2D);
}