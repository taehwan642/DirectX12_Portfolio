#pragma once
#include "MonoBehaviour.h"
#include "Character.h"

enum class EnemyShootingType
{
	LASER,
	RAPID,
	HEAVY,
	THREEWAY
};

enum class EnemyMovmentType
{
	LERP,
	CATMULLROM
};

class StateManager;
class EnemyMovement;
class EnemyShooting;
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

	EnemyShootingType GetEnemyShootingType() { return _enemyType; }
	void SetEnemyShootingType(EnemyShootingType type);

	void SetEnemyMovementType(EnemyMovmentType type, const std::vector<Vec3>& positions);

private:
	std::shared_ptr<StateManager> _stateManager;
	std::shared_ptr<EnemyMovement> _enemyMovement;
	std::shared_ptr<EnemyShooting> _enemyShooting;
	float _fireSpeed = 0.4f;
	float _deltaTime = 0.0f;

	EnemyShootingType _enemyType = EnemyShootingType::LASER;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};
