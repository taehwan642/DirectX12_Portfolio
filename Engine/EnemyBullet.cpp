#include "pch.h"
#include "EnemyBullet.h"
#include "Input.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "BaseCollider.h"
#include "BoneCollider.h"
#include "SphereCollider.h"
#include "AudioSource.h"
#include "PlayerBullet.h"

EnemyBullet::EnemyBullet()
{
	MONOCLASSNAME(EnemyBullet);
	Spawn(1, 10.f, 1);
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Spawn(int hp, float speed, int damage)
{
	_aliveTime = 0.f;
	Character::Spawn(hp, speed, damage);
}

void EnemyBullet::OnCollisionEnter(const CollisionOutput& collider)
{
	std::shared_ptr<GameObject> obj = collider.collider->GetGameObject();
	switch (collider.type)
	{
	case CollisionObjectType::PLAYER_BULLET:
	{
		if (_state == EnemyBulletState::DESTROYABLE)
		{
			int damage = obj->GetComponent<PlayerBullet>()->_damage;
			if (GetDamage(damage))
			{
				GetGameObject()->SetActive(false);
			}
		}
		break;
	}
	default:
		break;
	}
}

void EnemyBullet::LateUpdate()
{
	static float inputTime = 0;
	inputTime += DELTA_TIME;
	GetMeshRenderer()->GetMaterial()->SetFloat(0, inputTime);
	GetMeshRenderer()->GetMaterial()->SetInt(3, static_cast<int32>(_state));

	_aliveTime += DELTA_TIME;
	if (_aliveTime > 2.f)
	{
		GetGameObject()->SetActive(false);
	}
	Move();
}

void EnemyBullet::Move()
{
	Vec3 pos = GetTransform()->GetWorldPosition();
	pos -= _direction * _speed * DELTA_TIME;
	GetTransform()->SetWorldPosition(pos);
}

#ifdef TOOL
void EnemyBullet::DragAndDrop(size_t hash)
{
	_testObject = hash;
}
#endif