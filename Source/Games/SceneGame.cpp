#include "SceneGame.h"

#include "Components/Camera/Camera.h"
#include "Components/Camera/CameraFPSController.h"
#include "Components/CubeComponent.h"
#include "Components/KatamariPlayer.h"
#include "Components/MeshComponent.h"
#include "Components/MeshSpawner.h"
#include "Components/SphereComponent.h"
#include "Components/ResetAndRotate.h"
#include "Components/SphereCollider.h"
#include "Core/Color.h"
#include "Graphics/ModelImporter.h"

Entity SceneGame::Instantiate(const std::wstring& name, float scale)
{
	Entity e = CreateEntity();
	auto* tr = AddComponent<Transform>(e);
	tr->SetParent(nullptr);

	const auto meshes = ModelImporter::ImportMeshes(L"C:/Users/Vladislav/source/repos/Direct11Project/Models/SM_" + name + L".fbx", { scale });
	const auto texture = Texture2D::LoadFromFile(L"C:/Users/Vladislav/source/repos/Direct11Project/Models/T_" + name + L"_D.png");

	float3 min = meshes[0]->GetBounds().Min();
	float3 max = meshes[0]->GetBounds().Max();
	for (int i = 1; i < meshes.size(); i++)
	{
		min = float3::Min(min, meshes[i]->GetBounds().Min());
		max = float3::Max(max, meshes[i]->GetBounds().Max());
	}
	
	for (int i = 0; i < meshes.size(); ++i)
	{
		Entity eComp = CreateEntity();
		auto* eCompTr = AddComponent<Transform>(eComp);
		eCompTr->SetParent(tr);
		eCompTr->SetLocalTranslation(-Bounds::Center(min, max));
		
		auto* mComp = AddComponent<MeshComponent>(eComp);
		mComp->SetMesh(meshes[i]);
		mComp->SetMainTexture(texture);
	}

	const float3 scale3 = Bounds::Extents(min, max);
	tr->SetWorldScale({ 1.0f / scale3.x, 1.0f / scale3.y, 1.0f / scale3.z });

	return e;
}
	
	SceneGame::SceneGame() : Game()
{
	// Camera

	Entity eCam = CreateEntity();
	auto* camTr = AddComponent<Transform>(eCam);
	camTr->SetLocalTranslation({ 0.0f, 0.0f, 0.0f });
	camTr->SetLocalEuler({ 0.0f, 0.0f, 0.0f });

	auto* cam = AddComponent<Camera>(eCam);
	auto* camController = AddComponent<CameraFPSController>(eCam);

	std::wstring items[] = { L"Beachball", L"Apple", L"Barrel", L"Tree_1", L"Hammer", L"Boulder" };

	// Items
	MeshSpawner spawner{ items, ARRAYSIZE(items) };

	const int n = 20;
	std::vector<SphereCollider*> colliders;
	
	for (int i = 0; i < n; i++)
	{
		const auto item = spawner.InstantiateRandomMesh();
		auto* col = AddComponent<SphereCollider>(item);
		colliders.push_back(col);

		
		auto e = CreateEntity();
		AddComponent<Transform>(e);
		col->wireframe = AddComponent<SphereComponent>(e);
		col->wireframe->SetShader(Shaders::Get(L"C:/Users/Vladislav/source/repos/Direct11Project/x64/Debug/Shaders/White.hlsl", Position | VertexColor | UV0, false));
	}
	
	// Meshes
	auto sofa = Instantiate(L"Sofa", .005f);
	GetComponent<Transform>(sofa)->SetWorldScale({1, 1, 1});
	auto* sofaCol = AddComponent<SphereCollider>(sofa);
	auto* kat = AddComponent<KatamariPlayer>(sofa);
	kat->SetCamera(cam);
	kat->SetItems(colliders);
	camController->SetTarget(GetComponent<Transform>(sofa));

	auto ewf = CreateEntity();
	AddComponent<Transform>(ewf);
	sofaCol->wireframe = AddComponent<SphereComponent>(ewf);
	sofaCol->wireframe->SetShader(Shaders::Get(L"C:/Users/Vladislav/source/repos/Direct11Project/x64/Debug/Shaders/White.hlsl", Position | VertexColor | UV0, false));
	sofaCol->wireframe->SetColor(Color::purple);
	// Sun

	/*Entity sun = CreateEntity();
	auto* sunTr = AddComponent<Transform>(sun);
	sunTr->SetWorldScale({ 2.0f, 2.0f, 2.0f });
	sunTr->SetWorldTranslation({-3, 3, 10});
	
	auto* sunCube = AddComponent<SphereComponent>(sun);
	sunCube->SetColor(Color::yellow);

	auto* sunRotate = AddComponent<ResetAndRotate>(sun);
	sunRotate->spinSpeed = 1.0f;

	const int planetCount = 8;
	const int moonCount = 5;

	const float4 planetColors[8] = { Color::red, Color::purple, Color::green, Color::red, Color::cyan, Color::white, Color::green, Color::blue };
	const float4 moonColors[5] = { Color::red, Color::green, Color::blue, Color::yellow, Color::cyan };
	
	// Planets
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

		// Moons
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
			moonCube->SetColor(moonColors[j]);

			auto* resetMoon = AddComponent<ResetAndRotate>(moon);
			resetMoon->spinSpeed = mspinOffset + j * mspinFactor;
			resetMoon->orbitRadius = mposOffset + j * mposFactor;
			resetMoon->orbitSpeed = morbitOffset + j * j * morbitFactor;

			moonTr->SetWorldTranslation(planetTr->GetWorldTranslation() + resetMoon->orbitRadius * float3{ 10, 0.0f, 0 });

			//std::cout << "MOON " << j << " " << moonTr->GetLocalTranslation().x << " " << moonTr->GetLocalTranslation().y << " " << moonTr->GetLocalTranslation().z << std::endl;
			//std::cout << "MOON " << j << " " << moonTr->GetWorldTranslation().x << " " << moonTr->GetWorldTranslation().y << " " << moonTr->GetWorldTranslation().z << std::endl;
		}
	}*/
}