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
		Vec3 localPos = obj->GetTransform()->GetLocalPosition();
		localPos.z -= 0.3f;
		if (localPos.z < -300.f)
		{
			localPos.z = 200.f;
		}
		obj->GetTransform()->SetLocalPosition(localPos);
	}
}
