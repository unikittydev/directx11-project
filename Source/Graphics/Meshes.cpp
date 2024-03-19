#include "Meshes.h"
#include "ModelImporter.h"

Meshes* Meshes::s_instance = nullptr;

Meshes::Meshes()
{
    s_instance = this;
}

Meshes::~Meshes()
{
    for (const auto mesh : meshes)
        delete mesh;
}

Mesh* Meshes::GetEmpty()
{
    auto* mesh = new Mesh();
    s_instance->meshes.push_back(mesh);
    return mesh;
}
