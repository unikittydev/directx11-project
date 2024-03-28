#pragma once
#include "Components/IComponent.h"
#include "Core/Types.h"

class Transform;

class Light : public IComponent
{
public:
    enum Type
    {
        Directional,
        Point
    };
private:
    Type type;

    float4 color;
    float range;
public:
    Light(Entity e);
    
    void Init() override;
    void Update() override;
    void Destroy() override;
    void Draw() override;
    
    void SetColor(float4 color);
    void SetLightType(Type type);
    void SetRange(float range);
    
    Transform* GetTr() const;
    float4 GetColor() const;
    float GetRange() const;
};
