#pragma once
#include "Object.h"

class Mesh;
class Material;
class GameObject;

struct MeshRenderInfo
{
	std::shared_ptr<Mesh>				mesh;
	std::vector<std::shared_ptr<Material>>	materials;
};

class MeshData : public Object
{
public:
	MeshData();
	virtual ~MeshData();

public:
	static std::shared_ptr<MeshData> LoadFromFBX(const std::wstring& path, bool jsonLoad = true);
	static std::shared_ptr<MeshData> LoadFromFile(const std::wstring& path, bool jsonLoad = true);

	virtual void Load(const std::wstring& path);
	virtual void Save(const std::wstring& path);

	std::vector<std::shared_ptr<GameObject>> Instantiate();

private:
	friend class Resources;
	friend class RTTRMeshDataValue;
	friend class JsonManager;

	std::vector<MeshRenderInfo> _meshRenders;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};
