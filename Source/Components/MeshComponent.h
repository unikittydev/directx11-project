#pragma once
#include "IComponent.h"
#include "Transform.h"
#include "Camera/Camera.h"
#include "Games/Game.h"
#include "Graphics/Mesh.h"

class MeshComponent : public IComponent
{
private:
    Mesh* mesh;
    Transform* tr;
	Camera* camera;
    
public:
    MeshComponent(Entity e) : IComponent(e)
    {
        
    }

    void SetCamera(Camera* camera)
    {
        this->camera = camera;
    }
    
    void SetMesh(Mesh* mesh)
    {
        this->mesh = mesh;
    }

    void Init() override
    {
        tr = Game::getInstance().GetComponent<Transform>(entity);
    }
    
    void Draw() override
    {
        mesh->Draw(camera->GetViewProjectionMatrix(), tr->GetWorldMatrix());
    }

    void Update() override
    {
        
    }

    void Destroy() override
    {
        
    }
};
