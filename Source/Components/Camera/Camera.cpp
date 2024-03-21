#include "Camera.h"
#include "Components/Transform.h"
#include "Application/Application.h"
#include "Games/Game.h"

Camera* Camera::s_active = nullptr;

void Camera::SetPerspective(float aspectRatio, float fov, float nearPlane, float farPlane)
{
	this->aspectRatio = aspectRatio;
	this->fov = fov;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

	SetPerspective();
}

void Camera::SetPerspective()
{
	projMat = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane); 
}

void Camera::SetOrtographic(float aspectRatio, float fov, float nearPlane, float farPlane)
{
	this->aspectRatio = aspectRatio;
	this->fov = fov;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

	SetOrtographic();
}

void Camera::SetOrtographic()
{
	float sinFov;
	float cosFov;

	DirectX::XMScalarSinCos(&sinFov, &cosFov, fov * .5f);

	const float sizeFactor = 20.0f;
	const float height = cosFov / sinFov * sizeFactor;
	const float width = height * aspectRatio;

	projMat = DirectX::XMMatrixOrthographicLH(width, height, nearPlane, farPlane);
}


void Camera::Init()
{
	width = Application::WindowWidth();
	height = Application::WindowHeight();
	aspectRatio = (float)width / height;

	this->tr = Game::getInstance().GetComponent<Transform>(entity);

	SetPerspective();

	if (s_active == nullptr)
		s_active = this;
}

void Camera::Update()
{
	viewMat = tr->GetWorldMatrix().Invert();

	if (Application::WindowWidth() != width || Application::WindowHeight() != height)
		Init();
}

void Camera::Draw()
{

}

void Camera::Destroy()
{

}

matrix Camera::GetProjectionMatrix() const
{
	return projMat;
}

matrix Camera::GetViewMatrix() const
{
	return viewMat;
}

matrix Camera::GetViewProjectionMatrix() const
{
	return viewMat * projMat;
}

Camera* Camera::GetActive()
{
	return s_active;
}
