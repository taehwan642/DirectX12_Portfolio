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
	bool CheckCollisionSphere(const Vec3& srcPos, float srcRadius, const Vec3& dstPos, float dstRadius);
	bool CheckCollisionBox(const Vec3& srcPos, float srcRadius, const Vec3& dstPos, float dstRadius);
	bool CheckCollisionSphereBox(const Vec3& srcPos, float srcRadius, const Vec3& dstPos, float dstRadius);
	bool CheckCollisionSpherePoint(const Vec3& srcPos, float srcRadius, const Vec3& dstPos, float dstRadius);

private:
	std::list<std::shared_ptr<GameObject>> _listObject[static_cast<int>(CollisionObjectType::END)];

};