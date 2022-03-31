#include "pch.h"
#include "TerrainScript.h"
#include "GameObject.h"
#include "TransformComponent.h"

TerrainScript::~TerrainScript()
{
}

void TerrainScript::LateUpdate()
{
}

void TerrainScript::Picked(Vec4 rayOrigin, Vec4 rayDir, float distance)
{
	Vec4 position = rayOrigin + rayDir * distance;
	_testObject->GetTransform()->SetWorldPosition(Vec3(position.x, position.y, position.z));
}
