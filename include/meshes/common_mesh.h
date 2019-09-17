#pragma once
#include <vector>

struct MeshData {
    int numVertices;
    std::vector<float> vertices;
    std::vector<float> textureCoordinates;
    std::vector<float> normals;
};