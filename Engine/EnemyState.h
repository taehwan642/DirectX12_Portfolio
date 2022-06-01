#pragma once
#include "GameObjectState.h"

enum EnemyState
{
	ENEMY_IDLE,
	ENEMY_DEAD
};

class EnemyIdleState final :
	public GameObjectState
{
public:
	explicit EnemyIdleState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};

class EnemyDeadState final :
	public GameObjectState
{
public:
	explicit EnemyDeadState(std::shared_ptr<GameObject> obj) : GameObjectState(obj) {};
	[[nodiscard]] int handleInput() override;
};