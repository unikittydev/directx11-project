#include "Meshes.h"

Meshes* Meshes::s_instance = nullptr;

Meshes::Meshes() : meshes{}
{
    s_instance = this;
}

Meshes::~Meshes()
{
}

Mesh* Meshes::GetEmpty()
{
    s_instance->meshes.push_back(std::make_unique<Mesh>());
    return s_instance->meshes.back().get();
}
