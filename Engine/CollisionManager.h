#pragma once

enum class CollisionObjectType
{
	PLAYER,
	PLAYER_WEAPON,
	ENEMY,
	END
};

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
	bool CheckCollisionBox(const BoundingBox& srcCollider, const BoundingBox& dstCollider);
	bool CheckCollisionSphereBox(const BoundingSphere& srcCollider, const BoundingBox& dstCollider);

private:
	std::list<std::shared_ptr<GameObject>> _listObject[static_cast<int>(CollisionObjectType::END)];

};