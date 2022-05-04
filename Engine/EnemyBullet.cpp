#include "pch.h"
#include "EnemyBullet.h"
#include "Input.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "EffectManagerScript.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "BaseCollider.h"
#include "BoneCollider.h"
#include "SphereCollider.h"
#include "AudioSource.h"

EnemyBullet::EnemyBullet()
{
	MONOCLASSNAME(EnemyBullet);
	_speed = 10.f;
	_hp = 1;
	_damage = 1;
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Spawn(const Vec3& worldPosition)
{
	_hp = 1;
	GetTransform()->SetWorldPosition(worldPosition);
	_aliveTime = 0.f;
}

void EnemyBullet::OnCollisionEnter(std::shared_ptr<class BaseCollider> collider)
{
	if (collider->GetColliderType() == ColliderType::Sphere)
	{
	}
	else if (collider->GetColliderType() == ColliderType::Bone)
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