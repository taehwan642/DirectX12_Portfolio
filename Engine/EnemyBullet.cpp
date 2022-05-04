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

EnemyBullet::~EnemyBullet()
{
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

	GetTransform()->SetWorldPosition(pos);

	static float inputTime = 0;
	inputTime += DELTA_TIME;
	GetMeshRenderer()->GetMaterial()->SetFloat(0, inputTime);
	GetMeshRenderer()->GetMaterial()->SetInt(3, static_cast<int32>(_state));
}

#ifdef TOOL
void EnemyBullet::DragAndDrop(size_t hash)
{
	_testObject = hash;
}
#endif