#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "MonoBehaviour.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "BaseCollider.h"
#include "Animator.h"

GameObject::GameObject() : Object(OBJECT_TYPE::GAMEOBJECT)
{

}

GameObject::~GameObject()
{

}

void GameObject::Awake()
{
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}
}

std::shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

std::shared_ptr<Transform> GameObject::GetTransform()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);
	return std::static_pointer_cast<Transform>(component);
}

std::shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER);
	return std::static_pointer_cast<MeshRenderer>(component);
}

std::shared_ptr<Camera> GameObject::GetCamera()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);
	return std::static_pointer_cast<Camera>(component);
}

std::shared_ptr<Light> GameObject::GetLight()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::LIGHT);
	return std::static_pointer_cast<Light>(component);
}

std::shared_ptr<ParticleSystem> GameObject::GetParticleSystem()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::PARTICLE_SYSTEM);
	return std::static_pointer_cast<ParticleSystem>(component);
}

std::shared_ptr<Terrain> GameObject::GetTerrain()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TERRAIN);
	return std::static_pointer_cast<Terrain>(component);
}

std::shared_ptr<BaseCollider> GameObject::GetCollider()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::COLLIDER);
	return std::static_pointer_cast<BaseCollider>(component);
}

std::shared_ptr<Animator> GameObject::GetAnimator()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::ANIMATOR);
	return std::static_pointer_cast<Animator>(component);
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	{
		_scripts.push_back(std::dynamic_pointer_cast<MonoBehaviour>(component));
	}
}