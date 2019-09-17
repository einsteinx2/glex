#pragma once
#include "../meshes/common_mesh.h"

#include <string>

struct MeshObjInfo {
    int vertices;
    int positions;
    int texels;
    int normals;
    int faces;
};

class MeshLoader {
    public:
        static MeshData* loadObjMesh(std::string path);
    private:
        static MeshObjInfo _getObjInfo(std::string path);
        static MeshData* _readObjFile(std::string path, MeshObjInfo info);
};