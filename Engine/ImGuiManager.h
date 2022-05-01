#pragma once
#ifdef TOOL
#include "DebugLogger.h"

class Device;
class GraphicsCommandQueue;
class GameObject;
class Material;
class Camera;
class Mesh;
class SphereCollider;
class Texture;
class BoxCollider;
class MeshCollider;
class Scene;
class BoneCollider;

class ImGuiManager
{
public:
	std::shared_ptr<GameObject> _currentGameObject;
	int _drawCall = 0;
	
public:
	ImGuiManager(HWND hwnd, std::shared_ptr<Device> device);
	~ImGuiManager();

	void Update();
	void Render();
	void SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdq);

	void DragAndDrop(const std::wstring& path);

	std::shared_ptr<DebugLogger> GetDebugLogger() { return _debugLogger; }

private:
	void RenderMeshData(std::shared_ptr<Mesh> mesh);
	void RenderMaterialData(int materialIndex, std::shared_ptr<Material> material);
	void RenderCameraData(std::shared_ptr<Camera> camera);
	void RenderSphereColliderData(std::shared_ptr<SphereCollider> sphereCollider);
	void RenderBoxColliderData(std::shared_ptr<BoxCollider> boxCollider);
	void RenderMeshColliderData(std::shared_ptr<MeshCollider> meshCollider);
	void RenderBoneColliderData(std::shared_ptr<BoneCollider> boneCollider);
	void RenderMatrixData(const Matrix& matrix);

private:
	void RenderClientData();
	void RenderHierarchy();
	void RenderInspector();
	void RenderResources();
	void RenderDragAndDrop();
	void RenderCollisionManager();
	void RenderDebugLog();

	void RenderChild(std::shared_ptr<GameObject> parent, int i);

	void AddSceneChild(std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent, bool eraseLastChar);
	void RemoveSceneChild(std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent);

private:

	std::string _inputPath = "";
	ComPtr<ID3D12DescriptorHeap> _srvDescHeap;
	std::queue<std::function<void()>> _functionQueue;
	std::shared_ptr<DebugLogger> _debugLogger;
};

#endif