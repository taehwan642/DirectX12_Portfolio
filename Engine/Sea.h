#pragma once
#include "MonoBehaviour.h"

class Sea :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(Sea)

	virtual ~Sea();

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

