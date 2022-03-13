#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
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

void SceneManager::LoadScene(std::wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	// TEST
	/*_activeScene = */LoadTestScene();

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
	_activeScene = scene;
	
//#pragma region Camera
//	{
//		std::shared_ptr<GameObject> camera = std::make_shared<GameObject>();
//		camera->SetName(L"Main_Camera");
//		camera->AddComponent(std::make_shared<Transform>());
//		camera->AddComponent(std::make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
//		camera->AddComponent(std::make_shared<TestCameraScript>());
//		camera->GetCamera()->SetFar(10000.f);
//		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
//		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
//		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
//		scene->AddGameObject(camera);
//	}	
//#pragma endregion
//
//#pragma region UI_Camera
//	{
//		std::shared_ptr<GameObject> camera = std::make_shared<GameObject>();
//		camera->SetName(L"Orthographic_Camera");
//		camera->AddComponent(std::make_shared<Transform>());
//		camera->AddComponent(std::make_shared<Camera>()); // Near=1, Far=1000, 800*600
//		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
//		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
//		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
//		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
//		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
//		scene->AddGameObject(camera);
//	}
//#pragma endregion
//
//#pragma region UI_Test
//	for (int32 i = 0; i < 6; i++)
//	{
//		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
//		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
//		obj->SetName(L"RenderTarget UI " + std::to_wstring(i)); // UI
//		obj->AddComponent(std::make_shared<Transform>());
//		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(-550.f + (i * 120), 300.f, 500.f));
//		std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
//		{
//			std::shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
//			meshRenderer->SetMesh(mesh);
//		}
//		{
//			std::shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
//
//			std::shared_ptr<Texture> texture;
//			if (i < 3)
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
//			else if (i < 5)
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
//			else
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
//
//			std::shared_ptr<Material> material = std::make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			meshRenderer->SetMaterial(material);
//		}
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//#pragma endregion
//
//#pragma region Directional Light
//	{
//		std::shared_ptr<GameObject> light = std::make_shared<GameObject>();
//		light->SetName(L"DirLight");
//		light->AddComponent(std::make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(0, 300, 0));
//		light->AddComponent(std::make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 1.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
//		light->GetLight()->SetAmbient(Vec3(0.5f, 0.5f, 0.5f));
//		light->GetLight()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//
//#pragma region FBX
//	{
//		std::shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Dragon.fbx", true);
//
//		std::vector<std::shared_ptr<GameObject>> gameObjects = meshData->Instantiate();
//
//		for (auto& gameObject : gameObjects)
//		{
//			gameObject->SetName(L"Dragon");
//			gameObject->SetCheckFrustum(false);
//			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 300.f));
//			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
//			scene->AddGameObject(gameObject);
//			gameObject->AddComponent(std::make_shared<TestDragon>());
//		}
//		/*std::shared_ptr<GameObject> loadDragonTest = std::make_shared<GameObject>();
//		loadDragonTest->AddComponent(std::make_shared<MeshRenderer>());
//		loadDragonTest->AddComponent(std::make_shared<Transform>());
//		std::shared_ptr<Animator> ani = std::make_shared<Animator>();
//		ani->SetBones(loadDragonTest->GetMeshRenderer()->GetMesh()->GetBones());
//		ani->SetAnimClip(loadDragonTest->GetMeshRenderer()->GetMesh()->GetAnimClip());
//		loadDragonTest->AddComponent(ani);
//		GET_SINGLE(JsonManager)->Load("../Output/Dragon", loadDragonTest);
//		loadDragonTest->SetName(L"Dragon");
//		scene->AddGameObject(loadDragonTest);*/
//	}
//#pragma endregion

	GET_SINGLE(JsonManager)->LoadScene("../Output/TestScene", scene);
	return scene;
}