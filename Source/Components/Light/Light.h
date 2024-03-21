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
public:
    Light(Entity e);

    void SetLightType(Type type);
    
    void Init() override;
    void Update() override;
    void Destroy() override;
    void Draw() override;

    Transform* GetTr() const;
    float4 GetColor() const;
};
