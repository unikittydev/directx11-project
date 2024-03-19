#include "Source/Games/Game.h"

template<typename T>
T* Game::AddComponent(Entity e)
{
	static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent.");

	components.push_back(std::make_unique<T>(e));
	auto* component = reinterpret_cast<T*>(components.back().get());
	adding.push(component);

	entityComponentMap.insert(std::make_pair(e, component));
	return component;
}

template<typename T>
void Game::RemoveComponent(Entity e, T* component)
{
	static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent.");

	for (auto it = entityComponentMap.begin(); it != entityComponentMap.end(); it++)
		if ((*it).first == e && (*it).second == component)
		{
			entityComponentMap.erase(it);
			break;
		}

	for (auto it = components.begin(); it < components.end(); it++)
		if ((*it).get() == component)
		{
			destroying.push((*it).get());
			break;
		}
}

template<typename T>
T* Game::GetComponent(Entity e)
{
	static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent.");

	const auto& map = entityComponentMap;
	auto ret = map.equal_range(e);
	for (auto it = ret.first; it != ret.second; it++)
	{
		T* casted = dynamic_cast<T*>(it->second);
		if (casted != nullptr)
			return casted;
	}
	return nullptr;
}