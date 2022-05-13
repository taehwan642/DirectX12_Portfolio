#pragma once
#include "MonoBehaviour.h"
#include "Character.h"

enum class EnemyShootingType
{
	BULLET,
	LASER
};

enum class EnemyMovmentType
{
	LERP,
	CATMULLROM
};

class StateManager;
class EnemyMovement;
class Enemy :
	public MonoBehaviour, public Character
{
public:
	Enemy();

	virtual ~Enemy();

	virtual void Spawn(int hp, float speed, int damage) override;

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override;
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void Awake() override;
	virtual void LateUpdate() override;

	virtual void Move() override;
	virtual void Attack() override;

	EnemyShootingType GetEnemyType() { return _enemyType; }
	void SetEnemyMovementType(EnemyMovmentType type);

private:
	std::shared_ptr<StateManager> _stateManager;
	std::shared_ptr<EnemyMovement> _enemyMovement;
	float _fireSpeed = 0.4f;
	float _deltaTime = 0.0f;

	EnemyShootingType _enemyType = EnemyShootingType::LASER;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};
