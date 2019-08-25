#pragma once
#include "../common_gl.h"
#include "../meshes/common_mesh.h"
#include "Texture.h"

class Mesh {
    public:
        GLfloat rotationX;
        GLfloat rotationY;
        GLfloat rotationZ;
        GLfloat scale;
        Mesh(MeshData* meshData, Texture* texture, GLfloat scale = 1.0);
        void draw();
    private:
        MeshData* _meshData;
        Texture* _texture;
        GLfloat _scale;
        void _drawList();
};
