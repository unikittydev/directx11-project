#pragma once

#include <string>
#include <vector>
#include <assimp/matrix4x4.h>

#include "Core/Types.h"

struct Bounds;
struct aiNode;
struct aiScene;
class Mesh;
struct aiMesh;

class ModelImporter
{
public:
    struct ImportSettings
    {
        float scaleUnit = .005f;
    };
private:
    struct Vertex
    {
        float4 position;
        float4 normal;
        //float4 tangent;
        float4 uv;
    };

    static float3 D3Dfloat3(const aiVector3D& aiVector);
    static matrix D3Dmatrix(const aiMatrix4x4& aiMatrix);
    static float4 D3Dfloat4(const aiVector3D& aiVector);
    
    static void ProcessMeshes(const aiScene* scene, const aiNode* node, const aiMatrix4x4& parent, std::vector<Mesh*>& meshes, const ImportSettings& settings);
    static void ConvertAiMesh(const aiScene* scene, const aiMesh* aMesh, const matrix& transform, std::vector<Mesh*>& meshes, const ImportSettings& settings);
    
    static void FillPositions(const aiMesh* aMesh, const matrix& transform, std::vector<Vertex>& vertices, Bounds& bounds, const ImportSettings& settings);
    static void FillNormals(const aiMesh* aMesh, const matrix& transform, std::vector<Vertex>& vertices, const ImportSettings& settings);
    static void FillTangents(const aiMesh* aMesh, const matrix& transform, std::vector<Vertex>& vertices, const ImportSettings& settings);
    static void FillUVs(const aiMesh* aMesh, const matrix& transform, std::vector<Vertex>& vertices, uint index, const ImportSettings& settings);
    
public:
    static std::vector<Mesh*> ImportMeshes(const std::wstring& path, const ImportSettings& settings);
};
