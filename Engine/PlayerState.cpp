#include "pch.h"
#include "PlayerState.h"
#include "Input.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Player.h"

int FlightIdleState::handleInput()
{
	_object.lock()->GetComponent<Player>()->Move();

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
	return FLIGHT_DEAD;
}
