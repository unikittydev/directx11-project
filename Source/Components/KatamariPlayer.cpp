#include "KatamariPlayer.h"

#include "SphereCollider.h"
#include "Transform.h"
#include "Core/Time.h"
#include "Games/Game.h"
#include "Input/InputDevice.h"

KatamariPlayer::KatamariPlayer(Entity e) : IComponent(e)
{
    
}

void KatamariPlayer::Init()
{
    tr = Game::getInstance().GetComponent<Transform>(entity);
    col = Game::getInstance().GetComponent<SphereCollider>(entity);
}

void KatamariPlayer::Update()
{
    Move();
    CheckIntersection();
}

void KatamariPlayer::Destroy()
{
    
}

void KatamariPlayer::Draw()
{
    
}

void KatamariPlayer::SetCamera(Camera* camera)
{
    this->cam = camera;
}

void KatamariPlayer::SetItems(const std::vector<SphereCollider*>& colliders)
{
    this->colliders = colliders;
}

void KatamariPlayer::Move()
{
    float rotateDirection = 0.0f;
    
    if (InputDevice::GetInstance()->IsKeyDown(LEFT))
        rotateDirection = -rotateSpeed * Time::deltaTime();
    else if (InputDevice::GetInstance()->IsKeyDown(RIGHT))
        rotateDirection = rotateSpeed * Time::deltaTime();

    velocityDirection = DirectX::XMVector3Rotate(velocityDirection, quaternion::CreateFromAxisAngle(float3::Up, rotateDirection));

    tr->Translate(velocityDirection * moveSpeed * Time::deltaTime());

    auto dir = velocityDirection.Cross(float3::Up);
    
    tr->Rotate(quaternion::CreateFromAxisAngle(dir, -spinSpeed * Time::deltaTime()));
}

void KatamariPlayer::CheckIntersection()
{
    for (int i = 0; i < colliders.size(); ++i)
        if (colliders[i]->Intersects(col))
        {
            auto e = colliders[i]->entity;
            Game::getInstance().GetComponent<Transform>(e)->SetParent(tr);
            colliders.erase(colliders.begin() + i);

            float3 scale = tr->GetLocalScale();
            scale += float3 { addRadius };
            tr->SetLocalScale(scale);
            
            break;
        }
}
