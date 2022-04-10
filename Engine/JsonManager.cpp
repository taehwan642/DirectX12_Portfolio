#include "pch.h"
#include "JsonManager.h"
#include "RttrRegistration.h"
#include "to_json.h"
#include "from_json.h"
#include "Resources.h"
#include "FBXLoader.h"
#include "StructuredBuffer.h"

void JsonManager::Save(const std::string& path, std::shared_ptr<GameObject> object)
{
	std::string jsonString = io::to_json(*object);
	std::ofstream ost(path + ".json");
	ost << jsonString;

	RTTRGameObjectValue value = RTTRGameObjectValue(object);
	std::string jsonString2 = io::to_json(value);
	std::ofstream ost2(path + "_value.json");
	ost2 << jsonString2;
}

void JsonManager::SaveScene(const std::string& path, std::shared_ptr<Scene> scene)
{
#ifndef TESTGAME
	if (scene->GetMainCamera() != nullptr)
		scene->RemoveGameObject(scene->GetMainCamera()->GetGameObject());
#endif

	std::string jsonString = io::to_json(*scene);
	std::ofstream ost(path + ".json");
	ost << jsonString;

	RTTRSceneValue value = RTTRSceneValue(scene);
	std::string jsonString2 = io::to_json(value);
	std::ofstream ost2(path + "_value.json");
	ost2 << jsonString2;

#ifndef TESTGAME
	std::shared_ptr<GameObject> toolCamera = std::make_shared<GameObject>();
	toolCamera->AddComponent(std::make_shared<TransformComponent>());
	toolCamera->AddComponent(std::make_shared<Camera>());
	toolCamera->AddComponent(std::make_shared<TestCameraScript>());
	toolCamera->SetName(L"ToolCamera");
	toolCamera->GenerateHash();

	std::vector<std::shared_ptr<GameObject>> obj;
	for (auto& iter : scene->_cameras)
	{
		obj.push_back(iter->GetGameObject());
	}
	for (auto& iter : obj)
	{
		scene->RemoveGameObject(iter);
	}
	scene->AddGameObject(toolCamera);
	for (auto& iter : obj)
		scene->AddGameObject(iter);
#endif
}

void JsonManager::SaveMeshData(const std::string& path, std::shared_ptr<MeshData> data)
{
	RTTRMeshDataValue value = RTTRMeshDataValue(data);
	std::string jsonString2 = io::to_json(value);
	std::ofstream ost2(path + "_value.json");
	ost2 << jsonString2;
}

void JsonManager::SavePrefab(const std::string& path, std::shared_ptr<GameObject> object)
{
	// GameObject에 있는 child들도 저장.
	std::string jsonString = io::to_json(*object);
	std::ofstream ost(path + ".json");
	ost << jsonString;
}

bool JsonManager::Load(const std::string& path, std::shared_ptr<GameObject> object)
{
	std::string valueString;

	std::ifstream valueFile;
	valueFile.open(path + "_value.json");
	if (valueFile.is_open())
	{
		while (!valueFile.eof())
		{
			char arr[256];
			valueFile.getline(arr, 256);
			valueString += arr;
		}
	}
	valueFile.close();

	if (valueString.empty())
		return false;

	RTTRGameObjectValue value;
	io::from_json(valueString, value);

	LoadGameObject(value, object);

	std::string dataString;

	std::ifstream dataFile;
	dataFile.open(path + ".json");
	if (dataFile.is_open())
	{
		while (!dataFile.eof())
		{
			char arr[256];
			dataFile.getline(arr, 256);
			dataString += arr;
		}
	}
	dataFile.close();

	if (dataString.empty())
		return false;

	io::from_json(dataString, object);

	return true;
}

