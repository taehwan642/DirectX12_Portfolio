#pragma once
#include "MonoBehaviour.h"

class TestDragon : public MonoBehaviour
{
public:
	MONOBEHAVIOUR(TestDragon)

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void Update() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

