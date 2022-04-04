#pragma once
#include "Component.h"

class Mesh;
class Material;

// [32][32]
union InstanceID
{
	struct
	{
		uint32 meshID;
		uint32 materialID;
	};
	uint64 id;
};

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	std::shared_ptr<Mesh> GetMesh() { return _mesh; }
	std::shared_ptr<Material> GetMaterial(uint32 idx = 0) { return _materials[idx]; }

	void SetMesh(std::shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(std::shared_ptr<Material> material, uint32 idx = 0);

	void Render();
	void Render(std::shared_ptr<class InstancingBuffer>& buffer);
	void RenderShadow();

	uint64 GetInstanceID();

	std::shared_ptr<MeshRenderer> Clone()
	{
		std::shared_ptr<MeshRenderer> result = std::make_shared<MeshRenderer>();
		result->_mesh = _mesh;
		result->_materials = _materials;
		return result;
	}

private:
	friend class ImGuiManager;
	friend class RTTRGameObjectValue;
	friend class RTTRMeshRendererValue;

	std::shared_ptr<Mesh> _mesh;
	std::vector<std::shared_ptr<Material>> _materials;

	RTTR_REGISTRATION_FRIEND
};

