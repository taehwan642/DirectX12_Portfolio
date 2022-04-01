#pragma once
#include "Component.h"

class Mesh;
class Material;
class GameObject;

enum class ColliderType
{
	Sphere,
	Cube,
	Mesh
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance) { return false; };

	std::shared_ptr<GameObject> GetColliderMesh() { return _colliderMeshGameObject; }
	bool IsDrawMesh() { return _draw; }

protected:
	friend class RTTRColliderValue;
	
	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;
	std::shared_ptr<GameObject> _colliderMeshGameObject;

	bool _draw = false;

private:
	ColliderType _colliderType = {};

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};