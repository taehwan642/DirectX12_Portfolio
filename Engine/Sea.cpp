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
	GetMeshRenderer()->GetMaterial()->SetFloat(0, DELTA_TIME);
}
