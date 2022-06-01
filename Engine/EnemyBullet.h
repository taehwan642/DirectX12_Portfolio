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

	virtual void OnCollisionEnter(const CollisionOutput& collider) override;
	virtual void OnCollisionStay(const CollisionOutput& collider) override {}
	virtual void OnCollisionExit(const CollisionOutput& collider) override {}

	virtual void LateUpdate() override;

	virtual void Move() override;
	virtual void Attack() override {};

	void SetEnemyBulletState(EnemyBulletState state) { _state = state; }

#ifdef TOOL
	virtual void DragAndDrop(size_t hash) override;
#endif

private:
	EnemyBulletState _state = EnemyBulletState::DESTROYABLE;
	size_t _testObject;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

