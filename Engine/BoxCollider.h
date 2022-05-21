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

	void SetExtent(const Vec3& extent) { _extents = extent; }
	void SetCenter(const Vec3& center) { _center = center; }
	void SetOrientation(const Vec3& orientation) 
	{ 
		_orientation = orientation;
	}

private:
	friend class ImGuiManager;
	friend class CollisionManager;

	BoundingOrientedBox _boundingBox;

	Vec3 _center = Vec3::Zero;
	Vec3 _extents = Vec3::One;
	Vec3 _orientation = Vec3::Zero;

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};

