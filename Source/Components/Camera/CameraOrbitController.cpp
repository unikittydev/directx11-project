#pragma once

#include "CameraOrbitController.h"
#include "Components/Camera/CameraFPSController.h"
#include "Games/Game.h"

#include <iostream>

void CameraOrbitController::Init()
{
	Application::GetWindow().SetCursorLocked(TRUE);
	Application::GetWindow().SetCursorVisible(FALSE);
	InputDevice::GetInstance()->MouseMove.AddRaw(this, &CameraOrbitController::RotateCamera);

	this->tr = Game::getInstance().GetComponent<Transform>(entity);
	this->camera = Game::getInstance().GetComponent<Camera>(entity);
}

void CameraOrbitController::Update()
{
	if (InputDevice::GetInstance()->IsKeyDown(TOGGLE))
	{
		auto* controller = Game::getInstance().AddComponent<CameraFPSController>(entity);
		controller->SetTarget(target);
		Game::getInstance().RemoveComponent(entity, this);
		return;
	}

	Move();
	ChangeMode();
}

void CameraOrbitController::Destroy()
{
	InputDevice::GetInstance()->MouseMove.RemoveAll();
}

void CameraOrbitController::Draw()
{

}

void CameraOrbitController::Move()
{
	tr->RotateEuler(delta.x * Time::deltaTime(), 0.0f, 0.0f);
	tr->RotateEuler(0.0f, delta.y * Time::deltaTime(), 0.0f);
	
	float moveDelta = 0.0f;

	if (InputDevice::GetInstance()->IsKeyDown(FORWARD))
		moveDelta = 1.0f;
	else if (InputDevice::GetInstance()->IsKeyDown(BACKWARD))
		moveDelta = -1.0f;

	moveDelta *= moveSpeed * Time::deltaTime();

	radius += moveDelta;

	const float3 offset = target != nullptr ? target->GetWorldTranslation() : float3{ 0, 0 , 0 };
	
	tr->SetWorldTranslation(offset + tr->GetForward() * -radius);
	
	
	float3 euler = tr->GetWorldEuler();
	euler.z = 0.0f;
	tr->SetLocalEuler(euler);
	
	
	delta = float2{};
}

void CameraOrbitController::RotateCamera(const InputDevice::MouseMoveEventArgs& args)
{
	delta += args.Offset * sensitivity * .1f;
	float2 offset = args.Offset * sensitivity * 0.01f;

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

void CameraOrbitController::ChangeMode()
{
	if (InputDevice::GetInstance()->IsKeyDown(PERSPECTIVE)) {
		camera->SetPerspective();
	}
	else if (InputDevice::GetInstance()->IsKeyDown(ORTHOGRAPHIC)) {
		camera->SetOrtographic();
	}
}

Transform* CameraOrbitController::GetTarget() const
{
	return target;
}
