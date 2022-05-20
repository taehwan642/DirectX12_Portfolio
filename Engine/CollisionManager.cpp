#include "pch.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "BoxCollider.h"
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
				
					bool result = CheckCollisionSphere(srcCollider->_boundingSphere, dstCollider->_boundingSphere);
					if (result == true)
					{
						CollisionOutput srcObjOutput;
						srcObjOutput.type = srcType;
						srcObjOutput.collider = srcCollider;

						CollisionOutput dstObjOutput;
						dstObjOutput.type = dstType;
						dstObjOutput.collider = dstCollider;

						// 충돌 했다면? srcObj dstObj Script를 모두 순회하며 OnCollision 호출.
						for (auto& iter : srcObj->_scripts)
							iter->OnCollisionEnter(dstObjOutput);
						for (auto& iter : dstObj->_scripts)
							iter->OnCollisionEnter(srcObjOutput);
					}
					break;
				}
				case ColliderType::Box:
				{
					std::shared_ptr<BoxCollider> srcCollider = std::static_pointer_cast<BoxCollider>(srcObj->GetCollider());
					std::shared_ptr<BoxCollider> dstCollider = std::static_pointer_cast<BoxCollider>(dstObj->GetCollider());

					bool result = CheckCollisionBox(srcCollider->_boundingBox, dstCollider->_boundingBox);
					if (result == true)
					{
						CollisionOutput srcObjOutput;
						srcObjOutput.type = srcType;
						srcObjOutput.collider = srcCollider;

						CollisionOutput dstObjOutput;
						dstObjOutput.type = dstType;
						dstObjOutput.collider = dstCollider;

						// 충돌 했다면? srcObj dstObj Script를 모두 순회하며 OnCollision 호출.
						for (auto& iter : srcObj->_scripts)
							iter->OnCollisionEnter(dstObjOutput);
						for (auto& iter : dstObj->_scripts)
							iter->OnCollisionEnter(srcObjOutput);
					}
				}
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

							bool result = CheckCollisionSphere(srcBCInfo.sphere, dstBCInfo.sphere);
							if (result == true)
							{
								CollisionOutput srcObjOutput;
								srcObjOutput.type = srcType;
								srcObjOutput.collider = srcCollider;

								CollisionOutput dstObjOutput;
								dstObjOutput.type = dstType;
								dstObjOutput.collider = dstCollider;

								// 충돌 했다면? srcObj dstObj Script를 모두 순회하며 OnCollision 호출.
								for (auto& iter : srcObj->_scripts)
									iter->OnCollisionEnter(dstObjOutput);
								for (auto& iter : dstObj->_scripts)
									iter->OnCollisionEnter(srcObjOutput);
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

					bool result = CheckCollisionSphere(boneColliderInfo.sphere, sphereCollider->_boundingSphere);
					if (result == true)
					{
						// 충돌 했다면? srcObj dstObj Script를 모두 순회하며 OnCollision 호출.
						if (srcColliderType == ColliderType::Sphere)
						{
							CollisionOutput srcObjOutput;
							srcObjOutput.type = srcType;
							srcObjOutput.collider = sphereCollider;

							CollisionOutput dstObjOutput;
							dstObjOutput.type = dstType;
							dstObjOutput.collider = boneCollider;

							for (auto& iter : srcObj->_scripts)
								iter->OnCollisionEnter(dstObjOutput);
							for (auto& iter : dstObj->_scripts)
								iter->OnCollisionEnter(srcObjOutput);
						}
						else if (srcColliderType == ColliderType::Bone)
						{
							CollisionOutput srcObjOutput;
							srcObjOutput.type = srcType;
							srcObjOutput.collider = boneCollider;

							CollisionOutput dstObjOutput;
							dstObjOutput.type = dstType;
							dstObjOutput.collider = sphereCollider;

							for (auto& iter : srcObj->_scripts)
								iter->OnCollisionEnter(dstObjOutput);
							for (auto& iter : dstObj->_scripts)
								iter->OnCollisionEnter(srcObjOutput);
						}
						
					}
				}
			}

			else if ((srcColliderType == ColliderType::Sphere && dstColliderType == ColliderType::Box)
					|| (srcColliderType == ColliderType::Box && dstColliderType == ColliderType::Sphere))
			{
				std::shared_ptr<SphereCollider> sphereCollider = nullptr;
				std::shared_ptr<BoxCollider> boxCollider = nullptr;
				if (srcColliderType == ColliderType::Sphere)
				{
					sphereCollider = std::static_pointer_cast<SphereCollider>(srcObj->GetCollider());
					boxCollider = std::static_pointer_cast<BoxCollider>(dstObj->GetCollider());
				}
				else if (srcColliderType == ColliderType::Box)
				{
					sphereCollider = std::static_pointer_cast<SphereCollider>(dstObj->GetCollider());
					boxCollider = std::static_pointer_cast<BoxCollider>(srcObj->GetCollider());
				}

				bool result = CheckCollisionSphereBox(sphereCollider->_boundingSphere, boxCollider->_boundingBox);
				if (result == true)
				{
					// 충돌 했다면? srcObj dstObj Script를 모두 순회하며 OnCollision 호출.
					if (srcColliderType == ColliderType::Sphere)
					{
						CollisionOutput srcObjOutput;
						srcObjOutput.type = srcType;
						srcObjOutput.collider = sphereCollider;

						CollisionOutput dstObjOutput;
						dstObjOutput.type = dstType;
						dstObjOutput.collider = boxCollider;

						for (auto& iter : srcObj->_scripts)
							iter->OnCollisionEnter(dstObjOutput);
						for (auto& iter : dstObj->_scripts)
							iter->OnCollisionEnter(srcObjOutput);
					}
					else if (srcColliderType == ColliderType::Box)
					{
						CollisionOutput srcObjOutput;
						srcObjOutput.type = srcType;
						srcObjOutput.collider = boxCollider;

						CollisionOutput dstObjOutput;
						dstObjOutput.type = dstType;
						dstObjOutput.collider = sphereCollider;

						for (auto& iter : srcObj->_scripts)
							iter->OnCollisionEnter(dstObjOutput);
						for (auto& iter : dstObj->_scripts)
							iter->OnCollisionEnter(srcObjOutput);
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

bool CollisionManager::CheckCollisionSphere(const BoundingSphere& srcCollider, const BoundingSphere& dstCollider)
{
	return srcCollider.Intersects(dstCollider);
}

bool CollisionManager::CheckCollisionBox(const BoundingBox& srcCollider, const BoundingBox& dstCollider)
{
	// AABB
	return srcCollider.Intersects(dstCollider);
}

bool CollisionManager::CheckCollisionSphereBox(const BoundingSphere& srcCollider, const BoundingBox& dstCollider)
{
	Vec3 pointOnRect;
	pointOnRect.x = std::clamp(srcCollider.Center.x, (dstCollider.Center.x - dstCollider.Extents.x / 2.f), (dstCollider.Center.x + dstCollider.Extents.x / 2.f));
	pointOnRect.y = std::clamp(srcCollider.Center.y, (dstCollider.Center.y - dstCollider.Extents.y / 2.f), (dstCollider.Center.y + dstCollider.Extents.y / 2.f));
	pointOnRect.z = std::clamp(srcCollider.Center.z, (dstCollider.Center.z - dstCollider.Extents.z / 2.f), (dstCollider.Center.z + dstCollider.Extents.z / 2.f));

	Vec3 circleToRectPoint = pointOnRect - srcCollider.Center;

	return circleToRectPoint.LengthSquared() < srcCollider.Radius * srcCollider.Radius;
}