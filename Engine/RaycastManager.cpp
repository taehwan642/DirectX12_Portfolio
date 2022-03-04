#include "pch.h"
#include "RaycastManager.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Engine.h"
#include "Scene.h"
#include "GameObject.h"
#include "SphereCollider.h"

std::shared_ptr<GameObject> RaycastManager::Pick(int32 screenX, int32 screenY)
{
	std::shared_ptr<Camera> camera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix projectionMatrix = camera->GetProjectionMatrix();

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * screenX / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / projectionMatrix(1, 1);

	Matrix viewMatrix = camera->GetViewMatrix();
	Matrix viewMatrixInv = viewMatrix.Invert();

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	float minDistance = FLT_MAX;
	std::shared_ptr<GameObject> picked;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCollider() == nullptr)
			continue;

		// ViewSpace에서의 Ray 정의
		Vec4 tempRayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 tempRayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

		// WorldSpace에서의 Ray 정의
		tempRayOrigin = XMVector3TransformCoord(tempRayOrigin, viewMatrixInv);
		tempRayDir = XMVector3TransformNormal(tempRayDir, viewMatrixInv);
		tempRayDir.Normalize();

		// WorldSpace에서 연산
		float tempDistance = 0.f;
		if (gameObject->GetCollider()->Intersects(tempRayOrigin, tempRayDir, OUT tempDistance) == false)
			continue;

		if (tempDistance < minDistance)
		{
			minDistance = tempDistance;
			picked = gameObject;
			rayOrigin = tempRayOrigin;
			rayDir = tempRayDir;
			distance = tempDistance;
		}
	}

	return picked;
}
