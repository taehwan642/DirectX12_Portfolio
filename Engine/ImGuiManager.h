#pragma once
#ifdef TOOL
class Device;
class GraphicsCommandQueue;
class GameObject;
class Material;
class Camera;
class Mesh;
class SphereCollider;
class Texture;
class CubeCollider;
class MeshCollider;
class Scene;

class ImGuiManager
{
public:
	std::shared_ptr<GameObject> _currentGameObject;
	int _drawCall = 0;
	
public:
	ImGuiManager(HWND hwnd, std::shared_ptr<Device> device);
	~ImGuiManager();

	void Render();
	void SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdq);

	void DragAndDrop(const std::wstring& path);

private:
	void RenderMeshData(std::shared_ptr<Mesh> mesh);
	void RenderMaterialData(int materialIndex, std::shared_ptr<Material> material);
	void RenderCameraData(std::shared_ptr<Camera> camera);
	void RenderSphereColliderData(std::shared_ptr<SphereCollider> sphereCollider);
	void RenderCubeColliderData(std::shared_ptr<CubeCollider> cubeCollider);
	void RenderMeshColliderData(std::shared_ptr<MeshCollider> meshCollider);

private:
	void RenderClientData();
	void RenderHierarchy();
	void RenderInspector();
	void RenderResources();
	void RenderDragAndDrop();

	void RenderChild(std::shared_ptr<GameObject> parent, int i);

	void AddSceneChild(std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent, bool eraseLastChar);

private:

	std::string inputPath = "";
	ComPtr<ID3D12DescriptorHeap> _srvDescHeap;
};

#endif