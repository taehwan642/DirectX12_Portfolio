#include "pch.h"
#include "JsonManager.h"
#include "RttrRegistration.h"
#include "to_json.h"
#include "from_json.h"
#include <fstream>
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
	std::string jsonString = io::to_json(*scene);
	std::ofstream ost(path + ".json");
	ost << jsonString;

	RTTRSceneValue value = RTTRSceneValue(scene);
	std::string jsonString2 = io::to_json(value);
	std::ofstream ost2(path + "_value.json");
	ost2 << jsonString2;
}

void JsonManager::SaveMeshData(const std::string& path, std::shared_ptr<MeshData> data)
{
	RTTRMeshDataValue value = RTTRMeshDataValue(data);
	std::string jsonString2 = io::to_json(value);
	std::ofstream ost2(path + "_value.json");
	ost2 << jsonString2;
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

	for (int i = 0; i < sceneValue.gameObjects.size(); ++i)
	{
		RTTRGameObjectValue value = sceneValue.gameObjects[i];
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>();

		LoadGameObject(value, object);

		scene->AddGameObject(object);
	}

	for (int i = 0; i < sceneValue.cameraObjects.size(); ++i)
	{
		RTTRGameObjectValue value = sceneValue.cameraObjects[i];
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>();

		LoadGameObject(value, object);

		scene->AddGameObject(object);
	}

	for (int i = 0; i < sceneValue.lightObjects.size(); ++i)
	{
		RTTRGameObjectValue value = sceneValue.lightObjects[i];
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>();

		LoadGameObject(value, object);

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

	for (int i = 0; i < hashValues.size(); ++i)
	{
		// first = �ڱ� �ڽ��� hash
		// �ڱ� �ڽ��� hash�� �ڱ� �ڽ� ã��
		auto childIterator = std::find_if(scene->_gameObjects.begin(), scene->_gameObjects.end(), 
			[&](std::shared_ptr<GameObject> obj) 
			{
				return (obj->GetHash() == hashValues[i].first);
			});

		// second = �θ��� hash
		// �θ��� hash�� �θ� ã��
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
		// BoneOffet ���
		const int32 boneCount = static_cast<int32>(mesh->_bones.size());
		std::vector<Matrix> offsetVec(boneCount);
		for (size_t b = 0; b < boneCount; b++)
			offsetVec[b] = mesh->_bones[b].matOffset;

		// OffsetMatrix StructuredBuffer ����
		mesh->_offsetBuffer = std::make_shared<StructuredBuffer>();
		mesh->_offsetBuffer->Init(sizeof(Matrix), static_cast<uint32>(offsetVec.size()), offsetVec.data());

		const int32 animCount = static_cast<int32>(mesh->_animClips.size());
		for (int32 i = 0; i < animCount; i++)
		{
			AnimClipInfo& animClip = mesh->_animClips[i];

			// �ִϸ��̼� ������ ����
			std::vector<AnimFrameParams> frameParams;
			frameParams.resize(mesh->_bones.size() * animClip.frameCount);

			for (int32 b = 0; b < boneCount; b++)
			{
				const int32 keyFrameCount = static_cast<int32>(animClip.keyFrames[b].size());
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

			// StructuredBuffer ����
			mesh->_frameBuffer.push_back(std::make_shared<StructuredBuffer>());
			mesh->_frameBuffer.back()->Init(sizeof(AnimFrameParams), static_cast<uint32>(frameParams.size()), frameParams.data());
		}
	}
	
	return true;
}

void JsonManager::LoadGameObject(RTTRGameObjectValue value, std::shared_ptr<GameObject> object)
{
	std::shared_ptr<MeshData> meshData = nullptr;

	object->SetName(s2ws(value.tag).c_str());

	object->_hash = value.hashValue;

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::TRANSFORM)] == true)
	{
		object->AddComponent(std::make_shared<Transform>());
		if (value.transformValue.parentHashValue != -1)
		{
			Pair pair;
			pair.first = value.hashValue;
			pair.second = value.transformValue.parentHashValue;
			hashValues.push_back(pair);
		}
	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::MESH_RENDERER)] == true)
	{
		if (value.meshRendererValue.meshValue.tag.find(".fbx") != std::string::npos) {
			// fbx ���� ����
			// MeshData �ҷ�����.
			std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
			object->AddComponent(mr); 

			meshData = GET_SINGLE(Resources)->LoadFBX(s2ws(value.meshRendererValue.meshValue.tag), true);

			mr->SetMesh(meshData->_meshRenders[0].mesh);
			for (uint32 i = 0; i < meshData->_meshRenders[0].materials.size(); i++)
				mr->SetMaterial(meshData->_meshRenders[0].materials[i], i);

		}
		else
		{
			std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
			object->AddComponent(mr);
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
				object->GetMeshRenderer()->SetMaterial(material, i);
			}

			std::wstring meshTag = s2ws(value.meshRendererValue.meshValue.tag);
			std::shared_ptr<Mesh> mesh;

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

			mr->SetMesh(mesh);
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
		object->AddComponent(std::make_shared<ParticleSystem>());

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
		default:
			break;
		}
	}

	if (value.componentOnValue[static_cast<uint8>(COMPONENT_TYPE::ANIMATOR)] == true)
	{
		std::shared_ptr<Animator> animator = std::make_shared<Animator>();
		object->AddComponent(animator);
		animator->SetBones(meshData->_meshRenders[0].mesh->GetBones());
		animator->SetAnimClip(meshData->_meshRenders[0].mesh->GetAnimClip());
	}
	
	LoadMonobehaviour(value, object);
}

void JsonManager::LoadMonobehaviour(RTTRGameObjectValue value, std::shared_ptr<GameObject> object)
{
	if (value.monobehaviourOnValue[static_cast<int>(MonoBehaviourType::GameManagerScript)] == true)
	{
		object->AddComponent(std::make_shared<GameManagerScript>());
	}

	if (value.monobehaviourOnValue[static_cast<int>(MonoBehaviourType::TerrainScript)] == true)
	{
		object->AddComponent(std::make_shared<TerrainScript>());
	}

	if (value.monobehaviourOnValue[static_cast<int>(MonoBehaviourType::TestCameraScript)] == true)
	{
		object->AddComponent(std::make_shared<TestCameraScript>());
	}

	if (value.monobehaviourOnValue[static_cast<int>(MonoBehaviourType::TestDragon)] == true)
	{
		object->AddComponent(std::make_shared<TestDragon>());
	}
}