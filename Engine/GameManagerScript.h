#pragma once
#include "MonoBehaviour.h"
class GameManagerScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(GameManagerScript)

	virtual ~GameManagerScript();

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

