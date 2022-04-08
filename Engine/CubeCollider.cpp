#include "pch.h"
#include "CubeCollider.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Shader.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Visualizer.h"

CubeCollider::CubeCollider() : BaseCollider(ColliderType::Cube)
{
	_colliderVisualizers.push_back(std::make_shared<Visualizer>());
	_colliderVisualizers[0]->SetCubeMesh();
}

CubeCollider::~CubeCollider()
{
}

void CubeCollider::FinalUpdate()
{
	if (_draw)
	{
		_colliderVisualizers[0]->FinalUpdate(GetTransform()->GetWorldPosition());
	}

	_boundingBox.Center = _center = GetGameObject()->GetTransform()->GetWorldPosition();
	_boundingBox.Extents = _extents = GetGameObject()->GetTransform()->GetLocalScale();
}

bool CubeCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	return _boundingBox.Intersects(rayOrigin, rayDir, OUT distance);
}
