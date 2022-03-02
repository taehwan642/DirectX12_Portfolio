#pragma once
#include "BaseCollider.h"

class Mesh;
class Material;
class GameObject;

class SphereCollider : public BaseCollider
{
public:
	static int num;
	SphereCollider();
	virtual ~SphereCollider();

	void Render();

	virtual void FinalUpdate() override;
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;

	void SetRadius(float radius) { _radius = radius; }
	void SetCenter(Vec3 center) { _center = center; }

private:
	// Local ±‚¡ÿ
	float		_radius = 1.f;
	Vec3		_center = Vec3(0, 0, 0);

	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;
	std::shared_ptr<GameObject> _gameObject;

	BoundingSphere _boundingSphere;
};

