#include "pch.h"
#include "EnemyShooting.h"
#include "Timer.h"
#include "ObjectPool.h"
#include "EnemyBullet.h"
#include "SceneManager.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Resources.h"
#include "Scene.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "RaycastManager.h"
#include "LaserScript.h"
#include "TransformComponent.h"

void EnemyLaserShooting::Shoot()
{
	std::shared_ptr<GameObject> player = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"pl0010.fbx0");
	Vec3 dir = _object.lock()->GetTransform()->GetWorldPosition() - player->GetTransform()->GetWorldPosition();
	_object.lock()->GetTransform()->SetLookAtWorldRotation(dir);

	_deltaTime += DELTA_TIME;
	if (_deltaTime > 0)
	{
		if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("EnemyLaser"); poolObj != nullptr)
		{
			std::shared_ptr<LaserScript> laser = poolObj->GetComponent<LaserScript>();
			poolObj->GetTransform()->SetWorldScale(Vec3(2, 2, 100));

			laser->Spawn(1, 1.f, 1);

			Vec3 worldPosition = _object.lock()->GetTransform()->GetWorldPosition();

			laser->GetTransform()->SetWorldPosition(worldPosition +
				(_object.lock()->GetTransform()->GetWorldTransform()->GetLook() * (-0.683f * laser->GetTransform()->GetWorldScale().z)));
			
			laser->_attachedObject = _object.lock();
		}
		else
		{
			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
			object->AddComponent(std::make_shared<TransformComponent>());
			object->GetTransform()->SetWorldScale(Vec3(2, 2, 100));
			object->SetCheckFrustum(false);

			std::shared_ptr<GameObject> bulletPrefab = GET_SINGLE(Resources)->Get<GameObject>(L"bullet.fbx0");
			std::shared_ptr<MeshRenderer> mr = bulletPrefab->GetMeshRenderer()->Clone();
			object->AddComponent(mr);
			mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Laser")->Clone());

			std::shared_ptr<LaserScript> laser = std::make_shared<LaserScript>();
			object->AddComponent(laser);

			laser->Spawn(1, 1.f, 1);

			Vec3 worldPosition = _object.lock()->GetTransform()->GetWorldPosition();
			
			laser->GetTransform()->SetWorldPosition(worldPosition +
				(_object.lock()->GetTransform()->GetWorldTransform()->GetLook() * (-0.683f * laser->GetTransform()->GetWorldScale().z)));

			laser->_attachedObject = _object.lock();

			std::shared_ptr<BoxCollider> bc = std::make_shared<BoxCollider>();
			object->AddComponent(bc);
			bc->SetExtent(object->GetTransform()->GetWorldScale());

			std::shared_ptr<GameObject> laserParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"LaserParent");

			object->SetName(L"LaserChild" + std::to_wstring(laserParent->GetTransform()->GetChildCount()));
			object->GenerateHash();

			object->GetTransform()->SetParent(laserParent->GetTransform());

			GET_SINGLE(ObjectPool)->AddPoolObject("EnemyLaser", object);

			GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY_LASER, object);

			GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
		}
		_deltaTime = -999999.f;
	}
}

