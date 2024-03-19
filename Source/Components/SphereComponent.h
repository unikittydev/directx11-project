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
	Camera* camera;

	Mesh mesh{ ::SpherePrimitive() };

public:
	SphereComponent(Entity e) : IComponent(e)
	{

	}

	// TODO: make tr available throught methods and make camera available globally.
	void SetCamera(Camera* camera)
	{
		this->camera = camera;
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
		matrix vp = camera->GetViewProjectionMatrix();
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