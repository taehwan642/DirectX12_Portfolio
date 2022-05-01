#include "pch.h"
#include "PlayerState.h"
#include "Input.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Player.h"
#include "NierAnimator.h"

/*
Player Animation Index
0 = COMBAT -> FLIGHT
1 = FLIGHT -> COMBAT
2 = COMBAT ATTACK 1
3 = COMBAT ATTACK 2
4 = FLIGHT DODGE
5 = FLIGHT IDLE
6 = FLIGHT ROTATE IDLE
7 = COMBAT IDLE
8 = COMBAT FIRE
9 = FLIGHT DEAD
*/

int FlightIdleState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(5);
	}
	if (INPUT->GetButton(KEY_TYPE::RIGHT) || INPUT->GetButton(KEY_TYPE::LEFT))
	{
		for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(6);
		}
	}
	if (INPUT->GetButton(KEY_TYPE::SPACE))
	{
		return FLIGHT_DEAD;
	}
	return FLIGHT_IDLE;
}

int CombatIdleState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	return COMBAT_IDLE;
}

int FlightFireState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	return FLIGHT_FIRE;
}

int CombatFireState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	return COMBAT_FIRE;
}

int CombatAttack1State::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	return COMBAT_ATTACK1;
}

int CombatAttack2State::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	return COMBAT_ATTACK2;
}

int FlightToCombatState::handleInput()
{
	return FLIGHT_TO_COMBAT;
}

int CombatToFlightState::handleInput()
{
	return COMBAT_TO_FLIGHT;
}

int FlightDodgeState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	return FLIGHT_DODGE;
}

int FlightDeadState::handleInput()
{
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(9);

		if (std::static_pointer_cast<NierAnimator>(object->GetAnimator())->GetAnimationEnd())
		{
			_object.lock()->SetActive(false);
		}
	}


	return FLIGHT_DEAD;
}
