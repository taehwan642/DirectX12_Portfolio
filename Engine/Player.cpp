#include "pch.h"
#include "Player.h"
#include "Input.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "GameObjectState.h"
#include "PlayerState.h"
#include "Engine.h"
#include "AudioSource.h"
#include "GameObject.h"
#include "ObjectPool.h"
#include "PlayerBullet.h"
#include "SceneManager.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Resources.h"
#include "Scene.h"
#include "Animator.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "RaycastManager.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "EffectManagerScript.h"
#include "LaserScript.h"

Player::Player()
{
	MONOCLASSNAME(Player);
	Spawn(10, 50.f, 1);
	_stateManager = std::make_shared<StateManager>();
	_invincibleTime = 0.3f;
}

Player::~Player()
{
}

void Player::OnCollisionEnter(CollisionOutput collider)
{
	std::shared_ptr<GameObject> obj = collider.collider->GetGameObject();
	switch (collider.type)
	{
	case CollisionObjectType::ENEMY:
	{
		if (!IsInvincible())
		{
			int damage = obj->GetComponent<Enemy>()->_damage;
			// 만약 현재 dodge중이 아니라면;
			GetDamage(damage);
			ADDLOG("Enemy Damage %d, HP LEFT : %d\n", damage, _hp);
		}
		break;
	}
	case CollisionObjectType::ENEMY_BULLET:
	{
		if (!IsInvincible())
		{
			int damage = obj->GetComponent<EnemyBullet>()->_damage;
			// 만약 현재 dodge중이 아니라면;
			GetDamage(damage);
			ADDLOG("Enemy Bullet Damage %d, HP LEFT : %d\n", damage, _hp);
			std::shared_ptr<GameObject> effectManager = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EffectManager");
			effectManager->GetComponent<EffectManagerScript>()->SpawnEffect("Explosion3", GetTransform()->GetWorldPosition());
		}
		break;
	}
	case CollisionObjectType::ENEMY_LASER:
	{
		if (!IsInvincible())
		{
			int damage = obj->GetComponent<LaserScript>()->_damage;
			// 만약 현재 dodge중이 아니라면;
			GetDamage(damage);
			ADDLOG("Laser Damage %d, HP LEFT : %d\n", damage, _hp);
			std::shared_ptr<GameObject> effectManager = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EffectManager");
			effectManager->GetComponent<EffectManagerScript>()->SpawnEffect("Explosion3", GetTransform()->GetWorldPosition());
		}
		break;
	}
	default:
		break;
	}
}

void Player::Awake()
{
	_stateManager->AddState(FLIGHT_IDLE, std::make_shared<FlightIdleState>(GetGameObject()));
	_stateManager->AddState(COMBAT_IDLE, std::make_shared<CombatIdleState>(GetGameObject()));
	_stateManager->AddState(FLIGHT_FIRE, std::make_shared<FlightFireState>(GetGameObject()));
	_stateManager->AddState(COMBAT_FIRE, std::make_shared<CombatFireState>(GetGameObject()));
	_stateManager->AddState(COMBAT_ATTACK1, std::make_shared<CombatAttack1State>(GetGameObject()));
	_stateManager->AddState(COMBAT_ATTACK2, std::make_shared<CombatAttack2State>(GetGameObject()));
	_stateManager->AddState(FLIGHT_TO_COMBAT, std::make_shared<FlightToCombatState>(GetGameObject()));
	_stateManager->AddState(COMBAT_TO_FLIGHT, std::make_shared<CombatToFlightState>(GetGameObject()));
	_stateManager->AddState(FLIGHT_DODGE, std::make_shared<FlightDodgeState>(GetGameObject()));
	_stateManager->AddState(FLIGHT_DEAD, std::make_shared<FlightDeadState>(GetGameObject()));
	_stateManager->ChangeState(FLIGHT_IDLE);
	GetGameObject()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_13.wav");
	GetGameObject()->GetAudioSource()->SetLoop(true);
	GetGameObject()->GetAudioSource()->SetVolume(0.06f);
	GetGameObject()->GetAudioSource()->Play();
	_p = SimpleMath::Plane(GetTransform()->GetWorldPosition(), Vec3(0, 1, 0));
	GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::PLAYER, GetGameObject());

	GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::PLAYER_WEAPON, GetGameObject()->GetTransform()->GetChild(0)->GetChild(15)->GetGameObject());
	GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::PLAYER_WEAPON, GetGameObject()->GetTransform()->GetChild(0)->GetChild(16)->GetGameObject());
	GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::PLAYER_WEAPON, GetGameObject()->GetTransform()->GetChild(0)->GetChild(8)->GetGameObject());
	GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::PLAYER_WEAPON, GetGameObject()->GetTransform()->GetChild(0)->GetChild(9)->GetGameObject());
}

