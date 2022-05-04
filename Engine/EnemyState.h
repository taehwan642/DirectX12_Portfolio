#pragma once
#include "GameObjectState.h"

enum EnemyState
{
	ENEMY_IDLE,
	ENEMY_SHOOT_BULLET,
	ENEMY_SHOOT_LASER,
	ENEMY_DEAD
};

class EnemyIdleState final :
	public GameObjectState
{
public:
	explicit EnemyIdleState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class EnemyShootBulletState final :
	public GameObjectState
{
public:
	explicit EnemyShootBulletState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class EnemyShootLaserState final :
	public GameObjectState
{
public:
	explicit EnemyShootLaserState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class EnemyDeadState final :
	public GameObjectState
{
public:
	explicit EnemyDeadState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};