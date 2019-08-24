#pragma once
#include <vector>

using namespace std;
struct MeshData {
    const int numVertices;
    const vector<vector<float>> vertices;
    const vector<vector<float>> textureCoordinates;
    const vector<vector<float>> normals;
};