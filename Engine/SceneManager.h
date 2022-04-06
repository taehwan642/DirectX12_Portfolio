#pragma once

class Scene;

enum
{
	MAX_LAYER = 32
};

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Init();

	void Update();
	void Render();
	void LoadScene(const std::wstring& sceneName);
	void SetScene(int index);

	void SetLayerName(uint8 index, const std::wstring& name);
	const std::wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const std::wstring& name);

public:
	std::shared_ptr<Scene> GetActiveScene() { return _activeScene; }

	std::vector<std::wstring>& GetLoadedMeshDataTagVector() { return _loadedMeshDataTags; }
	std::vector<std::wstring>& GetLoadedTextureTagVector() { return _loadedTextureTags; }

	const FogData& GetFogData() { return _fogData; }
	void SetFogData(const FogData& data) { _fogData = data; }
	Matrix GetFogMatrix() 
	{
		Matrix fogMatrix;
		FogData fogData = GET_SINGLE(SceneManager)->GetFogData();
		fogMatrix._11 = fogData.fogStart;
		fogMatrix._12 = fogData.fogRange;
		fogMatrix._21 = fogData.fogColor.x;
		fogMatrix._22 = fogData.fogColor.y;
		fogMatrix._23 = fogData.fogColor.z;
		fogMatrix._24 = fogData.fogColor.w;
		return fogMatrix;
	}

private:
	std::shared_ptr<Scene> LoadTestScene();

private:
	friend class ImGuiManager;
	friend class RTTRSceneValue;

	std::shared_ptr<Scene> _activeScene;

	std::array<std::wstring, MAX_LAYER> _layerNames;
	std::map<std::wstring, uint8> _layerIndex;

	std::vector<std::wstring> _loadedTextureTags;
	std::vector<std::wstring> _loadedMeshDataTags;
	std::vector<std::wstring> _sceneTags;

	FogData _fogData;
};

