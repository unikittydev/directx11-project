#pragma once

#include "QuadComponent.h"
#include "RacketComponent.h"
#include "../../../ThirdParty/SimpleMath.h"

class BallComponent : public QuadComponent
{
private:
	const float3 startDirection = { .8f, 0.1f, 0.0f };
	const float startVelocity = 1.0f;
	const float countdown = 3.0f;
	const float acceleration = 0.2f;

	float3 velocity;
	float respawnCounter = 0;

	DirectX::BoundingBox AABB;

	RacketComponent* leftRacket;
	RacketComponent* rightRacket;
public:
	void SetRackets(RacketComponent* left, RacketComponent* right)
	{
		leftRacket = left;
		rightRacket = right;
	}

	void Reset(bool leftWon)
	{
		location = float4(0, 0, 0, 0);
		velocity = startDirection * startVelocity;

		if (leftWon)
		{
			leftRacket->AddScore();
		}
		else
		{
			rightRacket->AddScore();
			velocity.x *= -1;
		}

		respawnCounter = 0;
	}

	void Init() override;

	void Update() override;

	float3 GetAABBNormalFacingUp(DirectX::BoundingBox aabb)
	{
		float3 delta = -float3{ aabb.Center.x - AABB.Center.x, aabb.Center.y - AABB.Center.y, aabb.Center.z - AABB.Center.z };
		delta.Normalize();
		return delta;
	}
};