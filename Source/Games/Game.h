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

	static Game* s_instance;

protected:
	std::vector<std::unique_ptr<IComponent>> components{};
	std::queue<IComponent*> adding{};
	std::queue<IComponent*> destroying{};

	std::multimap<Entity, IComponent*> entityComponentMap{};

	int lastId;

	void PreDrawFrame()
	{
		RECT winRect;
		GetClientRect(app.GetWindow().GetHWND(), &winRect);

		//device.GetContext()->ClearState();

		D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (FLOAT)Application::WindowWidth(), (FLOAT)Application::WindowHeight(), 0.0f, 1.0f };

		Application::GetDeviceContext()->RSSetViewports(1, &viewport);

		const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		Application::GetDeviceContext()->OMSetRenderTargets(1, &Application::GetSwapchain().GetRenderTargetView(), NULL);
		Application::GetDeviceContext()->ClearRenderTargetView(Application::GetSwapchain().GetRenderTargetView(), clearColor);
	}

	void DrawFrame()
	{
		Application::GetSwapchainPtr()->Present(1, 0);
	}
public:
	Game()
	{
		Game::s_instance = this;
	}

	static Game& getInstance()
	{
		return *s_instance;
	}
	
	void Run()
	{
		Application::GetWindow().Show();

		for (auto&& component : components)
			component->Init();

		while (!Application::GetWindow().ShouldClose())
		{
			time.UpdateTime();

			Application::GetWindow().Update();

			PreDrawFrame();

			// Init phase
			while (!adding.empty())
			{
				adding.front()->Init();
				adding.pop();
			}

			// Update phase
			for (auto&& component : components)
				component->Update();

			for (auto&& component : components)
				component->Draw();

			// Destroy phase
			while (!destroying.empty())
			{
				auto* component = destroying.front();
				component->Destroy();

				for (auto it = components.begin(); it < components.end(); it++)
					if ((*it).get() == component)
					{
						components.erase(it);
						break;
					}
				destroying.pop();
			}

			DrawFrame();
			time.Finish();
		}

		for (auto&& component : components)
			component->Destroy();
	}

	Entity CreateEntity()
	{
		const Entity e { ++lastId };
		entityComponentMap.insert(std::make_pair(e, nullptr));
		return e;
	}

	template<typename T>
	T* AddComponent(Entity e);

	template<typename T>
	void RemoveComponent(Entity e, T* component);

	template<typename T>
	T* GetComponent(Entity e);

	/*template<typename T>
	T* AddComponent()
	{
		static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent.");

		components.push_back(std::make_unique<T>());
		auto* comp = reinterpret_cast<T*>(components.back().get());
		adding.push(comp);

		return comp;
	}*/

	/*template<typename T>
	void RemoveComponent(T* component)
	{
		static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent.");

		for (auto it = components.begin(); it < components.end(); it++)
			if ((*it).get() == component)
			{
				destroying.push((*it).get());
				break;
			}
	}*/
};

#include "Game.hpp"
#endif