bool JsonManager::LoadScene(const std::string& path, std::shared_ptr<Scene> scene)
{
	hashValues.clear();
	std::string valueString;

	std::ifstream valueFile;
	valueFile.open(path + "_value.json");
	if (valueFile.is_open())
	{
		while (!valueFile.eof())
		{
			char arr[256];
			valueFile.getline(arr, 256);
			valueString += arr;
		}
	}
	valueFile.close();

	if (valueString.empty())
		return false;

	RTTRSceneValue sceneValue;
	io::from_json(valueString, sceneValue);

	for (auto& iter : sceneValue.meshDataResources)
	{
		// 주소에서 obj 이름만 가져오려면, 맨 마지막 위치에서 //를 만나기 전까지.
		std::wstring name = s2ws(iter);
		std::wstring objString = name;
		if (size_t pos = name.find_last_of(L"\\"); pos != std::wstring::npos)
		{
			objString = name.substr(pos + 1, name.size());
		}

		// obj 이름은 맨 처음 "_"를 만나기 전.
		std::wstring objName = objString;
		if (size_t pos = objString.find_last_of(L"_"); pos != std::wstring::npos)
		{
			objName = objString.substr(0, pos);
		}

		std::shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(s2ws(iter.c_str()).c_str());
		meshData->SetMeshName(objName);
		meshData->Instantiate();
	}

	for (auto& iter : sceneValue.textureResources)
	{
		// texture load
		std::shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(s2ws(iter.c_str()).c_str(), s2ws(iter.c_str()).c_str());
	}

	for (int i = 0; i < sceneValue.gameObjects.size(); ++i)
	{
		RTTRGameObjectValue value = sceneValue.gameObjects[i];
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>();

		LoadGameObject(value, object, i);

		scene->AddGameObject(object);
	}

	std::string dataString;

	std::ifstream dataFile;
	dataFile.open(path + ".json");
	if (dataFile.is_open())
	{
		while (!dataFile.eof())
		{
			char arr[256];
			dataFile.getline(arr, 256);
			dataString += arr;
		}
	}
	dataFile.close();

	if (dataString.empty())
		return false;

	io::from_json(dataString, scene);

#ifndef TESTGAME
	if (scene->_cameras.size() != 0)
	{
		std::shared_ptr<GameObject> toolCamera = std::make_shared<GameObject>();
		toolCamera->AddComponent(std::make_shared<TransformComponent>());
		toolCamera->AddComponent(std::make_shared<Camera>());
		toolCamera->AddComponent(std::make_shared<TestCameraScript>());
		toolCamera->SetName(L"ToolCamera");
		toolCamera->GenerateHash();

		std::vector<std::shared_ptr<GameObject>> obj;
		for (auto& iter : scene->_cameras)
		{
			obj.push_back(iter->GetGameObject());
		}
		for (auto& iter : obj)
		{
			scene->RemoveGameObject(iter);
		}
		scene->AddGameObject(toolCamera);
		for (auto& iter : obj)
			scene->AddGameObject(iter);
	}
#endif

	for (int i = 0; i < hashValues.size(); ++i)
	{
		// first = 자기 자신의 hash
		// 자기 자신의 hash로 자기 자신 찾기
		auto childIterator = std::find_if(scene->_gameObjects.begin(), scene->_gameObjects.end(), 
			[&](std::shared_ptr<GameObject> obj) 
			{
				return (obj->GetHash() == hashValues[i].first);
			});

		// second = 부모의 hash
		// 부모의 hash로 부모 찾기
		auto parentIterator = std::find_if(scene->_gameObjects.begin(), scene->_gameObjects.end(),
			[&](std::shared_ptr<GameObject> obj)
			{
				return (obj->GetHash() == hashValues[i].second);
			});

		assert(childIterator != scene->_gameObjects.end() && parentIterator != scene->_gameObjects.end());

		(*childIterator)->GetTransform()->SetParent((*parentIterator)->GetTransform());
	}

	return true;
}

