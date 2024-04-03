#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Primitives.h"

#include "IComponent.h"
#include "Transform.h"
#include "Camera/Camera.h"

class CubeComponent : public IComponent
{
	using matrix = DirectX::SimpleMath::Matrix;

private:
	Transform* tr;

	Mesh mesh{ ::CubePrimitive() };

public:
	CubeComponent(Entity e) : IComponent(e)
	{

	}

	void Init() override
	{
		this->tr = Game::getInstance().GetComponent<Transform>(entity);
		Game::getInstance().GetRenderPipeline().AddMesh(&mesh);
	}

	void Update() override
	{
		
	}

	void Draw() override
	{
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