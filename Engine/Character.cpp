#include "pch.h"
#include "Character.h"
#include "Timer.h"

void Character::UpdateInvincibleTime()
{
	_invincibleDeltaTime -= DELTA_TIME;
}
