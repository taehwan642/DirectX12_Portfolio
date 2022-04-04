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
	Mesh
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance) { return false; };

	std::shared_ptr<Visualizer> GetColliderVisualizer() { return _colliderVisualizer; }
	bool IsDrawMesh() { return _draw; }

protected:
	friend class ImGuiManager;
	friend class RTTRColliderValue;
	
	std::shared_ptr<Visualizer> _colliderVisualizer;

	bool _draw = false;

private:
	ColliderType _colliderType = {};

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};