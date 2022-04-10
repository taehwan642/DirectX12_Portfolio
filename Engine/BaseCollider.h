#pragma once
#include "Component.h"

class Mesh;
class Material;
class GameObject;
class Visualizer;

enum class ColliderType
{
	Sphere,
	Cube,
	Mesh,
	Bone
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance) { return false; };

	std::vector<std::shared_ptr<Visualizer>>& GetColliderVisualizers() { return _colliderVisualizers; }
	bool IsDrawMesh() { return _draw; }

	ColliderType GetColliderType() { return _colliderType; }

protected:
	friend class ImGuiManager;
	friend class RTTRColliderValue;
	
	std::vector<std::shared_ptr<Visualizer>> _colliderVisualizers;

	bool _draw = false;

private:
	ColliderType _colliderType = {};

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};