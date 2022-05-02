#include "pch.h"
#include "PlayerBullet.h"
#include "Engine.h"

PlayerBullet::PlayerBullet()
{
	MONOCLASSNAME(PlayerBullet);
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Spawn()
{
	ADDLOG("BulletSpawn");
}

void PlayerBullet::OnCollisionEnter(std::shared_ptr<class BaseCollider> collider)
{
}

void PlayerBullet::Awake()
{
}

void PlayerBullet::LateUpdate()
{
}

void PlayerBullet::Move()
{
}