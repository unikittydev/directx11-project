#pragma once
#include "IComponent.h"
#include "Core/Types.h"

class SphereComponent;
class Transform;

class SphereCollider : public IComponent
{
private:
    Transform* tr;
    
    boundingSphere BS = {{ 0.0f, 0.0f, 0.0f }, 1.0f};

public:
    SphereComponent* wireframe;
    
    SphereCollider(Entity e);

    void Init() override;

    void Update() override;

    void Destroy() override;

    void Draw() override;
    
    boundingSphere GetBS() const;

    boundingSphere GetWorldBS() const;

    void SetBS(const boundingSphere& bs);

    bool Intersects(const SphereCollider* other) const;
};
