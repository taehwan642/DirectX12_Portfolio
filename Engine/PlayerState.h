#pragma once
#include "GameObjectState.h"
enum PlayerState
{
	FLIGHT_IDLE,
	COMBAT_IDLE,
	FLIGHT_FIRE,
	COMBAT_FIRE,
	COMBAT_ATTACK1,
	COMBAT_ATTACK2,
	FLIGHT_TO_COMBAT,
	COMBAT_TO_FLIGHT,
	FLIGHT_DODGE,
	FLIGHT_DEAD,
};

class FlightIdleState final :
	public GameObjectState
{
public:
	explicit FlightIdleState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class CombatIdleState final :
	public GameObjectState
{
public:
	explicit CombatIdleState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class FlightFireState final :
	public GameObjectState
{
public:
	explicit FlightFireState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class CombatFireState final :
	public GameObjectState
{
public:
	explicit CombatFireState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class CombatAttack1State final :
	public GameObjectState
{
public:
	explicit CombatAttack1State(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class CombatAttack2State final :
	public GameObjectState
{
public:
	explicit CombatAttack2State(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class FlightToCombatState final :
	public GameObjectState
{
public:
	explicit FlightToCombatState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class CombatToFlightState final :
	public GameObjectState
{
public:
	explicit CombatToFlightState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class FlightDodgeState final :
	public GameObjectState
{
public:
	explicit FlightDodgeState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class FlightDeadState final :
	public GameObjectState
{
public:
	explicit FlightDeadState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};