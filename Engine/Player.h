#pragma once
#include "Character.h"
#include "MonoBehaviour.h"

class StateManager;
class Player :
    public MonoBehaviour, public Character
{
public:
	Player();

	virtual ~Player();

	virtual void OnCollisionEnter(const CollisionOutput& collider) override;
	virtual void OnCollisionStay(const CollisionOutput& collider) override {}
	virtual void OnCollisionExit(const CollisionOutput& collider) override {}

	virtual void Awake() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void Move() override;
	virtual void Attack() override;

	void SetSwordCollider();

	bool Get9SMode() { return _9SMode; }
	bool GetFlightMode() { return _flightMode; }

	void Change9S2BMode();
	int ChangeFlightCombatMode();

public:
	std::shared_ptr<StateManager> _stateManager;
	float _fireSpeed = 0.1f;
	float _deltaTime = 0.0f;
	bool _9SMode = true;
	bool _flightMode = true;

	SimpleMath::Plane _p;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

