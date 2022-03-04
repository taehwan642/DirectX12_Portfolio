#pragma once
#include "Component.h"

class ImGuiManager;
class Mesh;
class Material;
class GameObject;

enum class ColliderType
{
	Sphere,
	END
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) = 0;

	std::shared_ptr<GameObject> GetColliderMesh() { return _colliderMeshGameObject; }
	bool IsDrawMesh() { return _draw; }

protected:
	friend class ImGuiManager;
	
	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;
	std::shared_ptr<GameObject> _colliderMeshGameObject;

	bool _draw = false;

private:
	ColliderType _colliderType = {};
};