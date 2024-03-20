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
	Camera* camera;

    Texture2D mainTexture;
    
public:
    MeshComponent(Entity e);

    void SetCamera(Camera* camera);
    
    void SetMesh(Mesh* mesh);

    void Init() override;
    
    void Draw() override;

    void Update() override;

    void Destroy() override;

    const Texture2D& GetMainTexture() const;

    void SetMainTexture(const Texture2D& texture);
};
