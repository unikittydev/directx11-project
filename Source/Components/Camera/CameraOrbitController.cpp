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
	std::cout << "Orbit" << tr->GetWorldTranslation().x << " " << tr->GetWorldTranslation().y << " " << tr->GetWorldTranslation().z << std::endl;
	if (InputDevice::GetInstance()->IsKeyDown(TOGGLE))
	{
		Game::getInstance().AddComponent<CameraFPSController>(entity);
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
	float moveDelta = 0.0f;

	if (InputDevice::GetInstance()->IsKeyDown(FORWARD))
		moveDelta = 1.0f;
	else if (InputDevice::GetInstance()->IsKeyDown(BACKWARD))
		moveDelta = -1.0f;

	moveDelta *= moveSpeed * Time::deltaTime();

	radius += moveDelta;

	tr->SetLocalTranslation(tr->GetForward() * -radius);
	
	float3 euler = tr->GetWorldEuler();
	euler.z = 0.0f;
	tr->SetLocalEuler(euler);
}

void CameraOrbitController::RotateCamera(const InputDevice::MouseMoveEventArgs& args)
{
	float2 offset = args.Offset * sensitivity * 0.01f;

	float acc = 1.0f;
	if (args.WheelDelta > 0)
		acc = 1.05f;
	else if (args.WheelDelta < 0)
		acc = 0.95f;

	tr->RotateEuler(offset.x, offset.y, 0.0f);

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