#pragma once

#include "Game.h"

#include "../Components/2D/RacketComponent.h"
#include "../Components/2D/BallComponent.h"
#include "../Components/2D/ScoreComponent.h"

class Pong : public Game
{
public:
	Pong() : Game()
	{
		Entity e = CreateEntity();

		components.push_back(std::make_unique<QuadComponent>(e));
		components.push_back(std::make_unique<QuadComponent>(e));

		components.push_back(std::make_unique<ScoreComponent>(e));
		components.push_back(std::make_unique<ScoreComponent>(e));

		components.push_back(std::make_unique<RacketComponent>(e));
		components.push_back(std::make_unique<RacketComponent>(e));

		components.push_back(std::make_unique<BallComponent>(e));

		auto* background = reinterpret_cast<RacketComponent*>(components.at(0).get());
		background->SetColor(float4(0.1f, 0.1f, 0.4f, 1.0f));
		background->SetRect(float4(0, 0, 0, 0), float4(1, 1, 1, 1));

		auto* separator = reinterpret_cast<RacketComponent*>(components.at(1).get());
		separator->SetColor(float4(0.7f, 0.7f, 0.7f, 1.0f));
		separator->SetRect(float4(0, 0, 0, 0), float4(.01f, 1, 1, 1));

		auto* scoreLeft = reinterpret_cast<ScoreComponent*>(components.at(2).get());
		scoreLeft->SetStartLocationAndDirection(float4(-.9f, .9f, 0, 0), float4(1, 0, 0, 0));

		auto* scoreRight = reinterpret_cast<ScoreComponent*>(components.at(3).get());
		scoreRight->SetStartLocationAndDirection(float4(.9f, .9f, 0, 0), float4(-1, 0, 0, 0));

		auto* racketLeft = reinterpret_cast<RacketComponent*>(components.at(4).get());
		racketLeft->SetColor(float4(0.2f, 0.9f, 0.1f, 1.0f));
		racketLeft->SetInputKeys(Keys::W, Keys::S);
		racketLeft->SetRect(float4(-.9f, 0, 0, 0), float4(.025f, .25f, 1, 1));
		racketLeft->SetScore(scoreLeft);

		auto* racketRight = reinterpret_cast<RacketComponent*>(components.at(5).get());
		racketRight->SetColor(float4(0.9f, 0.1f, 0.2f, 1.0f));
		racketRight->SetInputKeys(Keys::Up, Keys::Down);
		racketRight->SetRect(float4(.9f, 0, 0, 0), float4(.025f, .25f, 1, 1));
		racketRight->SetScore(scoreRight);

		auto* ball = reinterpret_cast<BallComponent*>(components.at(6).get());
		ball->SetColor(float4(1, 1, 1, 1));
		ball->SetRect(float4(0, 0, 0, 0), float4(.025f, .025f, 1, 1));
		ball->SetRackets(racketLeft, racketRight);
	}
};