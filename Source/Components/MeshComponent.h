#pragma once

#include "IComponent.h"
#include "Graphics/Texture2D.h"

class Camera;
class Transform;
class Mesh;

class MeshComponent : public IComponent
{
private:
    Mesh* mesh;
    Transform* tr;
    
public:
    MeshComponent(Entity e);

    void SetMesh(Mesh* mesh);

    void Init() override;
    
    void Draw() override;

    void Update() override;

    void Destroy() override;

    Mesh* GetMesh() const;
};
