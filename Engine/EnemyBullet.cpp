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

void EnemyBullet::OnCollisionEnter(CollisionOutput collider)
{
	if (collider.collider->GetColliderType() == ColliderType::Sphere)
	{
	}
	else if (collider.collider->GetColliderType() == ColliderType::Bone)
	{
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