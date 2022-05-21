#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Shader.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Visualizer.h"

BoxCollider::BoxCollider() : BaseCollider(ColliderType::Box)
{
	_colliderVisualizers.push_back(std::make_shared<Visualizer>());
	_colliderVisualizers[0]->SetCubeMesh();
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::FinalUpdate()
{
	if (_draw)
	{
		_colliderVisualizers[0]->FinalUpdate(_center + GetTransform()->GetWorldPosition(), _extents, _orientation);
	}

	_boundingBox.Center = _center + GetGameObject()->GetTransform()->GetWorldPosition();
	_boundingBox.Extents = _extents;
	_boundingBox.Orientation = SimpleMath::Quaternion::CreateFromYawPitchRoll(
		DegreeToRadian(_orientation.y), 
		DegreeToRadian(_orientation.x), 
		DegreeToRadian(_orientation.z));
}

bool BoxCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	return _boundingBox.Intersects(rayOrigin, rayDir, OUT distance);
}
