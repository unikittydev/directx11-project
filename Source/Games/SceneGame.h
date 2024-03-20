#pragma once

#include "Game.h"

#include "Components/Camera/Camera.h"
#include "Components/Camera/CameraFPSController.h"
#include "Components/CubeComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ResetAndRotate.h"
#include "Core/Color.h"
#include "Graphics/ModelImporter.h"

class SceneGame : public Game
{
public:
	SceneGame() : Game()
	{
		// Camera

		Entity eCam = CreateEntity();
		auto* camTr = AddComponent<Transform>(eCam);
		camTr->SetLocalTranslation({ 0.0f, 0.0f, 20.0f });
		camTr->SetLocalEuler({ 0.0f, 0.0f, 0.0f });

		auto* cam = AddComponent<Camera>(eCam);
		auto* camController = AddComponent<CameraFPSController>(eCam);

		// Sun

		Entity sun = CreateEntity();
		auto* sunTr = AddComponent<Transform>(sun);
		sunTr->SetWorldScale({ 2.0f, 2.0f, 2.0f });

		auto* sunCube = AddComponent<SphereComponent>(sun);
		sunCube->SetCamera(cam);
		sunCube->SetColor(Color::yellow);

		auto* sunRotate = AddComponent<ResetAndRotate>(sun);
		sunRotate->spinSpeed = 1.0f;

		const int planetCount = 8;
		const int moonCount = 5;

		const float4 planetColors[8] = { Color::red, Color::purple, Color::green, Color::red, Color::cyan, Color::white, Color::green, Color::blue };
		const float4 moonColors[5] = { Color::red, Color::green, Color::blue, Color::yellow, Color::cyan };

		/* Planets */
		for (size_t i = 0; i < planetCount; i++)
		{
			const float posOffset = 40.0f;
			const float posFactor = 50.0f;
			const float spinOffset = 5.0f;
			const float spinFactor = -0.5f;
			const float orbitOffset = 0.196f;
			const float orbitFactor = -0.0318298f;

			Entity planet = CreateEntity();
			auto* planetTr = AddComponent<Transform>(planet);
			planetTr->SetParent(sunTr);
			planetTr->SetWorldScale(float3{ 1.0f, 1.0f, 1.0f });

			auto* planetCube = AddComponent<CubeComponent>(planet);
			planetCube->SetCamera(cam);
			planetCube->SetColor(planetColors[i]);

			auto* resetPlanet = AddComponent<ResetAndRotate>(planet);
			resetPlanet->spinSpeed = spinOffset + i * spinFactor;
			resetPlanet->orbitRadius = posOffset + i * posFactor;
			resetPlanet->orbitSpeed = orbitOffset + i * i * orbitFactor;

			planetTr->SetWorldTranslation(sunTr->GetWorldTranslation() + resetPlanet->orbitRadius * float3{ 10, 0.0f, 0 });
			
			//std::cout << "PLANET " << i << " " << planetTr->GetWorldTranslation().x << " " << planetTr->GetWorldTranslation().y << " " << planetTr->GetWorldTranslation().z << std::endl;
			//std::cout << "PLANET " << i << " " << planetTr->GetWorldTranslation().x << " " << planetTr->GetWorldTranslation().y << " " << planetTr->GetWorldTranslation().z << std::endl;

			if (i == 1)
				resetPlanet->spinSpeed *= 4;

			/* Moons */
			for (size_t j = 0; j < moonCount; j++)
			{
				const float mposOffset = 10.0f;
				const float mposFactor = 3.0f;
				const float mspinOffset = 5.0f;
				const float mspinFactor = -0.5f;
				const float morbitOffset = 0.96f;
				const float morbitFactor = -0.02818298f;

				Entity moon = CreateEntity();
				auto* moonTr = AddComponent<Transform>(moon);
				moonTr->SetParent(planetTr);
				moonTr->SetWorldScale({ 0.5f, 0.5f, 0.5f });

				auto* moonCube = AddComponent<CubeComponent>(moon);
				moonCube->SetCamera(cam);
				moonCube->SetColor(moonColors[j]);

				auto* resetMoon = AddComponent<ResetAndRotate>(moon);
				resetMoon->spinSpeed = mspinOffset + j * mspinFactor;
				resetMoon->orbitRadius = mposOffset + j * mposFactor;
				resetMoon->orbitSpeed = morbitOffset + j * j * morbitFactor;

				moonTr->SetWorldTranslation(planetTr->GetWorldTranslation() + resetMoon->orbitRadius * float3{ 10, 0.0f, 0 });

				//std::cout << "MOON " << j << " " << moonTr->GetLocalTranslation().x << " " << moonTr->GetLocalTranslation().y << " " << moonTr->GetLocalTranslation().z << std::endl;
				//std::cout << "MOON " << j << " " << moonTr->GetWorldTranslation().x << " " << moonTr->GetWorldTranslation().y << " " << moonTr->GetWorldTranslation().z << std::endl;
			}
		}
		
		ModelImporter importer{};

		Entity eMesh = CreateEntity();
		AddComponent<Transform>(eMesh);

		const auto meshes = importer.ImportMeshes("C:/Users/Vladislav/source/repos/Direct11Project/Models/SM_Sofa.fbx");
		for (int i = 0; i < meshes.size(); ++i)
		{
			auto* mComp = AddComponent<MeshComponent>(eMesh);
			mComp->SetCamera(cam);
			mComp->SetMesh(meshes[i]);
		}
	}
};
