#pragma once
#include "Component.h"
class AudioSource :
    public Component
{
public:
	AudioSource();
	virtual ~AudioSource();

	virtual void FinalUpdate() override;

	RTTR_REGISTRATION_FRIEND
};