void EnemyRapidShooting::Shoot()
{
	std::shared_ptr<GameObject> player = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"pl0010.fbx0");
	Vec3 dir = _object.lock()->GetTransform()->GetWorldPosition() - player->GetTransform()->GetWorldPosition();
	_object.lock()->GetTransform()->SetLookAtWorldRotation(dir);

	_deltaTime += DELTA_TIME;
	if (_deltaTime > _fireSpeed)
	{
		if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("EnemyBullet"); poolObj != nullptr)
		{
			std::shared_ptr<EnemyBullet> eb = poolObj->GetComponent<EnemyBullet>();
			std::shared_ptr<MeshRenderer> mr = eb->GetMeshRenderer();

			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_real_distribution<float> dist(0, 3);

			int randNum = dist(generator);
			if (randNum != 0)
			{
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_DESTROYABLE"));
				eb->SetEnemyBulletState(EnemyBulletState::DESTROYABLE);
			}
			else
			{
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_NON_DESTROYABLE"));
				eb->SetEnemyBulletState(EnemyBulletState::NON_DESTROYABLE);
			}

			eb->Spawn(1, 10.f, 1);

			eb->GetTransform()->SetWorldPosition(_object.lock()->GetTransform()->GetWorldPosition());
			eb->_direction = _object.lock()->GetTransform()->GetWorldTransform()->GetLook();
		}
		else
		{
			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
			object->AddComponent(std::make_shared<TransformComponent>());

			std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
			object->AddComponent(mr);
			mr->SetMesh(GET_SINGLE(Resources)->LoadSphereMesh());

			std::shared_ptr<EnemyBullet> eb = std::make_shared<EnemyBullet>();
			object->AddComponent(eb);

			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_real_distribution<float> dist(0, 3);

			int randNum = dist(generator);
			if (randNum != 0)
			{
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_DESTROYABLE"));
				eb->SetEnemyBulletState(EnemyBulletState::DESTROYABLE);
			}
			else
			{
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_NON_DESTROYABLE"));
				eb->SetEnemyBulletState(EnemyBulletState::NON_DESTROYABLE);
			}

			eb->Spawn(1, 10.f, 1);

			eb->_direction = _object.lock()->GetTransform()->GetWorldTransform()->GetLook();
			eb->GetTransform()->SetWorldPosition(_object.lock()->GetTransform()->GetWorldPosition());

			std::shared_ptr<SphereCollider> sc = std::make_shared<SphereCollider>();
			object->AddComponent(sc);
			sc->SetRadius(object->GetTransform()->GetWorldScale().x);

			std::shared_ptr<GameObject> bulletParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EnemyBulletParent");

			object->SetName(L"EnemyBulletChild" + std::to_wstring(bulletParent->GetTransform()->GetChildCount()));
			object->GenerateHash();

			object->GetTransform()->SetParent(bulletParent->GetTransform());

			GET_SINGLE(ObjectPool)->AddPoolObject("EnemyBullet", object);

			GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY_BULLET, object);

			GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
		}
		_deltaTime = 0.f;
	}
}

void EnemyHeavyShooting::Shoot()
{
	std::shared_ptr<GameObject> player = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"pl0010.fbx0");
	Vec3 dir = _object.lock()->GetTransform()->GetWorldPosition() - player->GetTransform()->GetWorldPosition();
	_object.lock()->GetTransform()->SetLookAtWorldRotation(dir);

	_deltaTime += DELTA_TIME;
	if (_deltaTime > _fireSpeed)
	{
		if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("EnemyBullet"); poolObj != nullptr)
		{
			std::shared_ptr<EnemyBullet> eb = poolObj->GetComponent<EnemyBullet>();
			std::shared_ptr<MeshRenderer> mr = eb->GetMeshRenderer();

			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_real_distribution<float> dist(0, 2);

			int randNum = dist(generator);
			if (randNum != 0)
			{
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_DESTROYABLE"));
				eb->SetEnemyBulletState(EnemyBulletState::DESTROYABLE);
			}
			else
			{
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_NON_DESTROYABLE"));
				eb->SetEnemyBulletState(EnemyBulletState::NON_DESTROYABLE);
			}

			eb->Spawn(1, 10.f, 1);

			eb->GetTransform()->SetWorldPosition(_object.lock()->GetTransform()->GetWorldPosition());
			eb->_direction = _object.lock()->GetTransform()->GetWorldTransform()->GetLook();
		}
		else
		{
			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
			object->AddComponent(std::make_shared<TransformComponent>());

			std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
			object->AddComponent(mr);
			mr->SetMesh(GET_SINGLE(Resources)->LoadSphereMesh());

			std::shared_ptr<EnemyBullet> eb = std::make_shared<EnemyBullet>();
			object->AddComponent(eb);

			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_real_distribution<float> dist(0, 2);

			int randNum = dist(generator);
			if (randNum != 0)
			{
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_DESTROYABLE"));
				eb->SetEnemyBulletState(EnemyBulletState::DESTROYABLE);
			}
			else
			{
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_NON_DESTROYABLE"));
				eb->SetEnemyBulletState(EnemyBulletState::NON_DESTROYABLE);
			}


			eb->Spawn(1, 10.f, 1);

			eb->_direction = _object.lock()->GetTransform()->GetWorldTransform()->GetLook();
			eb->GetTransform()->SetWorldPosition(_object.lock()->GetTransform()->GetWorldPosition());

			std::shared_ptr<SphereCollider> sc = std::make_shared<SphereCollider>();
			object->AddComponent(sc);
			sc->SetRadius(object->GetTransform()->GetWorldScale().x);

			std::shared_ptr<GameObject> bulletParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EnemyBulletParent");

			object->SetName(L"EnemyBulletChild" + std::to_wstring(bulletParent->GetTransform()->GetChildCount()));
			object->GenerateHash();

			object->GetTransform()->SetParent(bulletParent->GetTransform());

			GET_SINGLE(ObjectPool)->AddPoolObject("EnemyBullet", object);

			GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY_BULLET, object);

			GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
		}
		_deltaTime = 0.f;
	}
}

