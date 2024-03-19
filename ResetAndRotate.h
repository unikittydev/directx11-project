#pragma once

#include "Source/Components/IComponent.h"
#include "Source/Components/Transform.h"
#include "Source/Games/Game.h"
#include "ThirdParty/SimpleMath.h"

class ResetAndRotate : public IComponent
{
private:
	Transform* tr;

public:
	float spinSpeed = 5;
	float orbitSpeed = 5;
	float orbitRadius = 1;

	ResetAndRotate(Entity e) : IComponent(e)
	{

	}

	void Init() override
	{
		tr = Game::getInstance().GetComponent<Transform>(entity);
	}

	void Update() override
	{
		float arg = orbitSpeed * Time::time();
		float x, y;
		DirectX::XMScalarSinCos(&x, &y, arg);

		if (tr->GetParent() != nullptr)
			tr->SetWorldTranslation(tr->GetParent()->GetWorldTranslation() + orbitRadius * float3{ x, 0.0f, y });
		tr->SetWorldEuler({ 0.0f, spinSpeed * Time::time(), 0.0f});
	}

	void Destroy() override
	{

	}

	void Draw() override
	{

	}
};