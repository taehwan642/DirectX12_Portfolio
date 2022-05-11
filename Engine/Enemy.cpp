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
#include "BoxCollider.h"
#include "RaycastManager.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "LaserScript.h"
#include "EnemyMovement.h"

Enemy::Enemy() : _stateManager(std::make_shared<StateManager>())
{
	MONOCLASSNAME(Enemy);
	_speed = 50.f;
	_hp = 1;
	_damage = 3;
	_stateManager = std::make_shared<StateManager>();
	_invincibleTime = 0.1f;
	SetEnemyMovementType(EnemyMovmentType::LERP);
}

Enemy::~Enemy()
{
}

void Enemy::Spawn(const Vec3& worldPosition)
{
	_deltaTime = 0.f;
}

void Enemy::OnCollisionEnter(std::shared_ptr<class BaseCollider> collider)
{
	if (std::shared_ptr<Player> p = collider->GetGameObject()->GetComponent<Player>(); p != nullptr)
	{
		if (!IsInvincible())
		{
			// ���� ���� dodge���� �ƴ϶��;
			GetDamage(p->_damage);
			ADDLOG("Player %d, HP LEFT : %d\n", p->_damage, _hp);
		}
	}
	else if (std::shared_ptr<PlayerBullet> pb = collider->GetGameObject()->GetComponent<PlayerBullet>(); pb != nullptr)
	{
		if (!IsInvincible())
		{
			// ���� ���� dodge���� �ƴ϶��;
			GetDamage(pb->_damage);
			ADDLOG("Player Bullet Damage %d, HP LEFT : %d\n", pb->_damage, _hp);
		}
	}
	else
	{
		// Į?
		if (!IsInvincible())
		{
			// ���� ���� dodge���� �ƴ϶��;
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
	if (_enemyType == EnemyShootingType::BULLET)
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
	else if (_enemyType == EnemyShootingType::LASER)
	{
		_deltaTime += DELTA_TIME;
		if (_deltaTime > 0)
		{
			if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("EnemyLaser"); poolObj != nullptr)
			{
				std::shared_ptr<LaserScript> laser = poolObj->GetComponent<LaserScript>();
				laser->Spawn(GetGameObject()->GetTransform()->GetWorldPosition());
				laser->_attachedObject = GetGameObject();
			}
			else
			{
				std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
				object->AddComponent(std::make_shared<TransformComponent>());
				object->GetTransform()->SetWorldScale(Vec3(2, 2, 10));
				object->SetCheckFrustum(false);

				std::shared_ptr<GameObject> bulletPrefab = GET_SINGLE(Resources)->Get<GameObject>(L"bullet.fbx0");
				std::shared_ptr<MeshRenderer> mr = bulletPrefab->GetMeshRenderer()->Clone();
				object->AddComponent(mr);
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Laser"));

				std::shared_ptr<LaserScript> laser = std::make_shared<LaserScript>();
				object->AddComponent(laser);
				laser->Spawn(GetGameObject()->GetTransform()->GetWorldPosition());
				laser->_attachedObject = GetGameObject();

				std::shared_ptr<BoxCollider> bc = std::make_shared<BoxCollider>();
				object->AddComponent(bc);
				bc->SetExtent(object->GetTransform()->GetWorldScale());

				std::shared_ptr<GameObject> laserParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"LaserParent");

				object->SetName(L"LaserChild" + std::to_wstring(laserParent->GetTransform()->GetChildCount()));
				object->GenerateHash();

				object->GetTransform()->SetParent(laserParent->GetTransform());

				GET_SINGLE(ObjectPool)->AddPoolObject("EnemyLaser", object);

				GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY, object);

				GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
			}
			_deltaTime = -999999.f;
		}
	}
}

void Enemy::SetEnemyMovementType(EnemyMovmentType type)
{
	switch (type)
	{
	case EnemyMovmentType::LERP:
		_enemyMovement = std::make_shared<EnemyLerpMovement>(Vec3::Zero, Vec3::One * 30);
		break;
	case EnemyMovmentType::CATMULLROM:
		_enemyMovement = std::make_shared<EnemyCatmullRomMovement>(std::array<Vec3,4>{ Vec3::Zero, Vec3::One * 30, Vec3::One * 5, Vec3::One });
		break;
	default:
		break;
	}
}