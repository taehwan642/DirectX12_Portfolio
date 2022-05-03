#include "pch.h"
#include "PlayerState.h"
#include "Input.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Player.h"
#include "NierAnimator.h"
#include "AudioSource.h"
#include "Engine.h"
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
	
	if (INPUT->GetButton(KEY_TYPE::RIGHT) || INPUT->GetButton(KEY_TYPE::LEFT))
	{
		for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(6);
		}
	}
	else
	{
		for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(5);
		}
	}
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		return FLIGHT_TO_COMBAT;//_object.lock()->GetComponent<Player>()->ChangeFlightCombatMode();
	}
	if (INPUT->GetButton(KEY_TYPE::E))
	{
		_object.lock()->GetComponent<Player>()->Attack();
		return FLIGHT_FIRE;
	}
	return FLIGHT_IDLE;
}

int CombatIdleState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(7);
	}
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		return COMBAT_TO_FLIGHT;//_object.lock()->GetComponent<Player>()->ChangeFlightCombatMode();
	}
	if (INPUT->GetButton(KEY_TYPE::E))
	{
		_object.lock()->GetComponent<Player>()->Attack();
		return COMBAT_FIRE;
	}
	return COMBAT_IDLE;
}

int FlightFireState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	
	return FLIGHT_IDLE;
}

int CombatFireState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	return COMBAT_IDLE;
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
	std::wstring findName = (_object.lock()->GetComponent<Player>()->Get9SMode() == true ? L"2B" : L"9S");
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<TransformComponent> childTransform = _object.lock()->GetTransform()->GetChild(0)->GetChild(i);
		if (size_t pos = childTransform->GetGameObject()->GetName().find(findName); pos != std::wstring::npos)
			continue;
	
		std::shared_ptr<GameObject> object = childTransform->GetGameObject();
		std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(1);

		if (std::static_pointer_cast<NierAnimator>(object->GetAnimator())->GetAnimationEnd())
		{
			ADDLOG("FLIGHT TO COMBAT END\n");
			return COMBAT_IDLE;
		}
	}
	return FLIGHT_TO_COMBAT;
}

int CombatToFlightState::handleInput()
{
	std::wstring findName = (_object.lock()->GetComponent<Player>()->Get9SMode() == true ? L"2B" : L"9S");
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<TransformComponent> childTransform = _object.lock()->GetTransform()->GetChild(0)->GetChild(i);
		if (size_t pos = childTransform->GetGameObject()->GetName().find(findName); pos != std::wstring::npos)
			continue;
		
		std::shared_ptr<GameObject> object = childTransform->GetGameObject();
		std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(0);

		if (std::static_pointer_cast<NierAnimator>(object->GetAnimator())->GetAnimationEnd())
		{
			ADDLOG("COMBAT TO FLIGHT END\n");
			return FLIGHT_IDLE;
		}
	}
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
