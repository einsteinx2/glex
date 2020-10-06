#include "glex/graphics/Mesh.h"
#include "glex/common/log.h"

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
    
    //glTranslatef(1.0, -2.0, 1.0);
    
    _drawList();
    glPopMatrix();
}

void Mesh::_drawList() {
    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _texture->id);

    // Draw the mesh
    glEnableClientState(GL_VERTEX_ARRAY); //enable vertex array
    glEnableClientState(GL_NORMAL_ARRAY); //enable normal array
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); //enable texcoord array

    glVertexPointer(3, GL_FLOAT, 0, &_meshData->vertices[0]); //give vertex array to OGL
    glTexCoordPointer(2, GL_FLOAT, 0, &_meshData->textureCoordinates[0]); //same with texcoord array
    glNormalPointer(GL_FLOAT, 0, &_meshData->normals[0]); //and normal array

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_meshData->numVertices);

    glDisableClientState(GL_VERTEX_ARRAY); //disable the client states again
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);
}