#pragma once

#include "../../Input/InputDevice.h"

#include "QuadComponent.h"
#include "ScoreComponent.h"

class RacketComponent : public QuadComponent
{
private:
	Keys up;
	Keys down;

	DirectX::BoundingBox AABB;

	ScoreComponent* score;
public:
	void SetInputKeys(Keys up, Keys down)
	{
		this->up = up;
		this->down = down;
	}

	void SetScore(ScoreComponent* score)
	{
		this->score = score;
	}

	DirectX::BoundingBox GetAABB()
	{
		return AABB;
	}

	void Init() override;

	void Update() override;

	void AddScore()
	{
		score->Increment();
	}
};