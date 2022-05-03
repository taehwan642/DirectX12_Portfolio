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
		_object.lock()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_21.wav");
		_object.lock()->GetAudioSource()->SetLoop(false);
		_object.lock()->GetAudioSource()->SetVolume(0.2f);
		_object.lock()->GetAudioSource()->Play();
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
	
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_object.lock()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_21.wav");
		_object.lock()->GetAudioSource()->SetLoop(false);
		_object.lock()->GetAudioSource()->SetVolume(0.2f);
		_object.lock()->GetAudioSource()->Play();
		return COMBAT_TO_FLIGHT;//_object.lock()->GetComponent<Player>()->ChangeFlightCombatMode();
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		_object.lock()->GetComponent<Player>()->Attack();
		for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
			anim->SetAnimationIndex(8);
		}
		return COMBAT_FIRE;
	}
	else
	{
		for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(7);
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::C))
	{
		_object.lock()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_8.wav");
		_object.lock()->GetAudioSource()->SetLoop(false);
		_object.lock()->GetAudioSource()->SetVolume(0.2f);
		_object.lock()->GetAudioSource()->Play();
		return COMBAT_ATTACK1;
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
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(2);

		if (anim->GetCurrentFrame() > 340)
		{
			if (INPUT->GetButtonDown(KEY_TYPE::C))
			{
				_object.lock()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_8.wav");
				_object.lock()->GetAudioSource()->SetLoop(false);
				_object.lock()->GetAudioSource()->SetVolume(0.2f);
				_object.lock()->GetAudioSource()->Play();
				return COMBAT_ATTACK2;
			}

			if (INPUT->GetButton(KEY_TYPE::E))
			{
				return COMBAT_IDLE;
			}
		}

		if (anim->GetAnimationEnd())
		{
			return COMBAT_IDLE;
		}
	}

	return COMBAT_ATTACK1;
}

int CombatAttack2State::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(3);

		if (anim->GetCurrentFrame() > 440)
		{
			if (INPUT->GetButtonDown(KEY_TYPE::C))
			{
				_object.lock()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_8.wav");
				_object.lock()->GetAudioSource()->SetLoop(false);
				_object.lock()->GetAudioSource()->SetVolume(0.2f);
				_object.lock()->GetAudioSource()->Play();
				return COMBAT_ATTACK1;
			}

			if (INPUT->GetButton(KEY_TYPE::E))
			{
				return COMBAT_IDLE;
			}
		}

		if (anim->GetAnimationEnd())
		{
			return COMBAT_IDLE;
		}
	}
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
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(1);

		if (anim->GetAnimationEnd())
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
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(0);

		if (anim->GetAnimationEnd())
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
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(4);

		if (anim->GetAnimationEnd())
		{
			return FLIGHT_IDLE;
		}
	}
	return FLIGHT_DODGE;
}

int FlightDeadState::handleInput()
{
	for (int i = 0; i < _object.lock()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = _object.lock()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(9);

		if (anim->GetAnimationEnd())
		{
			_object.lock()->SetActive(false);
		}
	}

	return FLIGHT_DEAD;
}
