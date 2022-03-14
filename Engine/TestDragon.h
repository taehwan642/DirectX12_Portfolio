#pragma once
#include "MonoBehaviour.h"

class TestDragon : public MonoBehaviour
{
public:
	MONOBEHAVIOUR(TestDragon)

	virtual void Update() override;
	
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

