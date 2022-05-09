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

	void SetExtent(Vec3 extent) { _extents = extent; }
	void SetCenter(Vec3 center) { _center = center; }

private:
	friend class ImGuiManager;
	friend class CollisionManager;

	BoundingBox _boundingBox;

	Vec3 _center = Vec3::Zero;
	Vec3 _extents = Vec3::One;

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};

