#pragma once
#include "MonoBehaviour.h"
class GameManagerScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(GameManagerScript)

	virtual ~GameManagerScript();

	virtual void OnCollisionEnter(const CollisionOutput& collider) override {}
	virtual void OnCollisionStay(const CollisionOutput& collider) override {}
	virtual void OnCollisionExit(const CollisionOutput& collider) override {}

	virtual void Awake() override;
	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

