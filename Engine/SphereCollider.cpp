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
		_colliderVisualizers[0]->FinalUpdate(GetTransform()->GetWorldPosition());
	}

	_boundingSphere.Center = _center = GetGameObject()->GetTransform()->GetWorldPosition();

	Vec3 scale = GetGameObject()->GetTransform()->GetWorldScale();
	_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);
}

bool SphereCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	return _boundingSphere.Intersects(rayOrigin, rayDir, OUT distance);
}