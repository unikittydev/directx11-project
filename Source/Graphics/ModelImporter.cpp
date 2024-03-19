#include "ModelImporter.h"

#include <iostream>

#include "Meshes.h"
#include "Shaders.h"
#include "Graphics/Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

Mesh* ModelImporter::ConvertAiMesh(aiMesh* aMesh)
{
    std::vector<Vertex> vertices{};
    std::vector<UINT> indices{};
        
    for (uint i = 0; i < aMesh->mNumVertices; i++)
        vertices.push_back(Vertex{});
        
    for (uint i = 0; i < aMesh->mNumVertices; i++)
    {
        const aiVector3D aiVertex = aMesh->mVertices[i];
        float4 position = { aiVertex.x, aiVertex.y, aiVertex.z, 0.0f };

        vertices[i] = Vertex{ position };
    }
    for (uint i = 0; i < aMesh->mNumFaces; i++)
    {
        const aiFace face = aMesh->mFaces[i];

        for (uint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Mesh* mesh = Meshes::GetEmpty();
    mesh->SetVertices(vertices.data(), static_cast<UINT>(vertices.size()));
    mesh->SetIndices(indices.data(), static_cast<UINT>(indices.size()));
    mesh->SetShader(Shaders::Get(L"./Shaders/Test.hlsl", Position));
    return mesh;
}

int ModelImporter::ImportMeshes(const std::string& path, Mesh** meshes)
{
    Assimp::Importer importer;
        
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_OptimizeMeshes |
        aiProcess_JoinIdenticalVertices |
        aiProcess_CalcTangentSpace);

    if (scene == nullptr)
        std::cerr << "Cannot import mesh!" << std::endl;

    std::vector<Mesh*> m{};
    for (uint i = 0; i < scene->mNumMeshes; i++)
        m.push_back(ConvertAiMesh(scene->mMeshes[i]));

    meshes = m.data();
    return scene->mNumMeshes;
    return 0;
}