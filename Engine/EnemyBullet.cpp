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
		//GetGameObject()->GetAudioSource()->LoadAudio(L"../Resources/Audio/media_Explo1.wav");
		//GetGameObject()->GetAudioSource()->Play();
	}

	Vec3 pos = GetTransform()->GetWorldPosition();

	//if (INPUT->GetButton(KEY_TYPE::Z))
	//	pos += GetTransform()->GetWorldTransform()->GetRight() * 100.f * DELTA_TIME;
	//
	//if (INPUT->GetButton(KEY_TYPE::C))
	//	pos -= GetTransform()->GetWorldTransform()->GetRight() * 100.f * DELTA_TIME;
	//
	//if (INPUT->GetButton(KEY_TYPE::Q))
	//	pos += GetTransform()->GetWorldTransform()->GetLook() * 100.f * DELTA_TIME;
	//
	//if (INPUT->GetButton(KEY_TYPE::E))
	//	pos -= GetTransform()->GetWorldTransform()->GetLook() * 100.f * DELTA_TIME;

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