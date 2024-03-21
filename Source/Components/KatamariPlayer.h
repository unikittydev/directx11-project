#pragma once
#include <vector>

#include "IComponent.h"
#include "Core/Types.h"
#include "Input/Keys.h"

class SphereCollider;
class Camera;
class Transform;

class KatamariPlayer : public IComponent
{
private:
    Camera* cam;
    Transform* tr;
    SphereCollider* col;

    std::vector<SphereCollider*> colliders;

    Keys LEFT = Keys::A;
    Keys RIGHT = Keys::D;

    float moveSpeed = 5;
    float rotateSpeed = 3;
    float spinSpeed = 3;
    float3 velocityDirection { 1, 0, 0 };
    float addRadius = .05f;
    
    void Move();
    void CheckIntersection();
public:
    KatamariPlayer(Entity e);

    void SetCamera(Camera* camera);
    void SetItems(const std::vector<SphereCollider*>& colliders);
    
    void Init() override;
    void Update() override;
    void Destroy() override;
    void Draw() override;

};
