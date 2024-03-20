#include "ModelImporter.h"

#include <iostream>

#include "Meshes.h"
#include "Shaders.h"
#include "Graphics/Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

Mesh* ModelImporter::ConvertAiMesh(const aiMesh* aMesh)
{
    std::vector<Vertex> vertices{};
    std::vector<UINT> indices{};
        
    for (uint i = 0; i < aMesh->mNumVertices; i++)
        vertices.push_back(Vertex{});
       

    if (aMesh->HasPositions())
        FillPositions(aMesh, vertices);
    if (aMesh->HasNormals())
        FillNormals(aMesh, vertices);
    if (aMesh->HasTangentsAndBitangents())
        FillTangents(aMesh, vertices);
    if (aMesh->HasTextureCoords(0))
        FillUVs(aMesh, vertices, 0);
    
    for (uint i = 0; i < aMesh->mNumFaces; i++)
    {
        const aiFace face = aMesh->mFaces[i];

        for (uint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Mesh* mesh = Meshes::GetEmpty();
    mesh->SetVertices(vertices.data(), static_cast<UINT>(vertices.size()));
    mesh->SetIndices(indices.data(), static_cast<UINT>(indices.size()));
    mesh->SetShader(Shaders::Get(L"./Shaders/Test.hlsl", Position | Normal | Tangent | UV0));
    return mesh;
}

void ModelImporter::FillPositions(const aiMesh* aMesh, std::vector<Vertex>& vertices)
{
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiVertex = aMesh->mVertices[i];
        const float4 position = { aiVertex.x, aiVertex.y, aiVertex.z, 0.0f };

        vertices[i].position = position;
    }
}

void ModelImporter::FillNormals(const aiMesh* aMesh, std::vector<Vertex>& vertices)
{
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiNormal = aMesh->mNormals[i];
        const float4 normal = { aiNormal.x, aiNormal.y, aiNormal.z, 0.0f };

        vertices[i].normal = normal;
    }
}

void ModelImporter::FillTangents(const aiMesh* aMesh, std::vector<Vertex>& vertices)
{
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiTangent = aMesh->mTangents[i];
        const float4 tangent = { aiTangent.x, aiTangent.y, aiTangent.z, 0.0f };

        vertices[i].tangent = tangent;
    }
}

void ModelImporter::FillUVs(const aiMesh* aMesh, std::vector<Vertex>& vertices, uint index)
{
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiUV = aMesh->mTextureCoords[0][i];
        const float4 uv = { aiUV.x, aiUV.y, aiUV.z, 0.0f };

        vertices[i].uv = uv;
    }
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

    std::vector<Mesh*> m{};
    for (uint i = 0; i < scene->mNumMeshes; i++)
        m.push_back(ConvertAiMesh(scene->mMeshes[i]));
    
    return std::move(m);
}