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

	virtual void Spawn(const Vec3& worldPosition) override;

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override;
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

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

