#include "pch.h"
#include "EnemyBullet.h"
#include "Input.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "EffectManagerScript.h"

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::LateUpdate()
{
	if (INPUT->GetButtonDown(KEY_TYPE::KEY_3))
	{
		std::shared_ptr<GameObject> obj = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(_testObject);
		if (obj != nullptr)
		{
			// Spawn Effect
			obj->GetComponent<EffectManagerScript>()->SpawnEffect("Explosion3", GetTransform()->GetWorldPosition());
		}
	}

	Vec3 pos = GetTransform()->GetWorldPosition();

	if (INPUT->GetButton(KEY_TYPE::Z))
		pos += GetTransform()->GetWorldTransform()->GetRight() * 100.f * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::C))
		pos -= GetTransform()->GetWorldTransform()->GetRight() * 100.f * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::Q))
		pos += GetTransform()->GetWorldTransform()->GetLook() * 100.f * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::E))
		pos -= GetTransform()->GetWorldTransform()->GetLook() * 100.f * DELTA_TIME;

	GetTransform()->SetWorldPosition(pos);
}

#ifdef TOOL
void EnemyBullet::DragAndDrop(size_t hash)
{
	_testObject = hash;
}
#endif