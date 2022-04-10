#include "pch.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "CubeCollider.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "BoneCollider.h"
#include "TransformComponent.h"
#include "MonoBehaviour.h"

void CollisionManager::AddObject(CollisionObjectType type, std::shared_ptr<GameObject> gameObject)
{
    _listObject[static_cast<int>(type)].push_back(gameObject);
}

void CollisionManager::Update()
{

}

void CollisionManager::CheckCollision(CollisionObjectType srcType, CollisionObjectType dstType)
{
	// srcType과 dstType 순회
	for (auto& srcObj : _listObject[static_cast<int>(srcType)])
	{
		if (srcObj->GetActive() == false)
			continue;
		for (auto& dstObj : _listObject[static_cast<int>(dstType)])
		{
			if (dstObj->GetActive() == false)
				continue;

			if (srcObj == dstObj)
				continue;
			
			// srcObj의 Collider Type 체크
			ColliderType srcColliderType = srcObj->GetCollider()->GetColliderType();
			// dstObj의 Collider Type 체크
			ColliderType dstColliderType = dstObj->GetCollider()->GetColliderType();
			// 체크한 Collider를 기준으로 충돌처리
			
			// 서로 Collider가 같다면
			if (srcColliderType == dstColliderType)
			{
				switch (srcColliderType)
				{
				case ColliderType::Sphere:
				{
					std::shared_ptr<SphereCollider> srcCollider = std::static_pointer_cast<SphereCollider>(srcObj->GetCollider());
					std::shared_ptr<SphereCollider> dstCollider = std::static_pointer_cast<SphereCollider>(dstObj->GetCollider());
				
					bool result = CheckCollisionSphere(srcCollider->GetTransform()->GetWorldPosition(), srcCollider->GetRadius(), dstCollider->GetTransform()->GetWorldPosition(), dstCollider->GetRadius());
					if (result == true)
					{
						// 충돌 했다면? srcObj dstObj Script를 모두 순회하며 OnCollision 호출.
						for (auto& iter : srcObj->_scripts)
						{
							iter->OnCollisionEnter(dstCollider);
						}
						for (auto& iter : dstObj->_scripts)
						{
							iter->OnCollisionEnter(srcCollider);
						}
					}
					break;
				}
				case ColliderType::Cube:
					break;
				case ColliderType::Mesh:
					break;
				case ColliderType::Bone:
				{
					std::shared_ptr<BoneCollider> srcCollider = std::static_pointer_cast<BoneCollider>(srcObj->GetCollider());
					std::shared_ptr<BoneCollider> dstCollider = std::static_pointer_cast<BoneCollider>(dstObj->GetCollider());
					// srcCollider의 Collider 개수 * dstCollider의 Collider 개수만큼 순회하며 Sphere 충돌체크를 한다.
					for (int i = 0; i < srcCollider->GetBoneColliders().size(); ++i)
					{
						const BoneColliderInfo& srcBCInfo = srcCollider->GetBoneColliders()[i];

						for (int j = 0; j < dstCollider->GetBoneColliders().size(); ++j)
						{
							const BoneColliderInfo& dstBCInfo = dstCollider->GetBoneColliders()[i];

							bool result = CheckCollisionSphere(srcBCInfo.sphere.Center, srcBCInfo.sphere.Radius, dstBCInfo.sphere.Center, dstBCInfo.sphere.Radius);
							if (result == true)
							{
								// 충돌 했다면? srcObj dstObj Script를 모두 순회하며 OnCollision 호출.
								for (auto& iter : srcObj->_scripts)
								{
									iter->OnCollisionEnter(dstCollider);
								}
								for (auto& iter : dstObj->_scripts)
								{
									iter->OnCollisionEnter(srcCollider);
								}
							}
						}
					}
					break;
				}
				default:
					break;
				}
			}

			else if ((srcColliderType == ColliderType::Sphere && dstColliderType == ColliderType::Bone)
					|| (srcColliderType == ColliderType::Bone && dstColliderType == ColliderType::Sphere))
			{
				std::shared_ptr<SphereCollider> sphereCollider = nullptr;
				std::shared_ptr<BoneCollider> boneCollider = nullptr;
				if (srcColliderType == ColliderType::Sphere)
				{
					sphereCollider = std::static_pointer_cast<SphereCollider>(srcObj->GetCollider());
					boneCollider = std::static_pointer_cast<BoneCollider>(dstObj->GetCollider());
				}
				else if (srcColliderType == ColliderType::Bone)
				{
					sphereCollider = std::static_pointer_cast<SphereCollider>(dstObj->GetCollider());
					boneCollider = std::static_pointer_cast<BoneCollider>(srcObj->GetCollider());
				}

				for (int i = 0; i < boneCollider->GetBoneColliders().size(); ++i)
				{
					const BoneColliderInfo& boneColliderInfo = boneCollider->GetBoneColliders()[i];

					bool result = CheckCollisionSphere(boneColliderInfo.sphere.Center, boneColliderInfo.sphere.Radius, 
						sphereCollider->GetTransform()->GetWorldPosition(), sphereCollider->GetRadius());
					if (result == true)
					{
						// 충돌 했다면? srcObj dstObj Script를 모두 순회하며 OnCollision 호출.
						if (srcColliderType == ColliderType::Sphere)
						{
							for (auto& iter : srcObj->_scripts)
							{
								iter->OnCollisionEnter(boneCollider);
							}
							for (auto& iter : dstObj->_scripts)
							{
								iter->OnCollisionEnter(sphereCollider);
							}
						}
						else if (srcColliderType == ColliderType::Bone)
						{
							for (auto& iter : srcObj->_scripts)
							{
								iter->OnCollisionEnter(sphereCollider);
							}
							for (auto& iter : dstObj->_scripts)
							{
								iter->OnCollisionEnter(boneCollider);
							}
						}
						
					}
				}
			}
		}
	}
}

void CollisionManager::DeleteAllObject()
{
	for (int i = 0; i < static_cast<int>(CollisionObjectType::END); ++i)
		_listObject[i].clear();
}

void CollisionManager::DeleteObject(CollisionObjectType eType)
{
	_listObject[static_cast<int>(eType)].clear();
}

bool CollisionManager::CheckCollisionSphere(const Vec3& srcPos, float srcRadius, const Vec3& dstPos, float dstRadius)
{
	Vec3 dir = (srcPos - dstPos);
	float length = dir.Length();

	return length <= (srcRadius + dstRadius);
}

bool CollisionManager::CheckCollisionBox(const Vec3& srcPos, float srcRadius, const Vec3& dstPos, float dstRadius)
{
    return false;
}

bool CollisionManager::CheckCollisionSphereBox(const Vec3& srcPos, float srcRadius, const Vec3& dstPos, float dstRadius)
{
    return false;
}

bool CollisionManager::CheckCollisionSpherePoint(const Vec3& srcPos, float srcRadius, const Vec3& dstPos, float dstRadius)
{
    return false;
}
