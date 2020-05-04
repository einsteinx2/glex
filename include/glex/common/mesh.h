#pragma once
#include <vector>

struct MeshData {
    size_t numVertices = 0;
    std::vector<float> vertices;
    std::vector<float> textureCoordinates;
    std::vector<float> normals;
};