#pragma once

#include "Input/InputDevice.h"
#include "Components/IComponent.h"
#include "Core/Types.h"

class Transform;
class Camera;

class CameraFPSController : public IComponent
{
private:
	Transform* tr;
	Camera* camera;

	float2 minMaxSpeed = { .01f, 500.0f };
	float moveSpeed = 10.0f;
	float2 sensitivity = { 1.0f, 0.6f };

	float2 cameraRotation;
	float maxAngle = 1.5f;

	Keys TOGGLE{ Keys::Tab };

	Keys FORWARD{ Keys::W };
	Keys BACKWARD{ Keys::S };
	Keys LEFT{ Keys::A };
	Keys RIGHT{ Keys::D };
	Keys UP{ Keys::Space };
	Keys DOWN{ Keys::LeftShift };
	Keys PERSPECTIVE{ Keys::D1 };
	Keys ORTHOGRAPHIC{ Keys::D2 };

public:
	CameraFPSController(Entity e) : IComponent(e)
	{

	}

	void Init() override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

private:
	void Move();
	void RotateCamera(const InputDevice::MouseMoveEventArgs& args);
	void ChangeMode();
};