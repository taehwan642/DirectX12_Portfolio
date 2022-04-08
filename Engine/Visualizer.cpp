#include "pch.h"
#include "Visualizer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Shader.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"

Visualizer::Visualizer()
{
	// ONLY FOR DEBUG.
	std::shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"WireFrame");
	_material = std::make_shared<Material>();
	_material->SetShader(shader);
	_mesh = GET_SINGLE(Resources)->LoadSphereMesh();
	_meshRenderer = std::make_shared<MeshRenderer>();
	_meshRenderer->SetMaterial(_material);
	_meshRenderer->SetMesh(_mesh);
	_transform = std::make_shared<TransformComponent>();
}

Visualizer::~Visualizer()
{
}

void Visualizer::SetCubeMesh()
{
	_meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadCubeMesh());
}

void Visualizer::SetSphereMesh()
{
	_meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadSphereMesh());
}

void Visualizer::FinalUpdate(const Vec3& position, const Vec3& scale)
{
	_transform->SetWorldPosition(position);
	_transform->SetWorldScale(scale);
}
