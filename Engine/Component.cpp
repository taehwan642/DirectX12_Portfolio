#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Animator.h"

Component::Component(COMPONENT_TYPE type) : Object(OBJECT_TYPE::COMPONENT), _type(type)
{

}

Component::~Component()
{
}

std::shared_ptr<GameObject> Component::GetGameObject()
{
	return _gameObject.lock();
}

std::shared_ptr<TransformComponent> Component::GetTransform()
{
	return _gameObject.lock()->GetTransform();
}

std::shared_ptr<MeshRenderer> Component::GetMeshRenderer()
{
	return _gameObject.lock()->GetMeshRenderer();
}

std::shared_ptr<Animator> Component::GetAnimator()
{
	return _gameObject.lock()->GetAnimator();
}