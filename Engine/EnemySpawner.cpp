#include "pch.h"
#include "EnemySpawner.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ObjectPool.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "Input.h"

EnemySpawner::EnemySpawner()
{
	MONOCLASSNAME(EnemySpawner);
}

void EnemySpawner::Spawn()
{
	if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("Enemy"); poolObj != nullptr)
	{
		std::shared_ptr<Enemy> enemy = poolObj->GetComponent<Enemy>();

		enemy->Spawn(1, 50.f, 3);

		// 적의 Movement
		std::shared_ptr<GameObject> spawnPoints = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EnemySpawnPoints");
		// 몇 번인지 알려주기
		int num = 0; // 인자로 받기
		std::shared_ptr<GameObject> spawnPoint = spawnPoints->GetTransform()->GetChild(0)->GetGameObject();
		// lerp
		if (spawnPoint->GetTransform()->GetChildCount() == 2)
		{
			// 적의 Movement 세팅
			enemy->SetEnemyMovementType(EnemyMovmentType::LERP, 
				{ 
					spawnPoint->GetTransform()->GetChild(0)->GetWorldPosition(), 
					spawnPoint->GetTransform()->GetChild(1)->GetWorldPosition() 
				});
		}
		// catmullrom
		else
		{
			// 적의 Movement 세팅
			enemy->SetEnemyMovementType(EnemyMovmentType::CATMULLROM,
				{
					spawnPoint->GetTransform()->GetChild(0)->GetWorldPosition(),
					spawnPoint->GetTransform()->GetChild(1)->GetWorldPosition(),
					spawnPoint->GetTransform()->GetChild(2)->GetWorldPosition(),
					spawnPoint->GetTransform()->GetChild(3)->GetWorldPosition()
				});
		}

		// 적의 총 쏘는 방식

	}
	else
	{
		std::shared_ptr<GameObject> object = nullptr;
		// ImGui에서 Prefab 불러오는것처럼 불러오기
		std::shared_ptr<Scene> enemyPrefabScene = GET_SINGLE(Resources)->LoadPrefab("..\\Resources\\FBX\\em0030.fbx_Prefab");
		for (auto& iter : enemyPrefabScene->GetGameObjects())
		{
			if (iter->GetName().find(L"mesh_root") != std::wstring::npos)
			{
				object = iter->GetTransform()->GetParent().lock()->GetGameObject();
				break;
			}
		}

		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		object->AddComponent(enemy);

		enemy->Spawn(1, 50.f, 3);

		// 적의 Movement
		std::shared_ptr<GameObject> spawnPoints = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EnemySpawnPoint");
		// 몇 번인지 알려주기
		int num = 0; // 인자로 받기
		std::shared_ptr<GameObject> spawnPoint = spawnPoints->GetTransform()->GetChild(0)->GetGameObject();
		// lerp
		if (spawnPoint->GetTransform()->GetChildCount() == 2)
		{
			// 적의 Movement 세팅
			enemy->SetEnemyMovementType(EnemyMovmentType::LERP,
				{
					spawnPoint->GetTransform()->GetChild(0)->GetWorldPosition(),
					spawnPoint->GetTransform()->GetChild(1)->GetWorldPosition()
				});
		}
		// catmullrom
		else
		{
			// 적의 Movement 세팅
			enemy->SetEnemyMovementType(EnemyMovmentType::CATMULLROM,
				{
					spawnPoint->GetTransform()->GetChild(0)->GetWorldPosition(),
					spawnPoint->GetTransform()->GetChild(1)->GetWorldPosition(),
					spawnPoint->GetTransform()->GetChild(2)->GetWorldPosition(),
					spawnPoint->GetTransform()->GetChild(3)->GetWorldPosition()
				});
		}

		// 적의 총 쏘는 방식

		// SphereCollider
		std::shared_ptr<SphereCollider> sc = std::make_shared<SphereCollider>();
		object->AddComponent(sc);
		sc->SetRadius(object->GetTransform()->GetWorldScale().x);

		object->GetTransform()->SetParent(GetTransform());

		GET_SINGLE(ObjectPool)->AddPoolObject("Enemy", object);

		GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY, object);

		std::shared_ptr<Scene> currentScene = GET_SINGLE(SceneManager)->GetActiveScene();
		for (auto& iter : enemyPrefabScene->GetGameObjects())
		{
			currentScene->AddGameObject(iter);
		}
	}
}

void EnemySpawner::LateUpdate()
{
	if (INPUT->GetButtonDown(KEY_TYPE::Z))
	{
		Spawn();
	}
}

void EnemySpawner::OnCollisionEnter(std::shared_ptr<class BaseCollider> collider)
{
}

void EnemySpawner::OnCollisionStay(std::shared_ptr<class BaseCollider> collider)
{
}

void EnemySpawner::OnCollisionExit(std::shared_ptr<class BaseCollider> collider)
{
}
