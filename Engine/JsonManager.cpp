#include "pch.h"
#include "JsonManager.h"
#include "RttrRegistration.h"
#include "to_json.h"
#include "from_json.h"
#include "Resources.h"
#include "FBXLoader.h"
#include "StructuredBuffer.h"
#include "Engine.h"

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
#ifdef TOOL
	if (!GEngine->GetIsGamePlaying())
	{
		// Tool Camera ????
		if (scene->FindGameObject(L"ToolCamera") != nullptr)
			scene->RemoveGameObject(scene->GetMainCamera()->GetGameObject());
	}
#endif

	std::string jsonString = io::to_json(*scene);
	std::ofstream ost(path + ".json");
	ost << jsonString;

	RTTRSceneValue value = RTTRSceneValue(scene);
	std::string jsonString2 = io::to_json(value);
	std::ofstream ost2(path + "_value.json");
	ost2 << jsonString2;

#ifdef TOOL
	if (!GEngine->GetIsGamePlaying())
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
	// GameObject?? ?ִ? child?鵵 ????.
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
		// ?ּҿ??? obj ?̸??? ??????????, ?? ?????? ??ġ???? //?? ?????? ??????.
		std::wstring name = s2ws(iter);
		std::wstring objString = name;
		if (size_t pos = name.find_last_of(L"\\"); pos != std::wstring::npos)
		{
			objString = name.substr(pos + 1, name.size());
		}

		// obj ?̸??? ?? ó?? "_"?? ?????? ??.
		std::wstring objName = objString;
		if (size_t pos = objString.find_last_of(L"_"); pos != std::wstring::npos)
		{
			objName = objString.substr(0, pos);
		}

		std::shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(s2ws(iter));
		meshData->SetMeshName(objName);
		meshData->Instantiate();
	}

	for (auto& iter : sceneValue.textureResources)
	{
		// texture load
		std::shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(s2ws(iter), s2ws(iter));
	}

	for (auto& iter : sceneValue.audioResources)
	{
		// audio load
		std::shared_ptr<AudioClip> texture = GET_SINGLE(Resources)->Load<AudioClip>(s2ws(iter), s2ws(iter));
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

#ifdef TOOL
	if (!GEngine->GetIsGamePlaying())
	{
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
	}
#endif

	for (int i = 0; i < hashValues.size(); ++i)
	{
		// first = ?ڱ? ?ڽ??? hash
		// ?ڱ? ?ڽ??? hash?? ?ڱ? ?ڽ? ã??
		auto childIterator = std::find_if(scene->_gameObjects.begin(), scene->_gameObjects.end(), 
			[&](std::shared_ptr<GameObject> obj) 
			{
				return (obj->GetHash() == hashValues[i].first);
			});

		// second = ?θ??? hash
		// ?θ??? hash?? ?θ? ã??
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
		// Animation
		std::shared_ptr<Mesh> mesh = data->_meshRenders[i].mesh;
		mesh->_animClips = mdValue.animationValue[0].animClips;
		mesh->_bones = mdValue.animationValue[0].bones;

		// encode?? Quaternion decode
		int keyFrameRotationIndex = 0;

		for (auto& iter : mesh->_animClips)
		{
			for (auto& boneKey : iter.keyFrames)
			{
				for (auto& keyFrame : boneKey)
				{
					quant::decode101010_quat(keyFrame.rotation, mdValue.animationValue[0].keyframeRotValue[keyFrameRotationIndex]);
					++keyFrameRotationIndex;
				}
			}
		}

		// BoneOffet ????
		const int32 boneCount = static_cast<int32>(mesh->_bones.size());
		std::vector<Matrix> offsetVec(boneCount);
		for (size_t b = 0; b < boneCount; b++)
			offsetVec[b] = mesh->_bones[b].matOffset;

		// OffsetMatrix StructuredBuffer ????
		// Size?? 0?̶??? Buffer?? ?????? ?ʾƵ? ??.
		if (mesh->_animClips.size() != 0)
		{
			mesh->_offsetBuffer = std::make_shared<StructuredBuffer>();
			mesh->_offsetBuffer->Init(sizeof(Matrix), static_cast<uint32>(offsetVec.size()), offsetVec.data());
		}

		const int32 animCount = static_cast<int32>(mesh->_animClips.size());
		for (int32 i = 0; i < animCount; i++)
		{
			// Ŭ?? i??°
			AnimClipInfo& animClip = mesh->_animClips[i];

			// ?ִϸ??̼? ?????? ????
			std::vector<AnimFrameParams> frameParams;
			// ?? ???? * Ŭ???? ?????? ī??Ʈ
			frameParams.resize(mesh->_bones.size() * animClip.frameCount);

			for (int32 b = 0; b < boneCount; b++)
			{
				// ?? ?? Ű???????? ?????? : Ű?????? ī??Ʈ?
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

			// StructuredBuffer ????
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
	object->SetName(s2ws(value.tag));

	object->_hash = value.hashValue;
	std::wstring objString;

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
		else if (meshTag == L"SquarePyramid")
		{
			mesh = GET_SINGLE(Resources)->LoadSquarePyramidMesh();
		}
		/*else if (meshTag == L"Terrain")
		{
			mesh = GET_SINGLE(Resources)->LoadTerrainMesh();
		}*/
		else
		{
			// mesh = GET_SINGLE(Resources)->Get<Mesh>(meshTag);
			// ?տ? fbx ????????
			std::wstring name = s2ws(value.tag);
			objString = name;
			if (size_t pos = name.find_last_of(L"."); pos != std::wstring::npos)
			{
				// .fbx ????
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
		case ColliderType::Box:
			object->AddComponent(std::make_shared<BoxCollider>());
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
		// ???? ?ִϸ??̼??? 1??????
		if (mesh->GetAnimClip()->size() == 1)
		{
			// ???? ?о?????
			std::vector<Vec2> vec;
			// objString?? ?޽????? ?̸?. ???⿡ _anim.txt?ٿ??? ?ҷ?????
			std::ifstream valueFile;
			valueFile.open("../Resources/FBX/" + ws2s(objString) + "_anim.txt");
			if (valueFile.is_open())
			{
				while (!valueFile.eof())
				{
					char arr[256];
					valueFile.getline(arr, 256);
					std::string line = arr;
					if (!line.empty())
					{
						if (size_t pos = line.find("~"); pos != std::string::npos)
						{
							// ~?? ???????? ?? ?? ?߶??? ?ֱ?
							std::string first = line.substr(0, pos);
							std::string second = line.substr(pos + 1, line.size());
							int firstValue = std::stoi(first);
							int secondValue = std::stoi(second);
							vec.push_back(Vec2(firstValue, secondValue));
						}
					}
				}
			}
			valueFile.close();
			std::shared_ptr<NierAnimator> animator = std::make_shared<NierAnimator>();
			object->AddComponent(animator);
			animator->SetBones(mesh->GetBones());
			animator->SetAnimClip(mesh->GetAnimClip());
			animator->SetAnimationFrames(vec);
		}
		else
		{
			std::shared_ptr<Animator> animator = std::make_shared<Animator>();
			object->AddComponent(animator);
			animator->SetBones(mesh->GetBones());
			animator->SetAnimClip(mesh->GetAnimClip());
		}
	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::AUDIOSOURCE)] == true)
	{
		std::shared_ptr<AudioSource> audioSource = std::make_shared<AudioSource>();
		object->AddComponent(audioSource);
		if (!value.audioSourceValue.tag.empty())
			audioSource->LoadAudio(s2ws(value.audioSourceValue.tag));
	}
	
	LoadMonobehaviour(value, object);
}

void JsonManager::LoadMonobehaviour(RTTRGameObjectValue value, std::shared_ptr<GameObject> object)
{
	RTTRMONOLOAD(GameManagerScript)
	RTTRMONOLOAD(TestCameraScript)
	RTTRMONOLOAD(Sea)
	RTTRMONOLOAD(EnemyBullet)
	RTTRMONOLOAD(EffectManagerScript)
	RTTRMONOLOAD(Effect)
	RTTRMONOLOAD(InfinitePassage)
	RTTRMONOLOAD(RenderTargetScript)
	RTTRMONOLOAD(PathTraceCameraScript)
	RTTRMONOLOAD(LaserScript)
	RTTRMONOLOAD(Player)
	RTTRMONOLOAD(Enemy)
	RTTRMONOLOAD(EnemySpawner)
}
