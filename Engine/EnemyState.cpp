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

	if (enemy->IsDead())
		return ENEMY_DEAD;

	return ENEMY_IDLE;
}

int EnemyDeadState::handleInput()
{
	_object.lock()->SetActive(false);
	return ENEMY_DEAD;
}