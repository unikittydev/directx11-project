#pragma once

#include "Core/Types.h"
#include "IComponent.h"
#include "Transform.h"
#include "Games/Game.h"

class MoveRotateComponent : public IComponent
{
private:
	Transform* tr;
public:
	float moveSpeed = 0;
	float3 moveAmplitude = { 10, 10, 10 };
	float3 rotateSpeed{ 1, 0, 0 };

	MoveRotateComponent(Entity e) : IComponent(e)
	{

	}

	void Init() override
	{
		this->tr = Game::getInstance().GetComponent<Transform>(entity);
	}

	void Update() override
	{
		float arg = Time::time() * moveSpeed;

		float sin, cos;
		DirectX::XMScalarSinCos(&sin, &cos, arg);

		float3 move = { cos * Time::deltaTime(), 0, sin * Time::deltaTime() };
		tr->SetLocalTranslation(moveAmplitude * move);
		tr->RotateEuler(rotateSpeed * Time::deltaTime());
	}

	void Destroy() override
	{

	}

	void Draw() override
	{

	}
};