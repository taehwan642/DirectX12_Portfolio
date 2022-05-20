#pragma once
#include "MonoBehaviour.h"
#include "Character.h"
class PlayerBullet :
    public MonoBehaviour, public Bullet
{
public:
	PlayerBullet();

	virtual ~PlayerBullet();

	virtual void Spawn(int hp, float speed, int damage) override;

	virtual void OnCollisionEnter(CollisionOutput collider) override;
	virtual void OnCollisionStay(CollisionOutput collider) override {}
	virtual void OnCollisionExit(CollisionOutput collider) override {}

	virtual void Awake() override;
	virtual void LateUpdate() override;

	virtual void Move() override;
	virtual void Attack() override {};

private:

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

