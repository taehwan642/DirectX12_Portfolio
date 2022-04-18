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

	static float t = 0;
	t += DELTA_TIME;
	static int num = 0;

	if (t > 1.f)
	{
		t = 0.f;
		num += 1;
		if ((num + 3) >= pathParent->GetTransform()->GetChildCount())
		{
			num = 0;
		}
	}

	Vec3 child0Pos = pathParent->GetTransform()->GetChild(0 + num)->GetWorldPosition();
	Vec3 child1Pos = pathParent->GetTransform()->GetChild(1 + num)->GetWorldPosition();
	Vec3 child2Pos = pathParent->GetTransform()->GetChild(2 + num)->GetWorldPosition();
	Vec3 child3Pos = pathParent->GetTransform()->GetChild(3 + num)->GetWorldPosition();

	Vec3::CatmullRom(child0Pos, child1Pos, child2Pos, child3Pos, t, result);

	lockObject->GetTransform()->SetWorldPosition(result);
}

#ifdef TOOL
void PathTraceCameraScript::DragAndDrop(size_t hash)
{
	_objects.push_back(hash);
}
#endif