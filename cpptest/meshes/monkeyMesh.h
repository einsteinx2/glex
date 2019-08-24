// This is a .h file for the model: monkey.obj

// Positions: 507
// Texels: 556
// Normals: 499
// Faces: 500
// Vertices: 1500

#pragma once

struct MonkeyMesh {
    const int numVertices;
    const float vertices[1500][3];
    const float textureCoordinates[1500][2];
    const float normals[1500][3];
};

extern struct MonkeyMesh monkeyMesh;
