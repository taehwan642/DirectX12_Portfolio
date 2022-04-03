#include "pch.h"
#include "GameObject.h"
#include "TransformComponent.h"
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
	if (_isActive == false)
		return;
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

#ifndef TOOL
	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Awake();
	}
#else
	if (GetName() == L"ToolCamera")
	{
		for (std::shared_ptr<MonoBehaviour>& script : _scripts)
		{
			script->Awake();
		}
	}
#endif
}

void GameObject::Start()
{
	if (_isActive == false)
		return; 
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

#ifndef TOOL
	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Start();
	}
#else
	if (GetName() == L"ToolCamera")
	{
		for (std::shared_ptr<MonoBehaviour>& script : _scripts)
		{
			script->Start();
		}
	}
#endif
}

void GameObject::Update()
{
	if (_isActive == false)
		return;
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

#ifndef TOOL
	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Update();
	}
#else
	if (GetName() == L"ToolCamera")
	{
		for (std::shared_ptr<MonoBehaviour>& script : _scripts)
		{
			script->Update();
		}
	}
#endif
}

void GameObject::LateUpdate()
{
	if (_isActive == false)
		return;
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

#ifndef TOOL
	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->LateUpdate();
	}
#else
	if (GetName() == L"ToolCamera")
	{
		for (std::shared_ptr<MonoBehaviour>& script : _scripts)
		{
			script->LateUpdate();
		}
	}
#endif
}

void GameObject::FinalUpdate()
{
	if (_isActive == false)
		return;
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}
}

std::shared_ptr<TransformComponent> GameObject::GetTransform()
{
	return _transform;
}

std::shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	return _meshRenderer;
}

std::shared_ptr<Camera> GameObject::GetCamera()
{
	return _camera;
}

std::shared_ptr<Light> GameObject::GetLight()
{
	return _light;
}

std::shared_ptr<ParticleSystem> GameObject::GetParticleSystem()
{
	return _particleSystem;
}

std::shared_ptr<Terrain> GameObject::GetTerrain()
{
	return _terrain;
}

std::shared_ptr<BaseCollider> GameObject::GetCollider()
{
	return _baseCollider;
}

std::shared_ptr<Animator> GameObject::GetAnimator()
{
	return _animator;
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
		if (index == static_cast<uint8>(COMPONENT_TYPE::TRANSFORM))
			_transform = std::static_pointer_cast<TransformComponent>(component);
		if (index == static_cast<uint8>(COMPONENT_TYPE::MESH_RENDERER))
			_meshRenderer = std::static_pointer_cast<MeshRenderer>(component);
		if (index == static_cast<uint8>(COMPONENT_TYPE::CAMERA))
			_camera = std::static_pointer_cast<Camera>(component);
		if (index == static_cast<uint8>(COMPONENT_TYPE::LIGHT))
			_light = std::static_pointer_cast<Light>(component);
		if (index == static_cast<uint8>(COMPONENT_TYPE::PARTICLE_SYSTEM))
			_particleSystem = std::static_pointer_cast<ParticleSystem>(component);
		if (index == static_cast<uint8>(COMPONENT_TYPE::TERRAIN))
			_terrain = std::static_pointer_cast<Terrain>(component);
		if (index == static_cast<uint8>(COMPONENT_TYPE::COLLIDER))
			_baseCollider = std::static_pointer_cast<BaseCollider>(component);
		if (index == static_cast<uint8>(COMPONENT_TYPE::ANIMATOR))
			_animator = std::static_pointer_cast<Animator>(component);
	}
	else
	{
		_scripts.push_back(std::dynamic_pointer_cast<MonoBehaviour>(component));
	}
}

void GameObject::SetCheckFrustum(bool checkFrustum)
{
	_checkFrustum = checkFrustum;
	for (int i = 0; i < _transform->GetChildCount(); ++i)
	{
		_transform->GetChild(i)->GetGameObject()->SetCheckFrustum(checkFrustum);
	}
}

void GameObject::SetLayerIndex(uint8 layer)
{
	_layerIndex = layer;
	for (int i = 0; i < _transform->GetChildCount(); ++i)
	{
		_transform->GetChild(i)->GetGameObject()->SetLayerIndex(layer);
	}
}

void GameObject::SetStatic(bool flag)
{
	_static = flag;
	for (int i = 0; i < _transform->GetChildCount(); ++i)
	{
		_transform->GetChild(i)->GetGameObject()->SetStatic(flag);
	}
}

void GameObject::GenerateHash()
{
	// Hash값 넣기
	_hash = std::hash<std::wstring>()(_name);
	for (int i = 0; i < _transform->GetChildCount(); ++i)
	{
		_transform->GetChild(i)->GetGameObject()->GenerateHash();
	}
}

void GameObject::SetActive(bool active)
{
	// 자식이 있다면, 자식들도 모두 꺼준다.
	_isActive = active;
	for (int i = 0; i < _transform->GetChildCount(); ++i)
	{
		_transform->GetChild(i)->GetGameObject()->SetActive(active);
	}
}
