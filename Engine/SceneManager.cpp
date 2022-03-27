#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "Light.h"

#include "TestCameraScript.h"
#include "Resources.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "SphereCollider.h"
#include "MeshData.h"
#include "TestDragon.h"

#include "ImGuiManager.h"
#include "CubeCollider.h"
#include "TerrainScript.h"

#include "JsonManager.h"
#include "Animator.h"

#include "MeshCollider.h"

void SceneManager::Init()
{
	_activeScene = std::make_shared<Scene>();
	// ONLY FOR DEBUG
	GET_SINGLE(Resources)->LoadCubeMesh();
	GET_SINGLE(Resources)->LoadPointMesh();
	GET_SINGLE(Resources)->LoadRectangleMesh();
	GET_SINGLE(Resources)->LoadSphereMesh();

	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
}

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

// TEMP
void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

void SceneManager::LoadScene(const std::wstring& sceneName)
{
	//if (_activeScene)
	//{
	//	// TODO : 기존 Scene 정리
	//	// TODO : 파일에서 Scene 정보 로드
	//}
	//else
	//{
	//	_activeScene = std::make_shared<Scene>();
	//}

	GET_SINGLE(JsonManager)->LoadScene(ws2s(sceneName).c_str(), _activeScene);

	// TEST
	//_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::SetLayerName(uint8 index, const std::wstring& name)
{
	// 기존 데이터 삭제
	const std::wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const std::wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}

std::shared_ptr<Scene> SceneManager::LoadTestScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

#pragma region ComputeShader
	{
		std::shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		// UAV 용 Texture 생성
		std::shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		std::shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVHandle(), UAV_REGISTER::u0);

		// 쓰레드 그룹 (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
#pragma endregion

	std::shared_ptr<Scene> scene = std::make_shared<Scene>();

#pragma region Camera
	{
		std::shared_ptr<GameObject> camera = std::make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(std::make_shared<TransformComponent>());
		camera->AddComponent(std::make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		camera->AddComponent(std::make_shared<TestCameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		std::shared_ptr<GameObject> camera = std::make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(std::make_shared<TransformComponent>());
		camera->AddComponent(std::make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		std::shared_ptr<GameObject> skybox = std::make_shared<GameObject>();
		skybox->SetName(L"SkyBox");
		skybox->AddComponent(std::make_shared<TransformComponent>());
		skybox->SetCheckFrustum(false);
		std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
		{
			std::shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			std::shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			std::shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg");
			std::shared_ptr<Material> material = std::make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Object
	{
		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
		obj->SetName(L"GameObject");
		obj->AddComponent(std::make_shared<TransformComponent>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0, 0.f, 500.f));
		obj->SetStatic(false);
		std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
		{
			std::shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			std::shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Plane
	{
		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
		obj->SetName(L"Plane");
		obj->AddComponent(std::make_shared<TransformComponent>());
		obj->GetTransform()->SetLocalScale(Vec3(1000.f, 1.f, 1000.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, -100.f, 500.f));
		obj->SetStatic(true);
		std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
		{
			std::shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			std::shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject")->Clone();
			material->SetInt(0, 0);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 6; i++)
	{
		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->SetName(L"UI " + std::to_wstring(i));
		obj->AddComponent(std::make_shared<TransformComponent>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));
		std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
		{
			std::shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			std::shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			std::shared_ptr<Texture> texture;
			if (i < 3)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			else if (i < 5)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			else
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);

			std::shared_ptr<Material> material = std::make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Directional Light
	{
		std::shared_ptr<GameObject> light = std::make_shared<GameObject>();
		light->SetName(L"Light");
		light->AddComponent(std::make_shared<TransformComponent>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 1000, 500));
		light->AddComponent(std::make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, -1, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}
#pragma endregion
	GET_SINGLE(JsonManager)->SaveScene("../Output/ShadowTest", scene);
	return scene;
}