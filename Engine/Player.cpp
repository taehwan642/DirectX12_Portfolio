#include "pch.h"
#include "Player.h"
#include "Input.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "GameObjectState.h"
#include "PlayerState.h"
#include "Engine.h"

Player::Player()
{
	MONOCLASSNAME(Player);
	_speed = 50.f;
	_hp = 1.f;
	_damage = 0.f;
	_stateManager = std::make_shared<StateManager>();
}

Player::~Player()
{
}

void Player::OnCollisionEnter(std::shared_ptr<class BaseCollider> collider)
{
}

void Player::Awake()
{
	_stateManager->AddState(FLIGHT_IDLE, std::make_shared<FlightIdleState>(GetGameObject()));
	_stateManager->AddState(COMBAT_IDLE, std::make_shared<CombatIdleState>(GetGameObject()));
	_stateManager->AddState(FLIGHT_FIRE, std::make_shared<FlightFireState>(GetGameObject()));
	_stateManager->AddState(COMBAT_FIRE, std::make_shared<CombatFireState>(GetGameObject()));
	_stateManager->AddState(COMBAT_ATTACK1, std::make_shared<CombatAttack1State>(GetGameObject()));
	_stateManager->AddState(COMBAT_ATTACK2, std::make_shared<CombatAttack2State>(GetGameObject()));
	_stateManager->AddState(FLIGHT_TO_COMBAT, std::make_shared<FlightToCombatState>(GetGameObject()));
	_stateManager->AddState(COMBAT_TO_FLIGHT, std::make_shared<CombatToFlightState>(GetGameObject()));
	_stateManager->AddState(FLIGHT_DODGE, std::make_shared<FlightDodgeState>(GetGameObject()));
	_stateManager->AddState(FLIGHT_DEAD, std::make_shared<FlightDeadState>(GetGameObject()));
	_stateManager->ChangeState(FLIGHT_IDLE);
}

void Player::LateUpdate()
{
	_stateManager->UpdateState();
}

#ifdef TOOL
void Player::DragAndDrop(size_t hash)
{
}
#endif

void Player::Move()
{
	Vec3 pos = GetTransform()->GetWorldPosition();

	if (INPUT->GetButton(KEY_TYPE::UP))
	{
		pos -= GetTransform()->GetWorldTransform()->GetLook() * _speed * DELTA_TIME;
		ADDLOG("adf %d\n", 5);
	}

	if (INPUT->GetButton(KEY_TYPE::DOWN))
		pos += GetTransform()->GetWorldTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::LEFT))
		pos += GetTransform()->GetWorldTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::RIGHT))
		pos -= GetTransform()->GetWorldTransform()->GetRight() * _speed * DELTA_TIME;

	GetTransform()->SetWorldPosition(pos);
}

void Player::Attack()
{
	
}