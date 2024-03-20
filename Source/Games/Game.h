#pragma once

#ifndef _GAME
#define GAME

#include <chrono>
#include <vector>
#include <queue>
#include <map>

#include "Application/Application.h"
#include "Core/Time.h"

#include "Components/Entity.h"
#include "Components/IComponent.h"
#include "Graphics/Meshes.h"
#include "Graphics/Shaders.h"

using duration = std::chrono::duration<double>;
using sys_clock = std::chrono::system_clock;
using time_point = std::chrono::time_point<sys_clock, duration>;

class Game
{
	friend class InputDevice;
	friend struct Entity;

private:
	Application app{};
	Time time{};
	Shaders shaders{};
	Meshes meshes{};

	static Game* s_instance;

protected:
	std::vector<std::unique_ptr<IComponent>> components{};
	std::queue<IComponent*> adding{};
	std::queue<IComponent*> destroying{};

	std::multimap<Entity, IComponent*> entityComponentMap{};

	int lastId;
	
	void PreDrawFrame();

	void DrawFrame();
public:
	Game();

	static Game& getInstance();
	
	void Run();

	Entity CreateEntity();

	template<typename T>
	T* AddComponent(Entity e);

	template<typename T>
	void RemoveComponent(Entity e, T* component);

	template<typename T>
	T* GetComponent(Entity e);
};

#include "Game.hpp"
#endif