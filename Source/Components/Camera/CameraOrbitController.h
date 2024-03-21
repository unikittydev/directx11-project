#pragma once

#include "Components/Transform.h"
#include "Components/Camera/Camera.h"
#include "Input/InputDevice.h"
#include "Components/IComponent.h"

class CameraOrbitController : public IComponent
{
private:
	Transform* tr;
	Transform* target;
	Camera* camera;

	Keys TOGGLE{ Keys::Tab };

	Keys FORWARD{ Keys::W };
	Keys BACKWARD{ Keys::S };

	Keys PERSPECTIVE{ Keys::D1 };
	Keys ORTHOGRAPHIC{ Keys::D2 };

	float2 minMaxSpeed = { .01f, 100.0f };
	float moveSpeed = 10.0f;
	float2 sensitivity = { 1.0f, 0.6f };

	float radius = 5.0f;

	float2 delta;
public:
	CameraOrbitController(Entity e) : IComponent(e)
	{

	}

	void Init() override;
	void Update() override;
	void Destroy() override;
	void Draw() override;
	
	Transform* GetTarget() const;
	
	void SetTarget(Transform* tr)
	{
		target = tr;
	}

private:
	void Move();

	void RotateCamera(const InputDevice::MouseMoveEventArgs& args);

	void ChangeMode();
};