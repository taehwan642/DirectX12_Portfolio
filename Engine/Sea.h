#pragma once
#include "MonoBehaviour.h"

class Sea :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(Sea)

	virtual ~Sea();

	virtual void OnCollisionEnter(const CollisionOutput& collider) override {}
	virtual void OnCollisionStay(const CollisionOutput& collider) override {}
	virtual void OnCollisionExit(const CollisionOutput& collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

