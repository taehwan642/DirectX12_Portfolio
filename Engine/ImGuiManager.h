#pragma once

class Device;
class GraphicsCommandQueue;
class GameObject;
class Material;
class Camera;
class Mesh;
class SphereCollider;

struct float3
{
	float x;
	float y;
	float z;
};

class ImGuiManager
{
public:
	std::vector<float3*> _positions;
	std::vector<float*> _floats;
	std::shared_ptr<GameObject> _currentGameObject;
	int _drawCall = 0;
	
public:
	ImGuiManager(HWND hwnd, std::shared_ptr<Device> device);
	~ImGuiManager();

	void Render();
	void SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdq);

private:
	ComPtr<ID3D12DescriptorHeap> _srvDescHeap;

private:
	void RenderMeshData(std::shared_ptr<Mesh> mesh);
	void RenderMaterialData(int materialIndex, std::shared_ptr<Material> material);
	void RenderCameraData(std::shared_ptr<Camera> camera);
	void RenderSphereColliderData(std::shared_ptr<SphereCollider> sphereCollider);

private:
	void RenderClientData();
	void RenderHierarchy();
	void RenderInspector();
};

