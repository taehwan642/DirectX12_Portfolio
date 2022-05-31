#include "pch.h"
#include "Enemy.h"
#include "Input.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "GameObjectState.h"
#include "EnemyState.h"
#include "Engine.h"
#include "AudioSource.h"
#include "GameObject.h"
#include "Animator.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "EnemyMovement.h"
#include "EnemyShooting.h"

Enemy::Enemy()
{
	MONOCLASSNAME(Enemy);
	Character::Spawn(1, 50.f, 3);
	_invincibleTime = 0.1f;
}

Enemy::~Enemy()
{
}

void Enemy::Spawn(int hp, float speed, int damage)
{
	if (_stateManager == nullptr)
	{
		_stateManager = std::make_shared<StateManager>();
		_stateManager->AddState(ENEMY_IDLE, std::make_shared<EnemyIdleState>(GetGameObject()));
		_stateManager->AddState(ENEMY_SHOOT_BULLET, std::make_shared<EnemyShootBulletState>(GetGameObject()));
		_stateManager->AddState(ENEMY_SHOOT_LASER, std::make_shared<EnemyShootLaserState>(GetGameObject()));
		_stateManager->AddState(ENEMY_DEAD, std::make_shared<EnemyDeadState>(GetGameObject()));
		_stateManager->ChangeState(ENEMY_IDLE);
	}

	Character::Spawn(hp, speed, damage);
}

void Enemy::OnCollisionEnter(const CollisionOutput& collider)
{
	std::shared_ptr<GameObject> obj = collider.collider->GetGameObject();
	switch (collider.type)
	{
	case CollisionObjectType::PLAYER:
	{
		if (!IsInvincible())
		{
			// 만약 현재 dodge중이 아니라면;
			int damage = obj->GetComponent<Player>()->_damage;
			GetDamage(damage);
			ADDLOG("Player %d, HP LEFT : %d\n", damage, _hp);
		}	
		break;
	}
	case CollisionObjectType::PLAYER_BULLET:
	{
		if (!IsInvincible())
		{
			// 만약 현재 dodge중이 아니라면;
			int damage = obj->GetComponent<PlayerBullet>()->_damage;
			GetDamage(damage); 
			ADDLOG("Player Bullet Damage %d, HP LEFT : %d\n", damage, _hp);
		}
		break;
	}
	case CollisionObjectType::PLAYER_WEAPON:
	{
		if (!IsInvincible())
		{
			// 만약 현재 dodge중이 아니라면;
			GetDamage(2);
			ADDLOG("Player knife %d, HP LEFT : %d\n", 2, _hp);
		}
		break;
	}
	default:
		break;
	}
}

void Enemy::Awake()
{
	
}

void Enemy::LateUpdate()
{
	_stateManager->UpdateState();
	UpdateInvincibleTime();
}

void Enemy::Move()
{
	GetTransform()->SetWorldPosition(_enemyMovement->GetMovmentResult());
}

void Enemy::Attack()
{
	_enemyShooting->Shoot();
}

void Enemy::SetEnemyShootingType(EnemyShootingType type)
{
	switch (type)
	{
	case EnemyShootingType::LASER:
		_enemyShooting = std::make_shared<EnemyLaserShooting>();
		break;
	case EnemyShootingType::RAPID:
		_enemyShooting = std::make_shared<EnemyRapidShooting>();
		break;
	case EnemyShootingType::HEAVY:
		_enemyShooting = std::make_shared<EnemyHeavyShooting>();
		break;
	case EnemyShootingType::THREEWAY:
		_enemyShooting = std::make_shared<EnemyThreeWayShooting>();
		break;
	default:
		break;
	}

	_enemyShooting->SetAttatchedObject(GetGameObject());
}

void Enemy::SetEnemyMovementType(EnemyMovmentType type, const std::vector<Vec3>& positions)
{
	switch (type)
	{
	case EnemyMovmentType::LERP:
		_enemyMovement = std::make_shared<EnemyLerpMovement>(positions[0], positions[1]);
		break;
	case EnemyMovmentType::CATMULLROM:
		_enemyMovement = std::make_shared<EnemyCatmullRomMovement>(std::array<Vec3,4>{ positions[0], positions[1], positions[2], positions[3] });
		break;
	default:
		break;
	}
}
