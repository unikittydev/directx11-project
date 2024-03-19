#pragma once

#include "../../ThirdParty/SimpleMath.h"
#include "IComponent.h"

class Transform;

class Camera : public IComponent
{
private:
	matrix viewMat;
	matrix projMat;

	Transform* tr;

	UINT width;
	UINT height;

	float aspectRatio;
	float fov = 1.6f;

	float nearPlane = 0.01f;
	float farPlane = 1000.0f;

public:
	Camera(Entity e) : IComponent(e)
	{

	}

	void SetPerspective(float aspectRatio, float fov, float nearPlane, float farPlane);

	void SetPerspective();

	void SetOrtographic(float aspectRatio, float fov, float nearPlane, float farPlane);

	void SetOrtographic();

	void Init() override;

	void Update() override;

	void Draw() override;

	void Destroy() override;

	matrix GetProjectionMatrix() const;

	matrix GetViewMatrix() const;

	matrix GetViewProjectionMatrix() const;
};