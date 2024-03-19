#pragma once

#include <string>

#include "Core/Types.h"

class Mesh;
struct aiMesh;

class ModelImporter
{
private:
    struct Vertex
    {
        float4 position;
    };
    
    Mesh* ConvertAiMesh(aiMesh* aMesh);
public:
    int ImportMeshes(const std::string& path, Mesh** meshes);
};
