#pragma once
#include "glex/common/mesh.h"

#include <string>

struct MeshObjInfo {
    int vertices  = 0;
    int positions = 0;
    int texels    = 0;
    int normals   = 0;
    int faces     = 0;
};

class MeshLoader {
public:
    static MeshData* loadObjMesh(std::string path);

private:
    static MeshObjInfo _getObjInfo(std::string path);
    static MeshData* _readObjFile(std::string path, MeshObjInfo info);
};