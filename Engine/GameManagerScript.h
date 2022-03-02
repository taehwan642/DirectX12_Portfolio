#pragma once
#include "MonoBehaviour.h"
class GameManagerScript :
    public MonoBehaviour
{
public:
	GameManagerScript();
	virtual ~GameManagerScript();

	virtual void LateUpdate() override;
};

