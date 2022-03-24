#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	MONOBEHAVIOUR(TestCameraScript)

	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 300.f;
	Vec2 pressedPos{};

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

