#pragma once

#include "Source/Components/IComponent.h"
#include "Source/Components/Transform.h"
#include "Source/Games/Game.h"

class FollowTarget : public IComponent
{
private:
	Transform* target;
	Transform* tr;

public:
	FollowTarget(Entity e) : IComponent(e)
	{

	}

	void SetTarget(Transform* target)
	{
		this->target = target;
	}

	void Init() override
	{
		tr = Game::getInstance().GetComponent<Transform>(entity);
	}

	void Update() override
	{

	}

	void Destroy() override
	{

	}

	void Draw() override
	{

	}
};