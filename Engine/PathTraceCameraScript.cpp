#include "pch.h"
#include "PathTraceCameraScript.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Timer.h"

PathTraceCameraScript::~PathTraceCameraScript()
{
}

void PathTraceCameraScript::LateUpdate()
{
	if (_objects.size() < 2)
		return;
	// 카메라와 고정된 object
	std::shared_ptr<GameObject> lockObject = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(_objects[0]);
	std::shared_ptr<GameObject> pathParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(_objects[1]);
	Vec3 position = GetTransform()->GetWorldPosition();
	Vec3 lockObjectPos = lockObject->GetTransform()->GetWorldPosition();
	Vec3 result = Vec3::Zero;

	// LockObject의 z를 기준으로, n번째 path와 n + 1 path사이를 CatMull-Rom 보간해 position에 넣는다.
	// 하지만 지금은 Camera를 움직임으로써 LockObject를 보간한다 (원하는 목표의 반대로 움직이는 것.)

	// 현재 위치가 어느 Area에 있는지 확인
	// Path와 Path 사이를 Area라고 부르기로 했다.
	// 이 때 area는? Area 0이라면 Child 1과 Child 2 사이를 뜻한다.
	int area = FindCurrentArea();

	// t 구하기
	Vec3 path1Pos = pathParent->GetTransform()->GetChild(1 + area)->GetWorldPosition();
	Vec3 path2Pos = pathParent->GetTransform()->GetChild(2 + area)->GetWorldPosition();
	float dist = (((path2Pos.z - path1Pos.z) - (path2Pos.z - position.z)) / (path2Pos.z - path1Pos.z));
	float time = max(0, dist);

	// Catmull-Rom 보간

	Vec3 child0Pos = pathParent->GetTransform()->GetChild(0 + area)->GetWorldPosition();
	Vec3 child1Pos = pathParent->GetTransform()->GetChild(1 + area)->GetWorldPosition();
	Vec3 child2Pos = pathParent->GetTransform()->GetChild(2 + area)->GetWorldPosition();
	Vec3 child3Pos = pathParent->GetTransform()->GetChild(3 + area)->GetWorldPosition();

	Vec3::CatmullRom(child0Pos, child1Pos, child2Pos, child3Pos, time, result);

	lockObject->GetTransform()->SetWorldPosition(result);
}

#ifdef TOOL
void PathTraceCameraScript::DragAndDrop(size_t hash)
{
	_objects.push_back(hash);
}
#endif

int PathTraceCameraScript::FindCurrentArea()
{
	// path 1의 z보다 크지만 path 2의 z보다 작다면? area 0에 있다는 뜻.
	Vec3 position = GetTransform()->GetWorldPosition();
	std::shared_ptr<GameObject> pathParent = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(_objects[1]);
	float posZ = position.z;
	int area = 0;
	int areaSize = pathParent->GetTransform()->GetChildCount() - 3;
	Vec3 path2Pos = pathParent->GetTransform()->GetChild(1 + areaSize)->GetWorldPosition();
	// 가장 끝 영역을 벗어났다면
	if (posZ >= path2Pos.z)
	{
		area = areaSize - 1;
		return area;
	}
	// 가장 처음 영역을 벗어났다면
	else if (path2Pos = pathParent->GetTransform()->GetChild(2)->GetWorldPosition(); posZ <= path2Pos.z)
	{
		area = 0;
		return area;
	}
	// 영역들 내에 존재. 어느 영역에 있는지 확인
	else
	{
		bool found = false;
		Vec3 path1Pos = pathParent->GetTransform()->GetChild(1)->GetWorldPosition();
		path2Pos = pathParent->GetTransform()->GetChild(2)->GetWorldPosition();
		while (found == false)
		{
			if (path1Pos.z < position.z && path2Pos.z > position.z)
			{
				found = true;
				break;
			}
			++area;
			path1Pos = pathParent->GetTransform()->GetChild(1 + area)->GetWorldPosition();
			path2Pos = pathParent->GetTransform()->GetChild(2 + area)->GetWorldPosition();
		}
	}

	return area;
}