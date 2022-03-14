#pragma once
#include "MonoBehaviour.h"
class GameManagerScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(GameManagerScript)

	virtual ~GameManagerScript();

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

