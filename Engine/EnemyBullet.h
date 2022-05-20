#pragma once
#include "MonoBehaviour.h"
#include "Character.h"

enum class EnemyBulletState
{
	DESTROYABLE,
	NON_DESTROYABLE
};

class EnemyBullet :
    public MonoBehaviour, public Bullet
{
public:
	EnemyBullet();

	virtual ~EnemyBullet();

	virtual void Spawn(int hp, float speed, int damage) override;

	virtual void OnCollisionEnter(CollisionOutput collider) override;
	virtual void OnCollisionStay(CollisionOutput collider) override {}
	virtual void OnCollisionExit(CollisionOutput collider) override {}

	virtual void LateUpdate() override;

	virtual void Move() override;
	virtual void Attack() override {};

#ifdef TOOL
	virtual void DragAndDrop(size_t hash) override;
#endif

private:
	EnemyBulletState _state = EnemyBulletState::DESTROYABLE;
	size_t _testObject;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

