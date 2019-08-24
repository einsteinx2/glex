// This is a .h file for the model: monkeyTriangles.obj

// Positions: 507
// Texels: 556
// Normals: 942
// Faces: 967
// Vertices: 2901

#pragma once

struct MonkeyTrianglesMesh {
    const int numVertices;
    const float vertices[2901][3];
    const float textureCoordinates[2901][2];
    const float normals[2901][3];
};

extern struct MonkeyTrianglesMesh monkeyTrianglesMesh;
