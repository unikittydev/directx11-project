#pragma once

#include "IComponent.h"
#include "Transform.h"
#include "Camera/Camera.h"

#include "Graphics/Mesh.h"
#include "Graphics/Primitives.h"

class SphereComponent : public IComponent
{
private:
	Transform* tr;

	Mesh mesh{ ::SpherePrimitive() };

public:
	SphereComponent(Entity e) : IComponent(e)
	{

	}

	void Init() override
	{
		this->tr = Game::getInstance().GetComponent<Transform>(entity);
	}

	void Update() override
	{
		if (InputDevice::GetInstance()->IsKeyDown(Keys::Enter))
		{
			Game::getInstance().RemoveComponent(entity, tr);
			Game::getInstance().RemoveComponent(entity, this);
		}
	}

	void Draw() override
	{
		matrix vp = Camera::GetActive()->GetViewProjectionMatrix();
		mesh.Draw(vp, tr->GetWorldMatrix());
	}

	void Destroy() override
	{
		mesh.Release();
	}

	void SetColor(float4 color)
	{
		mesh.SetColor(color);
	}
};