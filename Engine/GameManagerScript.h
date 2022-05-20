#pragma once
#include "MonoBehaviour.h"
class GameManagerScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(GameManagerScript)

	virtual ~GameManagerScript();

	virtual void OnCollisionEnter(CollisionOutput collider) override {}
	virtual void OnCollisionStay(CollisionOutput collider) override {}
	virtual void OnCollisionExit(CollisionOutput collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

