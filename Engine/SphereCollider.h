#pragma once
#include "BaseCollider.h"

class ImGuiManager;

class SphereCollider : public BaseCollider
{
public:
	SphereCollider();
	virtual ~SphereCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance) override;

	void SetRadius(float radius) { _radius = radius; }
	void SetCenter(Vec3 center) { _center = center; }

private:
	friend class ImGuiManager;
	
	// Local ±‚¡ÿ
	float		_radius = 1.f;
	Vec3		_center = Vec3(0, 0, 0);

	BoundingSphere _boundingSphere;

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};

