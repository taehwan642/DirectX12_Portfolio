#include "pch.h"
#include "TerrainScript.h"
#include "GameObject.h"
#include "Transform.h"

TerrainScript::TerrainScript()
{
	_className = s2ws(typeid(decltype(this)).name());
}

TerrainScript::~TerrainScript()
{
}

void TerrainScript::LateUpdate()
{
}

void TerrainScript::Picked(Vec4 rayOrigin, Vec4 rayDir, float distance)
{
	Vec4 position = rayOrigin + rayDir * distance;
	_testObject->GetTransform()->SetLocalPosition(Vec3(position.x, position.y, position.z));
}
