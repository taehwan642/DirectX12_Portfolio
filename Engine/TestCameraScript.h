#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	MONOBEHAVIOUR(TestCameraScript)

	virtual ~TestCameraScript();

	virtual void OnCollisionEnter(CollisionOutput collider) override {}
	virtual void OnCollisionStay(CollisionOutput collider) override {}
	virtual void OnCollisionExit(CollisionOutput collider) override {}

	virtual void LateUpdate() override;

private:
	float		_speed = 100.f;
	Vec2 pressedPos{};

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

