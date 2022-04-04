#include "pch.h"
#include "TerrainScript.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"

TerrainScript::~TerrainScript()
{
}

void TerrainScript::LateUpdate()
{
}

void TerrainScript::Picked(Vec4 rayOrigin, Vec4 rayDir, float distance)
{
	Vec4 position = rayOrigin + rayDir * distance;
	std::shared_ptr<GameObject> obj = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(_testObject);
	if (obj != nullptr)
		obj->GetTransform()->SetWorldPosition(Vec3(position.x, position.y, position.z));
}
#ifdef TOOL
void TerrainScript::DragAndDrop(size_t hash)
{
	_testObject = hash;
}
#endif