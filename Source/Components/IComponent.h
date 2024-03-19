#pragma once

#include "Entity.h"

class IComponent
{
public:
	Entity entity;

	IComponent(Entity entity) : entity(entity)
	{

	}

	IComponent(IComponent& other) = delete;

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;
};