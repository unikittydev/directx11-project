#pragma once

#include "../Graphics/Mesh.h"
#include "../Graphics/Primitives.h"

#include "IComponent.h"
#include "Transform.h"
#include "Camera.h"

class CubeComponent : public IComponent
{
	using matrix = DirectX::SimpleMath::Matrix;

private:
	Transform* tr;
	Camera* camera;

	Mesh mesh{ ::CubePrimitive() };

public:
	CubeComponent(Entity e) : IComponent(e)
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