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
#include "ObjectPool.h"
#include "EnemyBullet.h"
#include "SceneManager.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Resources.h"
#include "Scene.h"
#include "Animator.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "RaycastManager.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "LaserScript.h"

Enemy::Enemy()
{
	MONOCLASSNAME(Enemy);
	_speed = 50.f;
	_hp = 1;
	_damage = 3;
	_stateManager = std::make_shared<StateManager>();
	_invincibleTime = 0.1f;
}

Enemy::~Enemy()
{
}

void Enemy::Spawn(const Vec3& worldPosition)
{
}

void Enemy::OnCollisionEnter(std::shared_ptr<class BaseCollider> collider)
{
	if (std::shared_ptr<Player> p = collider->GetGameObject()->GetComponent<Player>(); p != nullptr)
	{
		if (!IsInvincible())
		{
			// 만약 현재 dodge중이 아니라면;
			GetDamage(p->_damage);
			ADDLOG("Player %d, HP LEFT : %d\n", p->_damage, _hp);
		}
	}
	else if (std::shared_ptr<PlayerBullet> pb = collider->GetGameObject()->GetComponent<PlayerBullet>(); pb != nullptr)
	{
		if (!IsInvincible())
		{
			// 만약 현재 dodge중이 아니라면;
			GetDamage(pb->_damage);
			ADDLOG("Player Bullet Damage %d, HP LEFT : %d\n", pb->_damage, _hp);
		}
	}
	else
	{
		// 칼?
		if (!IsInvincible())
		{
			// 만약 현재 dodge중이 아니라면;
			GetDamage(2);
			ADDLOG("Player knife %d, HP LEFT : %d\n", 2, _hp);
		}
	}
}

void Enemy::Awake()
{
	_stateManager->AddState(ENEMY_IDLE, std::make_shared<EnemyIdleState>(GetGameObject()));
	_stateManager->AddState(ENEMY_SHOOT_BULLET, std::make_shared<EnemyShootBulletState>(GetGameObject()));
	_stateManager->AddState(ENEMY_SHOOT_LASER, std::make_shared<EnemyShootLaserState>(GetGameObject()));
	_stateManager->AddState(ENEMY_DEAD, std::make_shared<EnemyDeadState>(GetGameObject()));
	_stateManager->ChangeState(ENEMY_IDLE);
	GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY, GetGameObject());
}

void Enemy::LateUpdate()
{
	_stateManager->UpdateState();
	UpdateInvincibleTime();
}

void Enemy::Move()
{
}

void Enemy::Attack()
{
	if (_enemyType == EnemyType::BULLET)
	{
		_deltaTime += DELTA_TIME;
		if (_deltaTime > _fireSpeed)
		{
			if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("EnemyBullet"); poolObj != nullptr)
			{
				std::shared_ptr<EnemyBullet> eb = poolObj->GetComponent<EnemyBullet>();
				eb->Spawn(GetGameObject()->GetTransform()->GetWorldPosition());
				eb->_direction = GetTransform()->GetWorldTransform()->GetLook();
			}
			else
			{
				std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
				object->AddComponent(std::make_shared<TransformComponent>());

				std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
				object->AddComponent(mr);
				mr->SetMesh(GET_SINGLE(Resources)->LoadSphereMesh());
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet"));

				std::shared_ptr<EnemyBullet> eb = std::make_shared<EnemyBullet>();
				object->AddComponent(eb);
				eb->_direction = GetTransform()->GetWorldTransform()->GetLook();
				eb->Spawn(GetGameObject()->GetTransform()->GetWorldPosition());

				std::shared_ptr<SphereCollider> sc = std::make_shared<SphereCollider>();
				object->AddComponent(sc);
				sc->SetRadius(object->GetTransform()->GetWorldScale().x);

				std::shared_ptr<GameObject> bulletParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EnemyBulletParent");

				object->SetName(L"EnemyBulletChild" + std::to_wstring(bulletParent->GetTransform()->GetChildCount()));
				object->GenerateHash();

				object->GetTransform()->SetParent(bulletParent->GetTransform());

				GET_SINGLE(ObjectPool)->AddPoolObject("EnemyBullet", object);

				GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY, object);

				GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
			}
			_deltaTime = 0.f;
		}
	}
	else if (_enemyType == EnemyType::LASER)
	{
		if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("EnemyLaser"); poolObj != nullptr)
		{
			std::shared_ptr<LaserScript> laser = poolObj->GetComponent<LaserScript>();
			laser->Spawn(GetGameObject()->GetTransform()->GetWorldPosition());
		}
		else
		{
			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
			object->AddComponent(std::make_shared<TransformComponent>());

			std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
			object->AddComponent(mr);
			mr->SetMesh(GET_SINGLE(Resources)->LoadSphereMesh());
			mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet"));

			std::shared_ptr<LaserScript> eb = std::make_shared<LaserScript>();
			object->AddComponent(eb);
			eb->Spawn(GetGameObject()->GetTransform()->GetWorldPosition());

			std::shared_ptr<SphereCollider> sc = std::make_shared<SphereCollider>();
			object->AddComponent(sc);
			sc->SetRadius(object->GetTransform()->GetWorldScale().x);

			std::shared_ptr<GameObject> bulletParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EnemyBulletParent");

			object->SetName(L"EnemyBulletChild" + std::to_wstring(bulletParent->GetTransform()->GetChildCount()));
			object->GenerateHash();

			object->GetTransform()->SetParent(bulletParent->GetTransform());

			GET_SINGLE(ObjectPool)->AddPoolObject("EnemyLaser", object);

			GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY, object);

			GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
		}
	}
}