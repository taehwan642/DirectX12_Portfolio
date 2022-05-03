#include "pch.h"
#include "PlayerBullet.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "GameObject.h"

PlayerBullet::PlayerBullet()
{
	MONOCLASSNAME(PlayerBullet);
	_speed = 50.f;
	_hp = 1;
	_damage = 1;
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Spawn(const Vec3& worldPosition)
{
	ADDLOG("BulletSpawn\n");
	_hp = 1;
	GetTransform()->SetWorldPosition(worldPosition);
	_aliveTime = 0.f;
}

void PlayerBullet::OnCollisionEnter(std::shared_ptr<class BaseCollider> collider)
{
}

void PlayerBullet::Awake()
{
}

void PlayerBullet::LateUpdate()
{
	_aliveTime += DELTA_TIME;
	if (_aliveTime > 2.f)
	{
		GetGameObject()->SetActive(false);
	}
	Move();
}

void PlayerBullet::Move()
{
	
	Vec3 pos = GetTransform()->GetWorldPosition();
	pos -= GetTransform()->GetWorldTransform()->GetLook() * _speed * DELTA_TIME;
	GetTransform()->SetWorldPosition(pos);
}