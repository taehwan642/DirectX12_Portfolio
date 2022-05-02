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

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override;
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void Awake() override;
	virtual void LateUpdate() override;

	virtual void Move() override;
	virtual void Attack() override;

public:
	std::shared_ptr<StateManager> _stateManager;
	float _fireSpeed = 0.1f;
	float _deltaTime = 0.0f;
	bool _isCombatMode = false;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

