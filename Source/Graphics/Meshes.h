#pragma once
#include "Mesh.h"

class Meshes
{
    std::vector<Mesh*> meshes;

    static Meshes* s_instance;
public:
    Meshes();

    ~Meshes();
    
    static Mesh* GetEmpty();
};
