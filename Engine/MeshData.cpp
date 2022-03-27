#include "pch.h"
#include "MeshData.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "TransformComponent.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "JsonManager.h"

MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

std::shared_ptr<MeshData> MeshData::LoadFromFBX(const std::wstring& path, bool jsonLoad)
{
	FBXLoader loader;
	loader.LoadFbx(path, jsonLoad);

	std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		std::shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader);
		
		mesh->_name = path;// +std::to_wstring(i);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		std::vector<std::shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			std::shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

std::shared_ptr<MeshData> MeshData::LoadFromFile(const std::wstring& path, bool jsonLoad)
{
	std::shared_ptr<MeshData> result = std::make_shared<MeshData>();

	if (jsonLoad == true)
	{
		// MeshData의 Animation을 Json으로 로드 시도, 없다면
		result = LoadFromFBX(path, true);
		if (GET_SINGLE(JsonManager)->LoadMeshData(ws2s(path).c_str(), result) == false)
		{
			result = LoadFromFBX(path, false);
			GET_SINGLE(JsonManager)->SaveMeshData(ws2s(path).c_str(), result);
		}
	}
	else
	{
		result = LoadFromFBX(path, jsonLoad);
		GET_SINGLE(JsonManager)->SaveMeshData(ws2s(path).c_str(), result);
	}

	return result;
}

void MeshData::Load(const std::wstring& _strFilePath)
{
	// TODO
}

void MeshData::Save(const std::wstring& _strFilePath)
{
	// TODO
}

std::vector<std::shared_ptr<GameObject>> MeshData::Instantiate()
{
	std::vector<std::shared_ptr<GameObject>> v;

	for (MeshRenderInfo& info : _meshRenders)
	{
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
		gameObject->AddComponent(std::make_shared<TransformComponent>());
		gameObject->AddComponent(std::make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(info.mesh);

		for (uint32 i = 0; i < info.materials.size(); i++)
			gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);

		if (info.mesh->IsAnimMesh())
		{
			std::shared_ptr<Animator> animator = std::make_shared<Animator>();
			gameObject->AddComponent(animator);
			animator->SetBones(info.mesh->GetBones());
			animator->SetAnimClip(info.mesh->GetAnimClip());
		}

		v.push_back(gameObject);
	}


	return v;
}