void Player::Update()
{

}

void Player::LateUpdate()
{
	POINT mousePos = INPUT->GetMousePos();
	GET_SINGLE(RaycastManager)->SetRayOriginRayDir(mousePos.x, mousePos.y);
	Vec3 rayOrigin = Vec3(GET_SINGLE(RaycastManager)->_rayOrigin);
	Vec3 rayDir = Vec3(GET_SINGLE(RaycastManager)->_rayDir);

	// plane 위 v = rayOrigin + t * rayDir
	float t = (-_p.D() - (_p.Normal().Dot(rayOrigin))) / _p.Normal().Dot(rayDir);

	Vec3 point = rayOrigin + t * rayDir;

	GetTransform()->SetLookAtWorldRotation(GetTransform()->GetWorldPosition() - point);

	_stateManager->UpdateState();
	SetSwordCollider();

	UpdateInvincibleTime();
}

void Player::Move()
{
	Vec3 pos = GetTransform()->GetWorldPosition();

	if (INPUT->GetButton(KEY_TYPE::UP))
	{
		pos -= GetTransform()->GetWorldTransform()->GetLook() * _speed * DELTA_TIME;
		ADDLOG("Position : %f %f %f\n", GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y, GetTransform()->GetWorldPosition().z);
	}

	if (INPUT->GetButton(KEY_TYPE::DOWN))
		pos += GetTransform()->GetWorldTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::LEFT))
		pos += GetTransform()->GetWorldTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::RIGHT))
		pos -= GetTransform()->GetWorldTransform()->GetRight() * _speed * DELTA_TIME;

	GetTransform()->SetWorldPosition(pos);
}

void Player::Attack()
{
	_deltaTime += DELTA_TIME;
	if (_deltaTime > _fireSpeed)
	{
		GetGameObject()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_30.wav");
		GetGameObject()->GetAudioSource()->SetLoop(false);
		GetGameObject()->GetAudioSource()->SetVolume(0.2f);
		GetGameObject()->GetAudioSource()->Play();
		_deltaTime = 0.f;

		// bone 006 왼쪽
		// bone 010 오른쪽
		Vec3 pos1 = GetGameObject()->GetTransform()->GetChild(0)->GetChild(static_cast<int>(_9SMode))->GetAnimator()->GetBonePosition("bone006");
		Vec3 pos2 = GetGameObject()->GetTransform()->GetChild(0)->GetChild(static_cast<int>(_9SMode))->GetAnimator()->GetBonePosition("bone010"); 

		for (int i = 0; i < 2; ++i)
		{
			if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("Bullet"); poolObj != nullptr)
			{
				std::shared_ptr<PlayerBullet> pb = poolObj->GetComponent<PlayerBullet>();

				pb->Spawn(1, 50.f, 1);

				if (i == 0)
					pb->GetTransform()->SetWorldPosition(pos1);
				else
					pb->GetTransform()->SetWorldPosition(pos2);

				pb->_direction = GetTransform()->GetWorldTransform()->GetLook();
				poolObj->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());
			}
			else
			{
				std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
				std::shared_ptr<GameObject> bulletPrefab = GET_SINGLE(Resources)->Get<GameObject>(L"bullet.fbx0");
				object->AddComponent(std::make_shared<TransformComponent>());

				std::shared_ptr<PlayerBullet> pb = std::make_shared<PlayerBullet>();
				object->AddComponent(pb);

				pb->Spawn(1, 50.f, 1);

				if (i == 0)
					pb->GetTransform()->SetWorldPosition(pos1);
				else
					pb->GetTransform()->SetWorldPosition(pos2);

				pb->_direction = GetTransform()->GetWorldTransform()->GetLook();
				pb->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());

				std::shared_ptr<MeshRenderer> mr = bulletPrefab->GetMeshRenderer()->Clone();
				object->AddComponent(mr);

				std::shared_ptr<SphereCollider> sc = std::make_shared<SphereCollider>();
				object->AddComponent(sc);
				sc->SetRadius(1.5f);

				std::shared_ptr<GameObject> bulletParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"BulletParent");

				object->SetName(L"BulletChild" + std::to_wstring(bulletParent->GetTransform()->GetChildCount()));
				object->GenerateHash();

				object->GetTransform()->SetParent(bulletParent->GetTransform());

				GET_SINGLE(ObjectPool)->AddPoolObject("Bullet", object);

				GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::PLAYER_BULLET, object);

				GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
			}
		}
	}
}

