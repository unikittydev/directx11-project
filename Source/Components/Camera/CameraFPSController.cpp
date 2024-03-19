#include "CameraFPSController.h"
#include "Games/Game.h"

#include "Components/Transform.h"
#include "Camera.h"
#include "CameraOrbitController.h"

#include <iostream>

void CameraFPSController::Init()
{
	Application::GetWindow().SetCursorLocked(TRUE);
	Application::GetWindow().SetCursorVisible(FALSE);
	InputDevice::GetInstance()->MouseMove.AddRaw(this, &CameraFPSController::RotateCamera);

	this->tr = Game::getInstance().GetComponent<Transform>(entity);
	this->camera = Game::getInstance().GetComponent<Camera>(entity);

	cameraRotation = { tr->GetWorldEuler().x, tr->GetWorldEuler().y };
}

void CameraFPSController::Update()
{
	//std::cout << "FPS" << tr->GetWorldTranslation().x << " " << tr->GetWorldTranslation().y << " " << tr->GetWorldTranslation().z << std::endl;
	if (InputDevice::GetInstance()->IsKeyDown(TOGGLE))
	{
		Game::getInstance().AddComponent<CameraOrbitController>(entity);
		Game::getInstance().RemoveComponent(entity, this);
		return;
	}

	Move();
	ChangeMode();
}

void CameraFPSController::Draw()
{

}

void CameraFPSController::Destroy()
{
	InputDevice::GetInstance()->MouseMove.RemoveAll();
}

void CameraFPSController::Move()
{
	float3 moveDelta;
	if (InputDevice::GetInstance()->IsKeyDown(FORWARD))
		moveDelta.z = -1;
	else if (InputDevice::GetInstance()->IsKeyDown(BACKWARD))
		moveDelta.z = 1;
	if (InputDevice::GetInstance()->IsKeyDown(RIGHT))
		moveDelta.x = 1;
	else if (InputDevice::GetInstance()->IsKeyDown(LEFT))
		moveDelta.x = -1;
	if (InputDevice::GetInstance()->IsKeyDown(UP))
		moveDelta.y = 1;
	else if (InputDevice::GetInstance()->IsKeyDown(DOWN))
		moveDelta.y = -1;

	float3 translation =
		moveDelta.x * tr->GetRight() +
		moveDelta.y * float3(.0f, 1.0f, .0f) +
		moveDelta.z * tr->GetForward();
	
	if (translation.LengthSquared() > 0.0f)
		translation.Normalize();

	translation *= moveSpeed * Time::deltaTime();
	tr->Translate(translation);
}

void CameraFPSController::RotateCamera(const InputDevice::MouseMoveEventArgs& args)
{
	float2 offset = args.Offset * sensitivity * 0.01f;
	cameraRotation += offset;

	cameraRotation.y = max(-maxAngle, min(maxAngle, cameraRotation.y));

	quaternion rotation = quaternion::CreateFromYawPitchRoll(cameraRotation.x, cameraRotation.y, 0.0f);

	tr->SetLocalRotation(rotation);

	// To prevent camera from rolling
	float3 euler = rotation.ToEuler();
	euler.z = 0.0f;
	tr->SetLocalEuler(euler);

	float acc = 1.0f;
	if (args.WheelDelta > 0)
		acc = 1.05f;
	else if (args.WheelDelta < 0)
		acc = 0.95f;

	// Idk where clamp scalar is
	moveSpeed *= acc;
	if (moveSpeed < minMaxSpeed.x)
		moveSpeed = minMaxSpeed.x;
	else if (moveSpeed > minMaxSpeed.y)
		moveSpeed = minMaxSpeed.y;
}

void CameraFPSController::ChangeMode()
{
	if (InputDevice::GetInstance()->IsKeyDown(PERSPECTIVE)) {
		camera->SetPerspective();
	}
	else if (InputDevice::GetInstance()->IsKeyDown(ORTHOGRAPHIC)) {
		camera->SetOrtographic();
	}
}