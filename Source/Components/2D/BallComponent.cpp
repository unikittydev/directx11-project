#include <math.h>

#include "BallComponent.h"

void BallComponent::Init()
{
	QuadComponent::Init();
	location = float4(0, 0, 0, 0);
	velocity = startDirection * startVelocity;
}

void BallComponent::Update()
{
	// Update location and AABB
	if (respawnCounter >= countdown)
		location += float4{ velocity.x, velocity.y, 0, 0 } * Time::deltaTime();

	AABB.Center = { location.x, location.y, location.z };
	AABB.Extents = { size.x, size.y, size.z };

	// Jump from edges
	DirectX::SimpleMath::Plane top({ 0, 1, 0 }, { 0, -1, 0 });
	DirectX::SimpleMath::Plane bottom({ 0, -1, 0 }, { 0, 1, 0 });

	if (AABB.Intersects(top))
		velocity.y *= -1 - acceleration;
	else if (AABB.Intersects(bottom))
		velocity.y *= -1 - acceleration;

	// Jump from rackets
	if (AABB.Intersects(leftRacket->GetAABB()))
	{
		float3 normal = GetAABBNormalFacingUp(leftRacket->GetAABB());
		velocity = normal * (velocity.Length() + acceleration);
	}
	else if (AABB.Intersects(rightRacket->GetAABB()))
	{
		float3 normal = GetAABBNormalFacingUp(rightRacket->GetAABB());
		velocity = normal * (velocity.Length() + acceleration);
	}

	// Missed the ball; respawn
	DirectX::SimpleMath::Plane left({ -1, 0, 0 }, { 1, 0, 0 });
	DirectX::SimpleMath::Plane right({ 1, 0, 0 }, { -1, 0, 0 });

	if (AABB.Intersects(left))
		Reset(false);
	else if (AABB.Intersects(right))
		Reset(true);

	auto* ctx = Application::GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE res = {};
	ctx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	float4* data = reinterpret_cast<float4*>(res.pData);

	data[0] = location;

	ctx->Unmap(constantBuffer, 0);

	respawnCounter += Time::deltaTime();
}