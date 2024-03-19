#pragma once

#include "../../Games/Game.h"

#include "../IComponent.h"
#include "QuadComponent.h"

class ScoreComponent : public IComponent
{
private:
	const float size = .025f;
	const float spacing = 0.01f;
	
	int value = 0;

	float4 location;
	float4 direction;

public:
	void Init() override;

	void Draw() override;

	void Update() override;

	void Destroy() override;

	int GetScore() const
	{
		return value;
	}

	void SetStartLocationAndDirection(float4 location, float4 direction)
	{
		this->location = location;
		this->direction = direction;
	}

	void Increment()
	{
		value++;

		//auto* quad = Game::getInstance().AddComponent<QuadComponent>(entity);
		//quad->SetColor(float4(.5f, .75f, 1, 1));
		//quad->SetRect(location + direction * (float)(value - 1) * (2.0f * size + spacing), float4(size, size, 1.0f, 1.0f));
	}
};