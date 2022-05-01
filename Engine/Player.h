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

#ifdef TOOL
	virtual void DragAndDrop(size_t hash) override;
#endif

	virtual void Move() override;
	virtual void Attack() override;

public:
	std::shared_ptr<StateManager> _stateManager;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

