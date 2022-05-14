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
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
		object->AddComponent(std::make_shared<TransformComponent>());
		object->GetTransform()->SetWorldScale(Vec3(2, 2, 10));
		object->SetCheckFrustum(false);

		// ImGui에서 Prefab 불러오는것처럼 불러오기
		//std::shared_ptr<GameObject> bulletPrefab = GET_SINGLE(Resources)->Get<GameObject>(L"bullet.fbx0");
		//std::shared_ptr<MeshRenderer> mr = bulletPrefab->GetMeshRenderer()->Clone();
		//object->AddComponent(mr);
		//mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Laser"));

		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		object->AddComponent(enemy);

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

		// SphereCollider
		// std::shared_ptr<BoxCollider> bc = std::make_shared<BoxCollider>();
		// object->AddComponent(bc);
		// bc->SetExtent(object->GetTransform()->GetWorldScale());

		std::shared_ptr<GameObject> enemyParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EnemyParent");

		object->SetName(L"EnemyChild" + std::to_wstring(enemyParent->GetTransform()->GetChildCount()));
		object->GenerateHash();

		object->GetTransform()->SetParent(enemyParent->GetTransform());

		GET_SINGLE(ObjectPool)->AddPoolObject("Enemy", object);

		GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY, object);

		GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
	}
	

}

void EnemySpawner::LateUpdate()
{

}
