#pragma once
#include "MonoBehaviour.h"

class Sea :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(Sea)

	virtual ~Sea();

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

