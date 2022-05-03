#pragma once
#include "MonoBehaviour.h"
#include "Character.h"
class PlayerBullet :
    public MonoBehaviour, public Character
{
public:
	PlayerBullet();

	virtual ~PlayerBullet();

	void Spawn(const Vec3& worldPosition);

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override;
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void Awake() override;
	virtual void LateUpdate() override;

	virtual void Move() override;
	virtual void Attack() override {};

	float _aliveTime = 0.f;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

