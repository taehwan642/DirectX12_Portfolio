#include "pch.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"

int SphereCollider::_num = 0;

SphereCollider::SphereCollider() : BaseCollider(ColliderType::Sphere)
{
	// ONLY FOR DEBUG.
	std::shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"WireFrame");
	_material = std::make_shared<Material>();
	_material->SetShader(shader);
	_mesh = GET_SINGLE(Resources)->LoadSphereMesh();
	std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
	meshRenderer->SetMaterial(_material);
	meshRenderer->SetMesh(_mesh);
	_colliderMeshGameObject = std::make_shared<GameObject>();
	_colliderMeshGameObject->SetName(L"SphereCollider" + std::to_wstring(_num));
	_num++;
	_colliderMeshGameObject->AddComponent(std::make_shared<Transform>());
	_colliderMeshGameObject->AddComponent(meshRenderer);
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::FinalUpdate()
{
	if (_draw)
	{
		Vec3 meshPos = GetTransform()->GetLocalPosition();
		_colliderMeshGameObject->GetTransform()->SetLocalPosition(meshPos);
		Vec3 meshScale = GetTransform()->GetLocalScale() * 1.3f;
		_colliderMeshGameObject->GetTransform()->SetLocalScale(meshScale);
		_colliderMeshGameObject->GetTransform()->FinalUpdate();
	}

	_boundingSphere.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();
	_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);
}

bool SphereCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return _boundingSphere.Intersects(rayOrigin, rayDir, OUT distance);
}