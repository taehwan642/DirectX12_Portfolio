#include "pch.h"
#include "EnemyState.h"
#include "Input.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Enemy.h"
#include "NierAnimator.h"
#include "AudioSource.h"
#include "Engine.h"

int EnemyIdleState::handleInput()
{
	std::shared_ptr<Enemy> enemy = _object.lock()->GetComponent<Enemy>();
	enemy->Move();
	enemy->Attack();
	return ENEMY_IDLE;
}

int EnemyShootBulletState::handleInput()
{
	std::shared_ptr<Enemy> enemy = _object.lock()->GetComponent<Enemy>();
	enemy->Move();
	enemy->Attack();
	return ENEMY_SHOOT_BULLET;
}

int EnemyShootLaserState::handleInput()
{
	std::shared_ptr<Enemy> enemy = _object.lock()->GetComponent<Enemy>();
	enemy->Move();
	enemy->Attack();
	return ENEMY_SHOOT_LASER;
}

int EnemyDeadState::handleInput()
{
	return ENEMY_DEAD;
}