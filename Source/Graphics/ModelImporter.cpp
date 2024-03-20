#include "ModelImporter.h"

#include <iostream>

#include "Meshes.h"
#include "Shaders.h"
#include "Graphics/Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

float3 ModelImporter::D3Dfloat3(const aiVector3D& aiVector)
{
    return float3{ aiVector.x, aiVector.y, aiVector.z };
}

float4 ModelImporter::D3Dfloat4(const aiVector3D& aiVector)
{
    return float4{ aiVector.x, aiVector.y, aiVector.z, 0.0f };
}

matrix ModelImporter::D3Dmatrix(const aiMatrix4x4& aiMatrix)
{
    auto mat = aiMatrix;
    return matrix
    {
       	mat.a1, mat.b1, mat.c1, mat.d1,
       	mat.a2, mat.b2, mat.c2, mat.d2,
       	mat.a3, mat.b3, mat.c3, mat.d3,
       	mat.a4, mat.b4, mat.c4, mat.d4,
    };
}

void ModelImporter::FillPositions(const aiMesh* aMesh, const matrix& transform, std::vector<Vertex>& vertices)
{
    // Idk why this works
    float3 p;
    float3 scale;
    quaternion rot;
    
    auto mat = transform;
    mat.Decompose(scale, rot, p);

    const float scaleFactorScalar = .1f;
    const float4 scaleFactor = float4{ scaleFactorScalar, scaleFactorScalar, scaleFactorScalar, 1.0f };
    
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiVertex = aMesh->mVertices[i];
        float4 position =  D3Dfloat4(aiVertex);
        float4::Transform(position, transform, position);
        
        vertices[i].position = position;
        vertices[i].position *= scaleFactor;
    }
}

void ModelImporter::FillNormals(const aiMesh* aMesh, const matrix& transform, std::vector<Vertex>& vertices)
{    
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiNormal = aMesh->mNormals[i];
        
        float3 normal = { aiNormal.x, aiNormal.y, aiNormal.z };
        float3::TransformNormal(normal, transform, normal);
        normal.Normalize();

        vertices[i].normal = float4 { normal.x, normal.y, normal.z, 0.0f };
    }
}

void ModelImporter::FillTangents(const aiMesh* aMesh, const matrix& transform, std::vector<Vertex>& vertices)
{
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiTangent = aMesh->mTangents[i];
        const float4 tangent = { aiTangent.x, aiTangent.y, aiTangent.z, 0.0f };

        //vertices[i].tangent = tangent;
    }
}

void ModelImporter::FillUVs(const aiMesh* aMesh, const matrix& transform, std::vector<Vertex>& vertices, uint index)
{
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiUV = aMesh->mTextureCoords[0][i];
        const float4 uv = { aiUV.x, 1.0f - aiUV.y, aiUV.z, 0.0f };

        vertices[i].uv = uv;
    }
}

void ModelImporter::ProcessMeshes(const aiScene* scene, const aiNode* node, const aiMatrix4x4& parent, std::vector<Mesh*>& meshes)
{
    const aiMatrix4x4& transform = parent * node->mTransformation;
    const matrix& d3dTransform = D3Dmatrix(transform);

    if (node->mNumMeshes > 0)
        for (uint i = 0; i < node->mNumMeshes; ++i)
        {
            const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ConvertAiMesh(scene, mesh, d3dTransform, meshes);
        }

    for (uint i = 0; i < node->mNumChildren; i++)
        ProcessMeshes(scene, node->mChildren[i], transform, meshes);
}

void ModelImporter::ConvertAiMesh(const aiScene* scene, const aiMesh* aMesh, const matrix& transform, std::vector<Mesh*>& meshes)
{
    std::vector<ModelImporter::Vertex> vertices{};
    std::vector<UINT> indices{};
        
    for (uint i = 0; i < aMesh->mNumVertices; i++)
        vertices.push_back(Vertex{});
    
    if (aMesh->HasPositions())
        FillPositions(aMesh, transform, vertices);
    if (aMesh->HasNormals())
        FillNormals(aMesh, transform, vertices);
    //if (aMesh->HasTangentsAndBitangents())
    //    FillTangents(aMesh, vertices);
    if (aMesh->HasTextureCoords(0))
        FillUVs(aMesh, transform, vertices, 0);
    
    for (uint i = 0; i < aMesh->mNumFaces; i++)
    {
        const aiFace face = aMesh->mFaces[i];

        for (uint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Mesh* mesh = Meshes::GetEmpty();
    mesh->SetVertices(vertices.data(), static_cast<uint>(vertices.size()));
    mesh->SetIndices(indices.data(), static_cast<uint>(indices.size()));
    mesh->SetShader(Shaders::Get(L"./Shaders/Test.hlsl", Position | Normal | UV0));

    meshes.push_back(mesh);
}

std::vector<Mesh*> ModelImporter::ImportMeshes(const std::string& path)
{
    Assimp::Importer importer;
        
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_OptimizeMeshes |
        aiProcess_JoinIdenticalVertices |
        aiProcess_CalcTangentSpace);
    
    if (scene == nullptr)
        std::cerr << importer.GetErrorString() << std::endl;
    
    std::vector<Mesh*> meshes{};
    
    ProcessMeshes(scene, scene->mRootNode, scene->mRootNode->mTransformation, meshes);

    return std::move(meshes);
}