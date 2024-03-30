#include "SceneGame.h"

#include "Components/Camera/Camera.h"
#include "Components/Camera/CameraFPSController.h"
#include "Components/CubeComponent.h"
#include "Components/KatamariPlayer.h"
#include "Components/MeshComponent.h"
#include "Components/MeshSpawner.h"
#include "Components/ResetAndRotate.h"
#include "Components/SphereCollider.h"
#include "Graphics/ModelImporter.h"
#include "Components/Light/Light.h"
#include "Components/SphereComponent.h"

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
	}
	
	// Meshes
	auto sofa = Instantiate(L"Sofa", .005f);
	GetComponent<Transform>(sofa)->SetWorldScale({1, 1, 1});
	auto* sofaCol = AddComponent<SphereCollider>(sofa);
	auto* kat = AddComponent<KatamariPlayer>(sofa);
	kat->SetCamera(cam);
	kat->SetItems(colliders);
	camController->SetTarget(GetComponent<Transform>(sofa));

	// Lighting
	auto* camLight = AddComponent<Light>(sofa);
	camLight->SetColor(float4{ 0.506f, 0.435f, 0.549f, 1.0f } * 1.0f);
	camLight->SetLightType(Light::Point);
	camLight->SetRange(7.0f);

	auto mainLightE = CreateEntity();
	auto* mainLight = AddComponent<Light>(mainLightE);
	auto* mainLightTr = AddComponent<Transform>(mainLightE);
	mainLight->SetLightType(Light::Directional);
	mainLightTr->SetWorldEuler(float3{ 60.0f, 0.0f, 0.0f });
	mainLight->SetColor(Color::white * 0.25f);

	auto testLightE = CreateEntity();
	auto* testLight = AddComponent<Light>(testLightE);
	testLight->SetColor(Color::blue);
	testLight->SetLightType(Light::Point);
	testLight->SetRange(5.0f);

	auto* testLightTr = AddComponent<Transform>(testLightE);
	auto* move = AddComponent<ResetAndRotate>(testLightE);
	move->orbitRadius = 10;
	move->orbitSpeed = 1;

	// Floor
	auto f = Instantiate(L"Plane", 1.0f);
	auto* ftr = GetComponent<Transform>(f);
	ftr->SetWorldScale(float3{ .25f, .25f, .25f });
	ftr->SetWorldRotation(quaternion::Identity);
	ftr->SetWorldTranslation(float3{ 0.0f, -0.5f, 0.0f });
}