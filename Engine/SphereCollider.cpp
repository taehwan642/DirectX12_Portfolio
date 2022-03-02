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

int SphereCollider::num = 0;

SphereCollider::SphereCollider() : BaseCollider(ColliderType::Sphere)
{

}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Render()
{
	std::shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"WireFrame");
	_material = std::make_shared<Material>();
	_material->SetShader(shader);
	_mesh = GET_SINGLE(Resources)->LoadSphereMesh();
	std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
	meshRenderer->SetMaterial(_material);
	meshRenderer->SetMesh(_mesh);
	_gameObject = std::make_shared<GameObject>();
	_gameObject->SetName(L"SphereCollider" + std::to_wstring(num));
	num++;
	_gameObject->AddComponent(std::make_shared<Transform>());
	Vec3 pos = GetTransform()->GetLocalPosition();
	_gameObject->GetTransform()->SetLocalPosition(pos);
	Vec3 scale = GetTransform()->GetLocalScale() * 1.3f;
	_gameObject->GetTransform()->SetLocalScale(scale);
	_gameObject->AddComponent(meshRenderer);
	GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(_gameObject);
}

void SphereCollider::FinalUpdate()
{
	_boundingSphere.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();
	_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);
}

bool SphereCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return _boundingSphere.Intersects(rayOrigin, rayDir, OUT distance);
}