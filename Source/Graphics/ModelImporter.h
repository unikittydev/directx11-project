#pragma once

#include <string>
#include <vector>

#include "Core/Types.h"

class Mesh;
struct aiMesh;

class ModelImporter
{
private:
    struct Vertex
    {
        float4 position;
        float4 normal;
        float4 tangent;
        float4 uv;
    };
    
    static Mesh* ConvertAiMesh(const aiMesh* aMesh);

    static void FillPositions(const aiMesh* aMesh, std::vector<Vertex>& vertices);
    static void FillNormals(const aiMesh* aMesh, std::vector<Vertex>& vertices);
    static void FillTangents(const aiMesh* aMesh, std::vector<Vertex>& vertices);
    static void FillUVs(const aiMesh* aMesh, std::vector<Vertex>& vertices, uint index);
    
public:
    static std::vector<Mesh*> ImportMeshes(const std::string& path);
};
