#pragma once
#include "Component.h"

enum class ColliderType
{
	Sphere,
	END
};

class BaseCollider : public Component
{
public:
	static bool _drawCollider[static_cast<int>(ColliderType::END)];

	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) = 0;

private:
	ColliderType _colliderType = {};
};