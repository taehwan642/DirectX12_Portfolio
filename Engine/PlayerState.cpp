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
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	if (player->IsDead())
		return FLIGHT_DEAD;

	obj->GetTransform()->SetWorldRotation(Vec3::Zero);
	player->Move();

	if (INPUT->GetButton(KEY_TYPE::RIGHT) || INPUT->GetButton(KEY_TYPE::LEFT))
	{
		for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = obj->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(6);
		}
	}
	else
	{
		for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = obj->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(5);
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		obj->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_21.wav");
		obj->GetAudioSource()->SetLoop(false);
		obj->GetAudioSource()->SetVolume(0.2f);
		obj->GetAudioSource()->Play();
		return FLIGHT_TO_COMBAT;//_object.lock()->GetComponent<Player>()->ChangeFlightCombatMode();
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		player->Attack();
		return FLIGHT_FIRE;
	}

	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		obj->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_8.wav");
		obj->GetAudioSource()->SetLoop(false);
		obj->GetAudioSource()->SetVolume(0.2f);
		obj->GetAudioSource()->Play();
		return FLIGHT_DODGE;
	}

	return FLIGHT_IDLE;
}

int CombatIdleState::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	if (player->IsDead())
		return FLIGHT_DEAD;

	player->Move();

	if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		obj->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_21.wav");
		obj->GetAudioSource()->SetLoop(false);
		obj->GetAudioSource()->SetVolume(0.2f);
		obj->GetAudioSource()->Play();
		return COMBAT_TO_FLIGHT;//_object.lock()->GetComponent<Player>()->ChangeFlightCombatMode();
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		obj->GetComponent<Player>()->Attack();
		for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = obj->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
			anim->SetAnimationIndex(8);
		}
		return COMBAT_FIRE;
	}
	else
	{
		for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> object = obj->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
			std::static_pointer_cast<NierAnimator>(object->GetAnimator())->SetAnimationIndex(7);
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::C))
	{
		obj->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_8.wav");
		obj->GetAudioSource()->SetLoop(false);
		obj->GetAudioSource()->SetVolume(0.2f);
		obj->GetAudioSource()->Play();
		return COMBAT_ATTACK1;
	}
	return COMBAT_IDLE;
}

int FlightFireState::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	if (player->IsDead())
		return FLIGHT_DEAD;

	obj->GetTransform()->SetWorldRotation(Vec3::Zero);
	player->Move();
	return FLIGHT_IDLE;
}

int CombatFireState::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	if (player->IsDead())
		return FLIGHT_DEAD;

	player->Move();
	return COMBAT_IDLE;
}

int CombatAttack1State::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	if (player->IsDead())
		return FLIGHT_DEAD;

	bool isEnd = false;
	obj->GetComponent<Player>()->Move();
	for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = obj->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(2);

		if (anim->GetCurrentFrame() > 340)
		{
			if (INPUT->GetButtonDown(KEY_TYPE::C))
			{
				obj->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_8.wav");
				obj->GetAudioSource()->SetLoop(false);
				obj->GetAudioSource()->SetVolume(0.2f);
				obj->GetAudioSource()->Play();
				return COMBAT_ATTACK2;
			}

			if (INPUT->GetButton(KEY_TYPE::E))
			{
				return COMBAT_IDLE;
			}
		}

		if (anim->GetAnimationEnd())
		{
			anim->SetAnimationIndex(7);
			isEnd = true;
		}
	}
	if (isEnd == true)
		return COMBAT_IDLE;
	return COMBAT_ATTACK1;
}

int CombatAttack2State::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	if (player->IsDead())
		return FLIGHT_DEAD;

	bool isEnd = false;
	player->Move();
	for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = obj->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(3);

		if (anim->GetCurrentFrame() > 450)
		{
			if (INPUT->GetButtonDown(KEY_TYPE::C))
			{
				obj->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_8.wav");
				obj->GetAudioSource()->SetLoop(false);
				obj->GetAudioSource()->SetVolume(0.2f);
				obj->GetAudioSource()->Play();
				return COMBAT_ATTACK1;
			}

			if (INPUT->GetButton(KEY_TYPE::E))
			{
				return COMBAT_IDLE;
			}
		}

		if (anim->GetAnimationEnd())
		{
			anim->SetAnimationIndex(7);
			isEnd = true;
		}
	}

	if (isEnd == true)
		return COMBAT_IDLE;

	return COMBAT_ATTACK2;
}

int FlightToCombatState::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	if (player->IsDead())
		return FLIGHT_DEAD;

	bool isEnd = false;
	for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<TransformComponent> childTransform = obj->GetTransform()->GetChild(0)->GetChild(i);
		std::shared_ptr<GameObject> object = childTransform->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(1);

		if (anim->GetAnimationEnd())
		{
			ADDLOG("FLIGHT TO COMBAT END\n");
			anim->SetAnimationIndex(7);
			isEnd = true;
		}
	}

	if (isEnd == true)
		return COMBAT_IDLE;
	
	return FLIGHT_TO_COMBAT;
}

int CombatToFlightState::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	if (player->IsDead())
		return FLIGHT_DEAD;

	bool isEnd = false;
	for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<TransformComponent> childTransform = obj->GetTransform()->GetChild(0)->GetChild(i);
		std::shared_ptr<GameObject> object = childTransform->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(0);

		if (anim->GetAnimationEnd())
		{
			ADDLOG("COMBAT TO FLIGHT END\n");
			anim->SetAnimationIndex(5);
			isEnd = true;
		}
	}

	if (isEnd == true)
		return FLIGHT_IDLE;

	return COMBAT_TO_FLIGHT;
}

int FlightDodgeState::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();
	std::shared_ptr<Player> player = obj->GetComponent<Player>();

	bool isEnd = false;
	obj->GetTransform()->SetWorldRotation(Vec3::Zero);
	player->Move();
	for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = obj->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(4);

		if (anim->GetAnimationEnd())
		{
			anim->SetAnimationIndex(5);
			isEnd = true;
		}
	}

	if (isEnd == true)
		return FLIGHT_IDLE;

	return FLIGHT_DODGE;
}

int FlightDeadState::handleInput()
{
	std::shared_ptr<GameObject> obj = _object.lock();

	obj->GetTransform()->SetWorldRotation(Vec3::Zero);
	for (int i = 0; i < obj->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> object = obj->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject();
		std::shared_ptr<NierAnimator> anim = std::static_pointer_cast<NierAnimator>(object->GetAnimator());
		anim->SetAnimationIndex(9);

		if (anim->GetAnimationEnd())
		{
			obj->SetActive(false);
		}
	}

	return FLIGHT_DEAD;
}
