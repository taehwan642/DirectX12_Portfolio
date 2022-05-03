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

Player::Player()
{
	MONOCLASSNAME(Player);
	_speed = 50.f;
	_hp = 1.f;
	_damage = 0.f;
	_stateManager = std::make_shared<StateManager>();
}

Player::~Player()
{
}

void Player::OnCollisionEnter(std::shared_ptr<class BaseCollider> collider)
{
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
}

void Player::LateUpdate()
{
	_stateManager->UpdateState();
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
		GetGameObject()->GetAudioSource()->SetVolume(1.f);
		GetGameObject()->GetAudioSource()->Play();
		_deltaTime = 0.f;
		if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("Bullet"); poolObj != nullptr)
		{
			poolObj->GetComponent<PlayerBullet>()->Spawn();
		}
		else
		{
			// bone 006
			// bone 010
			Vec3 pos1 = GetGameObject()->GetTransform()->GetChild(0)->GetChild(static_cast<int>(_9SMode))->GetAnimator()->GetBonePosition("bone006");
			Vec3 pos2 = GetGameObject()->GetTransform()->GetChild(0)->GetChild(static_cast<int>(_9SMode))->GetAnimator()->GetBonePosition("bone010");

			for (int i = 0; i < 2; ++i)
			{
				std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
				std::shared_ptr<GameObject> bulletPrefab = GET_SINGLE(Resources)->Get<GameObject>(L"bullet.fbx0");
				object->AddComponent(std::make_shared<TransformComponent>());

				if(i == 0)
					object->GetTransform()->SetWorldPosition(pos1);
				else
					object->GetTransform()->SetWorldPosition(pos2);

				std::shared_ptr<MeshRenderer> mr = bulletPrefab->GetMeshRenderer()->Clone();
				object->AddComponent(mr);

				std::shared_ptr<PlayerBullet> pb = std::make_shared<PlayerBullet>();
				object->AddComponent(pb);
				pb->Spawn();

				std::shared_ptr<GameObject> bulletParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"BulletParent");

				object->SetName(L"BulletChild" + std::to_wstring(bulletParent->GetTransform()->GetChildCount()));
				object->GenerateHash();

				object->GetTransform()->SetParent(bulletParent->GetTransform());

				GET_SINGLE(ObjectPool)->AddPoolObject("Bullet", object);

				GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
			}
		}
	}
}

void Player::ChangeCurrentMode()
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
