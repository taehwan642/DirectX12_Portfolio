#include "pch.h"
#include "InfinitePassage.h"
#include "TransformComponent.h"
#include "GameObject.h"

InfinitePassage::~InfinitePassage()
{
}

void InfinitePassage::LateUpdate()
{
	// -z ∑Œ ¿Ãµø
	for (int i = 0; i < GetTransform()->GetChildCount(); ++i)
	{
		std::shared_ptr<GameObject> obj = GetTransform()->GetChild(i)->GetGameObject();
		Vec3 worldPos = obj->GetTransform()->GetWorldPosition();
		worldPos.z -= 0.3f;
		if (worldPos.z < -300.f)
		{
			worldPos.z = 200.f;
		}
		obj->GetTransform()->SetWorldPosition(worldPos);
	}
}