bool JsonManager::LoadMeshData(const std::string& path, std::shared_ptr<MeshData> data)
{
	std::string valueString;

	std::ifstream valueFile;
	valueFile.open(path + "_value.json");
	if (valueFile.is_open())
	{
		while (!valueFile.eof())
		{
			char arr[256];
			valueFile.getline(arr, 256);
			valueString += arr;
		}
	}
	valueFile.close();

	if (valueString.empty())
		return false;

	RTTRMeshDataValue mdValue;
	io::from_json(valueString, mdValue);

	for (int i = 0; i < mdValue.meshRenderValue; ++i)
	{
		mdValue.tag;
		
		MeshRenderInfo info;

		// Animation
		data->_meshRenders[i].mesh->_animClips = mdValue.animationValue[i].animClips;
		data->_meshRenders[i].mesh->_bones = mdValue.animationValue[i].bones;

		std::shared_ptr<Mesh> mesh = data->_meshRenders[i].mesh;
		// BoneOffet 행렬
		const int32 boneCount = static_cast<int32>(mesh->_bones.size());
		std::vector<Matrix> offsetVec(boneCount);
		for (size_t b = 0; b < boneCount; b++)
			offsetVec[b] = mesh->_bones[b].matOffset;

		// OffsetMatrix StructuredBuffer 세팅
		// Size가 0이라면 Buffer를 만들지 않아도 됨.
		if (mesh->_animClips.size() != 0)
		{
			mesh->_offsetBuffer = std::make_shared<StructuredBuffer>();
			mesh->_offsetBuffer->Init(sizeof(Matrix), static_cast<uint32>(offsetVec.size()), offsetVec.data());
		}

		const int32 animCount = static_cast<int32>(mesh->_animClips.size());
		for (int32 i = 0; i < animCount; i++)
		{
			// 클립 i번째
			AnimClipInfo& animClip = mesh->_animClips[i];

			// 애니메이션 프레임 정보
			std::vector<AnimFrameParams> frameParams;
			// 뼈 개수 * 클립의 프레임 카운트
			frameParams.resize(mesh->_bones.size() * animClip.frameCount);

			for (int32 b = 0; b < boneCount; b++)
			{
				// 이 때 키프레임의 사이즈 : 키프레임 카운트?
				const int32 keyFrameCount = min(static_cast<int32>(animClip.keyFrames[b].size()), animClip.frameCount);
				
				for (int32 f = 0; f < keyFrameCount; f++)
				{
					int32 idx = static_cast<int32>(boneCount * f + b);

					frameParams[idx] = AnimFrameParams
					{
						Vec4(animClip.keyFrames[b][f].scale),
						animClip.keyFrames[b][f].rotation, // Quaternion
						Vec4(animClip.keyFrames[b][f].translate)
					};
				}
			}

			// StructuredBuffer 세팅
			mesh->_frameBuffer.push_back(std::make_shared<StructuredBuffer>());
			mesh->_frameBuffer.back()->Init(sizeof(AnimFrameParams), static_cast<uint32>(frameParams.size()), frameParams.data());
		}
	}
	
	return true;
}

void JsonManager::LoadPrefab(const std::string& path, std::shared_ptr<GameObject> object)
{
}

