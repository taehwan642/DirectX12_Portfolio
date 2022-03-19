#include "pch.h"
#include "CubeCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"

int CubeCollider::_num = 0;

CubeCollider::CubeCollider() : BaseCollider(ColliderType::Cube)
{
	// ONLY FOR DEBUG.
	std::shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"WireFrame");
	_material = std::make_shared<Material>();
	_material->SetShader(shader);
	_mesh = GET_SINGLE(Resources)->LoadCubeMesh();
	std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
	meshRenderer->SetMaterial(_material);
	meshRenderer->SetMesh(_mesh);
	_colliderMeshGameObject = std::make_shared<GameObject>();
	_colliderMeshGameObject->SetName(L"CubeCollider " + std::to_wstring(_num));
	_num++;
	_colliderMeshGameObject->AddComponent(std::make_shared<Transform>());
	_colliderMeshGameObject->AddComponent(meshRenderer);
}

CubeCollider::~CubeCollider()
{
}

void CubeCollider::FinalUpdate()
{
	if (_draw)
	{
		Vec3 meshPos = GetTransform()->GetLocalPosition();
		_colliderMeshGameObject->GetTransform()->SetLocalPosition(meshPos);
		Vec3 meshScale = GetTransform()->GetLocalScale() * 1.3f;
		_colliderMeshGameObject->GetTransform()->SetLocalScale(meshScale);
		_colliderMeshGameObject->GetTransform()->FinalUpdate();
	}

	_boundingBox.Center = _center = GetGameObject()->GetTransform()->GetWorldPosition();
	_boundingBox.Extents = _extents = GetGameObject()->GetTransform()->GetLocalScale();
}

bool CubeCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	return _boundingBox.Intersects(rayOrigin, rayDir, OUT distance);
}
