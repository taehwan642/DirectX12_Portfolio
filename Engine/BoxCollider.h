#pragma once
#include "BaseCollider.h"
class BoxCollider :
    public BaseCollider
{
public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance) override;

private:
	friend class ImGuiManager;
	friend class CollisionManager;

	BoundingBox _boundingBox;

	Vec3 _center;
	Vec3 _extents;

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};