void EnemyThreeWayShooting::Shoot()
{
	std::shared_ptr<GameObject> player = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"pl0010.fbx0");
	Vec3 dir = _object.lock()->GetTransform()->GetWorldPosition() - player->GetTransform()->GetWorldPosition();
	_object.lock()->GetTransform()->SetLookAtWorldRotation(dir);

	_deltaTime += DELTA_TIME;
	if (_deltaTime > _fireSpeed)
	{
		Vec3 bulletDirections[3];
		bulletDirections[0] = _object.lock()->GetTransform()->GetWorldTransform()->GetLook();
		Matrix rotMat1 = Matrix::CreateFromYawPitchRoll(DegreeToRadian(15), 0, 0);
		Matrix rotMat2 = Matrix::CreateFromYawPitchRoll(DegreeToRadian(-15), 0, 0);
		bulletDirections[1] = Vec3::TransformNormal(bulletDirections[0], rotMat1);
		bulletDirections[2] = Vec3::TransformNormal(bulletDirections[0], rotMat2);

		for (int i = 0; i < 3; ++i)
		{
			if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("EnemyBullet"); poolObj != nullptr)
			{
				std::shared_ptr<EnemyBullet> eb = poolObj->GetComponent<EnemyBullet>();
				std::shared_ptr<MeshRenderer> mr = eb->GetMeshRenderer();

				std::random_device rd;
				std::mt19937 generator(rd());
				std::uniform_real_distribution<float> dist(0, 3);

				int randNum = dist(generator);
				if (randNum != 0)
				{
					mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_DESTROYABLE"));
					eb->SetEnemyBulletState(EnemyBulletState::DESTROYABLE);
				}
				else
				{
					mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_NON_DESTROYABLE"));
					eb->SetEnemyBulletState(EnemyBulletState::NON_DESTROYABLE);
				}

				eb->Spawn(1, 10.f, 1);

				eb->GetTransform()->SetWorldPosition(_object.lock()->GetTransform()->GetWorldPosition());
				
				eb->_direction = bulletDirections[i];
			}
			else
			{
				std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
				object->AddComponent(std::make_shared<TransformComponent>());

				std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
				object->AddComponent(mr);
				mr->SetMesh(GET_SINGLE(Resources)->LoadSphereMesh());
				mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet"));

				std::shared_ptr<EnemyBullet> eb = std::make_shared<EnemyBullet>();
				object->AddComponent(eb);

				std::random_device rd;
				std::mt19937 generator(rd());
				std::uniform_real_distribution<float> dist(0, 3);

				int randNum = dist(generator);
				if (randNum != 0)
				{
					mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_DESTROYABLE"));
					eb->SetEnemyBulletState(EnemyBulletState::DESTROYABLE);
				}
				else
				{
					mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"EnemyBullet_NON_DESTROYABLE"));
					eb->SetEnemyBulletState(EnemyBulletState::NON_DESTROYABLE);
				}

				eb->Spawn(1, 10.f, 1);

				eb->_direction = bulletDirections[i];

				eb->GetTransform()->SetWorldPosition(_object.lock()->GetTransform()->GetWorldPosition());

				std::shared_ptr<SphereCollider> sc = std::make_shared<SphereCollider>();
				object->AddComponent(sc);
				sc->SetRadius(object->GetTransform()->GetWorldScale().x);

				std::shared_ptr<GameObject> bulletParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"EnemyBulletParent");

				object->SetName(L"EnemyBulletChild" + std::to_wstring(bulletParent->GetTransform()->GetChildCount()));
				object->GenerateHash();

				object->GetTransform()->SetParent(bulletParent->GetTransform());

				GET_SINGLE(ObjectPool)->AddPoolObject("EnemyBullet", object);

				GET_SINGLE(CollisionManager)->AddObject(CollisionObjectType::ENEMY_BULLET, object);

				GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
			}
		}
		_deltaTime = 0.f;
	}
}
