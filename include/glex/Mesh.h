#pragma once
#include "../common_gl.h"
#include "../meshes/common_mesh.h"
#include "Texture.h"

class Mesh {
public:
    GLfloat rotationX = 0.0;
    GLfloat rotationY = 0.0;
    GLfloat rotationZ = 0.0;
    GLfloat scale = 0.0;

    Mesh(MeshData* meshData, Texture* texture, GLfloat scale = 1.0);
    void draw();

private:
    MeshData* _meshData;
    Texture* _texture;
    GLfloat _scale = 1.0;
    
    void _drawList();
};
