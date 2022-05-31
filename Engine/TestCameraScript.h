#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	MONOBEHAVIOUR(TestCameraScript)

	virtual ~TestCameraScript();

	virtual void OnCollisionEnter(const CollisionOutput& collider) override {}
	virtual void OnCollisionStay(const CollisionOutput& collider) override {}
	virtual void OnCollisionExit(const CollisionOutput& collider) override {}

	virtual void LateUpdate() override;

private:
	float		_speed = 100.f;
	Vec2 pressedPos{};

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

