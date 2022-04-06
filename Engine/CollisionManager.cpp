#include "pch.h"
#include "CollisionManager.h"

void CollisionManager::AddObject(CollisionObjectType type, std::shared_ptr<GameObject> gameObject)
{
    _listObject[static_cast<int>(type)].push_back(gameObject);
}

void CollisionManager::Update()
{
	//for (int i = 0; i < static_cast<int>(CollisionObjectType::END); ++i)
	//{
	//	auto iter = _listObject[i].begin();
	//	auto iter_End = _listObject[i].end();
	//
	//	for (; iter != iter_End; )
	//	{
	//		if (*iter == nullptr)
	//		{
	//			iter = _listObject[i].erase(iter);
	//		}
	//		else
	//			++iter;
	//	}
	//}
}

void CollisionManager::CheckCollision(CollisionObjectType srcType, CollisionObjectType dstType)
{
	//bool isColl = false;
	//Vec3 srcPos, dstPos;
	//float srcRadius = 0.f, dstRadius = 0.f, dist = 0.f;
	//
	//for (auto& pSrcObj : _listObject[static_cast<int>(srcType)])
	//{
	//	for (auto& pDstObj : _listObject[static_cast<int>(dstType)])
	//	{
	//		// 동일한 오브젝트일 경우
	//		if (pSrcObj == pDstObj)
	//			continue;
	//
	//		// 콜라이더 정보를 가지고 체크한다.
	//
	//		pmapSrcCollData = pSrcObj->Get_CollDataMap();
	//		pmapDstCollData = pDstObj->Get_CollDataMap();
	//
	//		for (auto& SrcPair : *pmapSrcCollData)
	//		{
	//			pSrcObj->Get_CollisionPos(SrcPair.second, &vSrcPos, &fSrcRadius);
	//
	//			for (auto& DstPair : *pmapDstCollData)
	//			{
	//				pDstObj->Get_CollisionPos(DstPair.second, &vDstPos, &fDstRadius);
	//
	//				// 충돌했을 경우
	//				if (bIsColl = Check_Collision_Sphere(vSrcPos, fSrcRadius, vDstPos, fDstRadius, &vSrcCollPos, &vDstCollPos, &fDist))
	//				{
	//					pSrcObj->Check_CollisionEvent(pDstObj->Get_ObjectName(), COLL_RESULT(vSrcCollPos, fSrcRadius, fDist, (vSrcPos - vSrcCollPos)), pDstObj);
	//					pDstObj->Check_CollisionEvent(pSrcObj->Get_ObjectName(), COLL_RESULT(vDstCollPos, fDstRadius, fDist, (vDstPos - vDstCollPos)), pSrcObj);
	//					DstPair.second->eIsColl = COL_TRUE;
	//					SrcPair.second->eIsColl = COL_TRUE;
	//					break;
	//				}
	//			}
	//			// for (auto& SrcPair : *pmapSrcCollData) 중료
	//			if (bIsColl)
	//			{
	//				bIsColl = false;
	//				break;
	//			}
	//		}
	//	}
	//}
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
	dir.Normalize();

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
