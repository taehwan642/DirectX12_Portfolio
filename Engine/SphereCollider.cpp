#include "pch.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Shader.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Visualizer.h"
#include "Scene.h"

SphereCollider::SphereCollider() : BaseCollider(ColliderType::Sphere)
{
	_colliderVisualizers.push_back(std::make_shared<Visualizer>());
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::FinalUpdate()
{
	if (_draw)
	{
		_colliderVisualizers[0]->FinalUpdate(_center + GetTransform()->GetWorldPosition(), Vec3(_radius));
	}

	_boundingSphere.Center = _center + GetGameObject()->GetTransform()->GetWorldPosition();
	_boundingSphere.Radius = _radius;
}

bool SphereCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	return _boundingSphere.Intersects(rayOrigin, rayDir, OUT distance);
}