void JsonManager::LoadGameObject(RTTRGameObjectValue value, std::shared_ptr<GameObject> object, int index)
{
	std::shared_ptr<Mesh> mesh = nullptr;
	object->SetName(s2ws(value.tag).c_str());

	object->_hash = value.hashValue;

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::TRANSFORM)] == true)
	{
		object->AddComponent(std::make_shared<TransformComponent>());
		if (value.transformValue.parentHashValue != 0)
		{
			Pair pair;
			pair.first = value.hashValue;
			pair.second = value.transformValue.parentHashValue;
			hashValues.push_back(pair);
		}
	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::MESH_RENDERER)] == true)
	{
		std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
		std::wstring meshTag = s2ws(value.meshRendererValue.meshValue.tag);

		bool isLoaded = false;

		if (meshTag == L"Point")
		{
			mesh = GET_SINGLE(Resources)->LoadPointMesh();
		}
		else if (meshTag == L"Rectangle")
		{
			mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		}
		else if (meshTag == L"Cube")
		{
			mesh = GET_SINGLE(Resources)->LoadCubeMesh();
		}
		else if (meshTag == L"Sphere")
		{
			mesh = GET_SINGLE(Resources)->LoadSphereMesh();
		}
		/*else if (meshTag == L"Terrain")
		{
			mesh = GET_SINGLE(Resources)->LoadTerrainMesh();
		}*/
		else
		{
			// mesh = GET_SINGLE(Resources)->Get<Mesh>(meshTag);
			// 앞에 fbx 가져오기
			std::wstring name = s2ws(value.tag);
			std::wstring objString = name;
			if (size_t pos = name.find_last_of(L"."); pos != std::wstring::npos)
			{
				// .fbx 까지
				objString = name.substr(0, pos + 4);
			}

			std::shared_ptr<GameObject> obj = GET_SINGLE(Resources)->Get<GameObject>(objString + meshTag);
			mesh = obj->GetMeshRenderer()->GetMesh();
			if (obj != nullptr)
			{
				object->AddComponent(obj->_meshRenderer->Clone());
				for (int i = 0; i < value.meshRendererValue.materialValues.size(); ++i)
				{
					object->GetMeshRenderer()->GetMaterial(i)->SetShader(GET_SINGLE(Resources)->Get<Shader>(s2ws(value.meshRendererValue.materialValues[i].shaderValue.tag)));
				}
				isLoaded = true;
			}
		}

		if (isLoaded != true)
		{
			for (int i = 0; i < value.meshRendererValue.meshRendererMaterialNumValue; ++i)
			{
				std::shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(s2ws(value.meshRendererValue.materialValues[i].tag));
				if (material == nullptr)
				{
					material = std::make_shared<Material>();

					for (int j = 0; j < MATERIAL_ARG_COUNT; ++j)
					{
						if (value.meshRendererValue.materialValues[i].textureOnValue[j] == false)
							continue;

						std::shared_ptr<Texture> texture =
							GET_SINGLE(Resources)->Get<Texture>(s2ws(value.meshRendererValue.materialValues[i].textureValues[j].tag));

						assert(texture != nullptr);

						material->SetTexture(j, texture);
					}

					std::shared_ptr<Shader> shader =
						GET_SINGLE(Resources)->Get<Shader>(s2ws(value.meshRendererValue.materialValues[i].shaderValue.tag));

					assert(shader != nullptr);

					material->SetShader(shader);
				}
				mr->SetMesh(mesh);
				mr->SetMaterial(material, i);
				object->AddComponent(mr);
			}
		}
	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::CAMERA)] == true)
		object->AddComponent(std::make_shared<Camera>());

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::LIGHT)] == true)
	{
		std::shared_ptr<Light> light = std::make_shared<Light>();
		object->AddComponent(light);
		light->SetLightType(static_cast<LIGHT_TYPE>(value.lightValue.lightType));
	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::PARTICLE_SYSTEM)] == true)
	{
		std::shared_ptr<ParticleSystem> ps = std::make_shared<ParticleSystem>();
		std::shared_ptr<Texture> texture = GET_SINGLE(Resources)->Get<Texture>(s2ws(value.particleSystemValue.materialValue.textureValues[0].tag));
		ps->_material->SetTexture(0, texture);
		object->AddComponent(ps);
	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::TERRAIN)] == true)
	{
		std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>();
		object->AddComponent(terrain);
		terrain->Init(value.terrainValue.sizeX, value.terrainValue.sizeZ);

	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::COLLIDER)] == true)
	{
		switch (value.colliderValue.type)
		{
		case ColliderType::Sphere:
			object->AddComponent(std::make_shared<SphereCollider>());
			break;
		case ColliderType::Cube:
			object->AddComponent(std::make_shared<CubeCollider>());
			break;
		case ColliderType::Mesh:
		{
			std::shared_ptr<MeshCollider> mc = std::make_shared<MeshCollider>();
			object->AddComponent(mc);
			mc->Init();
			break;
		}
		case ColliderType::Bone:
		{
			std::shared_ptr<BoneCollider> bc = std::make_shared<BoneCollider>();
			object->AddComponent(bc);
			for (int i = 0; i < value.colliderValue.visualizerSize; ++i)
			{
				bc->_colliderVisualizers.push_back(std::make_shared<Visualizer>());
			}
			break;
		}
		default:
			break;
		}
	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::ANIMATOR)] == true)
	{
		std::shared_ptr<Animator> animator = std::make_shared<Animator>();
		object->AddComponent(animator);
		animator->SetBones(mesh->GetBones());
		animator->SetAnimClip(mesh->GetAnimClip());
	}
	
	LoadMonobehaviour(value, object);
}

void JsonManager::LoadMonobehaviour(RTTRGameObjectValue value, std::shared_ptr<GameObject> object)
{
	RTTRMONOLOAD(GameManagerScript)
	RTTRMONOLOAD(TerrainScript)
	RTTRMONOLOAD(TestCameraScript)
	RTTRMONOLOAD(TestDragon)
	RTTRMONOLOAD(Sea)
	RTTRMONOLOAD(EnemyBullet)
	RTTRMONOLOAD(EffectManagerScript)
	RTTRMONOLOAD(Effect)
	RTTRMONOLOAD(InfinitePassage)
}
