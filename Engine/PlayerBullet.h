#pragma once
#include "MonoBehaviour.h"
#include "Character.h"
class PlayerBullet :
    public MonoBehaviour, public Bullet
{
public:
	PlayerBullet();

	virtual ~PlayerBullet();

	virtual void Spawn(const Vec3& worldPosition) override;

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override;
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void Awake() override;
	virtual void LateUpdate() override;

	virtual void Move() override;
	virtual void Attack() override {};

private:

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

