#include <DirectXCollision.h>

#include "../../../ThirdParty/SimpleMath.h"

#include "RacketComponent.h"

void RacketComponent::Init()
{
	QuadComponent::Init();
}

void RacketComponent::Update()
{
	DirectX::SimpleMath::Plane top({ 0, 1, 0 }, { 0, -1, 0 });
	DirectX::SimpleMath::Plane bottom({ 0, -1, 0 }, { 0, 1, 0 });

	float dt = Time::deltaTime();

	if (AABB.Intersects(top))
		location -= float4{ 0.0f, 1.0f, 0.0f, 0.0f } * dt;
	else if (AABB.Intersects(bottom))
		location += float4{ 0.0f, 1.0f, 0.0f, 0.0f } * dt;

	if (InputDevice::GetInstance()->IsKeyDown(up))
		location += float4{ 0.0f, 1.0f, 0.0f, 0.0f } * dt;
	else if (InputDevice::GetInstance()->IsKeyDown(down))
		location -= float4{ 0.0f, 1.0f, 0.0f, 0.0f } * dt;

	AABB.Center = { location.x, location.y, location.z };
	AABB.Extents = { size.x, size.y, size.z };

	D3D11_MAPPED_SUBRESOURCE res = {};

	auto* ctx = Application::GetDeviceContext();
	ctx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	float4* data = reinterpret_cast<float4*>(res.pData);

	data[0] = location;

	ctx->Unmap(constantBuffer, 0);
}