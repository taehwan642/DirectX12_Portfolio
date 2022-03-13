#pragma once
class GameObject;
class Scene;
class RTTRGameObjectValue;
class MeshData;
class Mesh;

class JsonManager
{
	DECLARE_SINGLE(JsonManager);

public:
	void Save(const std::string& path, std::shared_ptr<GameObject> object);
	void SaveScene(const std::string& path, std::shared_ptr<Scene> scene);
	void SaveMeshData(const std::string& path, std::shared_ptr<MeshData> data);

	bool Load(const std::string& path, std::shared_ptr<GameObject> object);
	bool LoadScene(const std::string& path, std::shared_ptr<Scene> scene);
	bool LoadMeshData(const std::string& path, std::shared_ptr<MeshData> data);

private:
	void LoadGameObject(RTTRGameObjectValue value, std::shared_ptr<GameObject> object);
};

