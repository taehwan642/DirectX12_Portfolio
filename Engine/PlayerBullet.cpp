#include "pch.h"
#include "PlayerBullet.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "GameObject.h"

PlayerBullet::PlayerBullet()
{
	MONOCLASSNAME(PlayerBullet);
	Spawn(1, 50.f, 1);
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Spawn(int hp, float speed, int damage)
{
	ADDLOG("BulletSpawn\n");
	_aliveTime = 0.f;
	Character::Spawn(hp, speed, damage);
}

void PlayerBullet::OnCollisionEnter(CollisionOutput collider)
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
	pos -= _direction * _speed * DELTA_TIME;
	GetTransform()->SetWorldPosition(pos);
}