#pragma once

#include "IComponent.h"
#include "Graphics/Mesh.h"

class Transform;

class SphereComponent : public IComponent
{
private:
	Transform* tr;
	Mesh mesh;

public:
	SphereComponent(Entity e);

	void Init() override;

	void Update() override;

	void Draw() override;

	void Destroy() override;

	void SetColor(float4 color);

	void SetShader(Shader* shader);
};
