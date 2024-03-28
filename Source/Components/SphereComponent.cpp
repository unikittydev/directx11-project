#include "SphereComponent.h"

#include "Camera/Camera.h"
#include "Graphics/Primitives.h"
#include "Transform.h"

SphereComponent::SphereComponent(Entity e) : IComponent(e), mesh{::SpherePrimitive()}
{

}

void SphereComponent::Init()
{
    this->tr = Game::getInstance().GetComponent<Transform>(entity);
}

void SphereComponent::Update()
{
    if (InputDevice::GetInstance()->IsKeyDown(Keys::Enter))
    {
        Game::getInstance().RemoveComponent(entity, tr);
        Game::getInstance().RemoveComponent(entity, this);
    }
}

void SphereComponent::Draw()
{
    mesh.Draw(tr->GetWorldMatrix(), tr->GetWorldTranslation());
}

void SphereComponent::Destroy()
{
    mesh.Release();
}

void SphereComponent::SetColor(float4 color)
{
    mesh.SetColor(color);
}

void SphereComponent::SetShader(Shader* shader)
{
    mesh.SetShader(shader);
}