void Player::SetSwordCollider()
{
	// 팔꿈치
	// bone 008 왼쪽
	// bone 012 오른쪽
	// 손
	// bone 013 왼쪽
	// bone 009 오른쪽

	int leftObjectIndex = (_9SMode == true) ? 15 : 8;
	int rightObjectIndex = (_9SMode == true) ? 16 : 9;

	Vec3 leftElbow = GetGameObject()->GetTransform()->GetChild(0)->GetChild(leftObjectIndex)->GetAnimator()->GetBonePosition("bone008");
	Vec3 leftHand = GetGameObject()->GetTransform()->GetChild(0)->GetChild(leftObjectIndex)->GetAnimator()->GetBonePosition("bone013");

	Vec3 leftHandToElbowDir = leftHand - leftElbow;
	leftHandToElbowDir.Normalize();

	Vec3 rightElbow = GetGameObject()->GetTransform()->GetChild(0)->GetChild(rightObjectIndex)->GetAnimator()->GetBonePosition("bone012");
	Vec3 rightHand = GetGameObject()->GetTransform()->GetChild(0)->GetChild(rightObjectIndex)->GetAnimator()->GetBonePosition("bone009");

	Vec3 rightHandToElbowDir = rightHand - rightElbow;
	rightHandToElbowDir.Normalize();

	// 그 Object의 Collider의 center를 구하기

	Vec3 leftColliderCenter = leftHand + leftHandToElbowDir * 1.5f;
	Vec3 rightColliderCenter = rightHand + rightHandToElbowDir * 1.5f;

	// BonePosition에는 이미 GameObject의 WorldPosition이 더해진 상태.
	// 이 때 Collider의 center의 공식은 center + WorldPosition.
	// 그럼 BonePosition + WorldPosition은 WorldPosition을 두번 더하는 것.
	// 그렇기 때문에 GetWorldPosition으로 WorldPosition을 빼줘서, BonePosition만 남기게 한다.
	leftColliderCenter -= GetGameObject()->GetTransform()->GetWorldPosition();
	rightColliderCenter -= GetGameObject()->GetTransform()->GetWorldPosition();

	std::shared_ptr<BaseCollider> leftBC = GetGameObject()->GetTransform()->GetChild(0)->GetChild(leftObjectIndex)->GetGameObject()->GetCollider();
	std::static_pointer_cast<SphereCollider>(leftBC)->SetCenter(leftColliderCenter);
	std::shared_ptr<BaseCollider> rightBC = GetGameObject()->GetTransform()->GetChild(0)->GetChild(rightObjectIndex)->GetGameObject()->GetCollider();
	std::static_pointer_cast<SphereCollider>(rightBC)->SetCenter(rightColliderCenter);
}

void Player::Change9S2BMode()
{
	_9SMode = !_9SMode;
	std::wstring findName = (_9SMode == true ? L"2B" : L"9S");
	for (int i = 0; i < GetGameObject()->GetTransform()->GetChild(0)->GetChildCount(); ++i)
	{
		std::shared_ptr<TransformComponent> childTransform = GetGameObject()->GetTransform()->GetChild(0)->GetChild(i);
		if (size_t pos = childTransform->GetGameObject()->GetName().find(findName); pos != std::wstring::npos)
		{
			GetGameObject()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject()->SetActive(false);
		}
		else
		{
			GetGameObject()->GetTransform()->GetChild(0)->GetChild(i)->GetGameObject()->SetActive(true);
		}
	}
}

int Player::ChangeFlightCombatMode()
{
	_flightMode = !_flightMode;
	if (_flightMode == true)
		return COMBAT_TO_FLIGHT;
	else
		return FLIGHT_TO_COMBAT;
}
