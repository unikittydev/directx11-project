#include "Light.h"

#include "Games/Game.h"
#include "Components/Transform.h"
#include "Core/Color.h"

Light::Light(Entity e) : IComponent(e), color{ Color::white }
{
    
}

void Light::Init() 
{
    if (type == Directional)
        Game::getInstance().GetRenderPipeline().SetMainLight(this);
    else if (type == Point)
        Game::getInstance().GetRenderPipeline().AddAdditionalLight(this);
}

void Light::Update() 
{
    
}

void Light::Destroy() 
{
    
}

void Light::Draw() 
{
    
}

void Light::SetColor(float4 color)
{
    this->color = color;
}

void Light::SetLightType(Type type)
{
    this->type = type;
}

void Light::SetRange(float range)
{
    this->range = range;
}

Transform* Light::GetTr() const
{
    return Game::getInstance().GetComponent<Transform>(entity);
}

float4 Light::GetColor() const
{
    return color;
}

float Light::GetRange() const
{
    return range;
}