#include "MeshSpawner.h"

#include "Games/SceneGame.h"
#include "Entity.h"
#include "Transform.h"

MeshSpawner::MeshSpawner(const std::wstring* meshNames, int n) : selectDist(0, n - 1), meshNames(meshNames)
{
}

Entity MeshSpawner::InstantiateRandomMesh()
{
    SceneGame& game = reinterpret_cast<SceneGame&>(Game::getInstance());

    const auto nameIndex = selectDist(re);
    Entity e = game.Instantiate(meshNames[nameIndex], .005f);

    const float x = trDist(re); 
    const float z = trDist(re);
        
    const float rotY = rotDist(re);

    game.GetComponent<Transform>(e)->SetWorldTranslation({ x, 0.0f, z });
    game.GetComponent<Transform>(e)->SetWorldEuler({ 0.0f, rotY, 0.0f });

    return e;
}