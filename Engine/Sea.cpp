#include "pch.h"
#include "Sea.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Timer.h"

Sea::~Sea()
{
}

void Sea::LateUpdate()
{
	static float inputTime = 0;
	inputTime += DELTA_TIME;
	GetMeshRenderer()->GetMaterial()->SetFloat(0, inputTime);
}
