#pragma once
#include <random>

struct Entity;

class MeshSpawner
{
private:
    std::random_device r;
    std::default_random_engine re { r() };
    std::uniform_real_distribution<float> trDist{ -20.0f, 20.0f };
    std::uniform_real_distribution<float> rotDist{ 0.0f, 360.0f };
    std::uniform_int_distribution<int> selectDist;

    const std::wstring* meshNames;
public:
    MeshSpawner(const std::wstring* meshNames, int n);

    Entity InstantiateRandomMesh();
};
