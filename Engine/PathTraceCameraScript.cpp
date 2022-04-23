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
	Vec3 resultPosition = Vec3::Zero;
	Vec3 resultRotation = Vec3::Zero;

	// LockObject의 z를 기준으로, n번째 path와 n + 1 path사이를 CatMull-Rom 보간해 position에 넣는다.
	// 하지만 지금은 Camera를 움직임으로써 LockObject를 보간한다 (원하는 목표의 반대로 움직이는 것.)

	// 현재 위치가 어느 Area에 있는지 확인
	// Path와 Path 사이를 Area라고 부르기로 했다.
	// 이 때 area는? Area 0이라면 Child 1과 Child 2 사이를 뜻한다.
	int area = FindCurrentArea();
	static float t = 0;

	// 현재 Area가 FOLLOW인지, NON-FOLLOW인지 구별
	// FOLLOW라면? 보간
	if (pathParent->GetTransform()->GetChild(1 + area)->GetChildCount() == 0)
	{
		t = 0;

		std::shared_ptr<TransformComponent> child1 = pathParent->GetTransform()->GetChild(1 + area);
		std::shared_ptr<TransformComponent> child2 = pathParent->GetTransform()->GetChild(2 + area);

		Vec3 child0Pos = pathParent->GetTransform()->GetChild(0 + area)->GetWorldPosition();
		Vec3 child1Pos = child1->GetWorldPosition();
		Vec3 child2Pos = child2->GetWorldPosition();
		Vec3 child3Pos = pathParent->GetTransform()->GetChild(3 + area)->GetWorldPosition();
		
		// t 구하기
		float dist;
		float time;
		
		// 그 Area를 벗어났다면? 고정 pos to b 해야 할텐데, 그건 어떻게?
		// 이전 Area가 고정 Area였다면, 
		if (pathParent->GetTransform()->GetChild(area)->GetChildCount() == 1)
		{
			Vec3 fixedPosition = pathParent->GetTransform()->GetChild(area)->GetChild(0)->GetWorldPosition();
			dist = (((child2Pos.z - fixedPosition.z) - (child2Pos.z - position.z)) / (child2Pos.z - fixedPosition.z));
			time = min(1, max(0, dist));

			// Catmull-Rom 보간
			Vec3::CatmullRom(child0Pos, fixedPosition, child2Pos, child3Pos, time, resultPosition);

			// Rotation 보간
			Vec3::Lerp(pathParent->GetTransform()->GetChild(area)->GetChild(0)->GetWorldRotation(), 
				child2->GetWorldRotation(), time, resultRotation);
		}
		else
		{
			dist = (((child2Pos.z - child1Pos.z) - (child2Pos.z - position.z)) / (child2Pos.z - child1Pos.z));
			time = min(1, max(0, dist));

			// Catmull-Rom 보간
			Vec3::CatmullRom(child0Pos, child1Pos, child2Pos, child3Pos, time, resultPosition);

			// Rotation 보간
			Vec3::Lerp(child1->GetWorldRotation(), child2->GetWorldRotation(), time, resultRotation);
		}
	}
	// NON-FOLLOW라면?
	else
	{
		// 그 Child가 바로 영역 속 고정될 위치이니, 그곳으로 Lerp.
		// a to 고정 pos
		Vec3 fixedPosition = pathParent->GetTransform()->GetChild(1 + area)->GetChild(0)->GetWorldPosition();
		if (t > 1.f)
			t = 1.f; 
		Vec3::Lerp(pathParent->GetTransform()->GetChild(1 + area)->GetWorldPosition(), fixedPosition, t, resultPosition);
		t += DELTA_TIME;

		// Rotation 보간
		Vec3::Lerp(pathParent->GetTransform()->GetChild(1 + area)->GetWorldRotation(), 
			pathParent->GetTransform()->GetChild(1 + area)->GetChild(0)->GetWorldRotation(), t, resultRotation);
	}

	// 만약 흔들리고 있다면?? 
	if (_shakeTime > 0.f)
	{
		_shakeTime -= DELTA_TIME;
		resultPosition += GetRandomVec3();
	}

	lockObject->GetTransform()->SetWorldPosition(resultPosition);
	lockObject->GetTransform()->SetWorldRotation(resultRotation);
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

void PathTraceCameraScript::ShakeCamera()
{
	_shakeTime = 0.5f;
}

const Vec3& PathTraceCameraScript::GetRandomVec3()
{
	// 현재 위치에서 셰이크
	float min = -1.f;
	float max = 1.f;

	Vec3 result;
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_real_distribution<float> dist(min, max);

	result = Vec3(dist(generator), dist(generator), dist(generator));

	return result;
}