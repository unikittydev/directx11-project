#pragma once

#include "Game.h"

class SceneGame : public Game
{
public:
	Entity Instantiate(const std::wstring& name, float scale);
	
	SceneGame();
};
