#pragma once
#include "MonoBehaviour.h"

class Sea :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(Sea)

	virtual ~Sea();

	virtual void OnCollisionEnter(CollisionOutput collider) override {}
	virtual void OnCollisionStay(CollisionOutput collider) override {}
	virtual void OnCollisionExit(CollisionOutput collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

