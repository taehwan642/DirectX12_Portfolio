#pragma once

class GameObject;
class CollisionManager
{
	DECLARE_SINGLE(CollisionManager);

public:
	void AddObject(CollisionObjectType type, std::shared_ptr<GameObject> gameObject);
	void Update();
	void CheckCollision(CollisionObjectType srcType, CollisionObjectType dstType);
	void DeleteAllObject();
	void DeleteObject(CollisionObjectType eType);

private:
	bool CheckCollisionSphere(const BoundingSphere& srcCollider, const BoundingSphere& dstCollider);
	bool CheckCollisionBox(const BoundingOrientedBox& srcCollider, const BoundingOrientedBox& dstCollider);
	bool CheckCollisionSphereBox(const BoundingSphere& srcCollider, const BoundingOrientedBox& dstCollider);

private:
	std::array< std::list< std::shared_ptr<GameObject> >, static_cast<int>(CollisionObjectType::END) > _listObject;

};