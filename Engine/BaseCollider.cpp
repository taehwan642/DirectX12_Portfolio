#include "pch.h"
#include "BaseCollider.h"
bool BaseCollider::_drawCollider[static_cast<int>(ColliderType::END)]{};

BaseCollider::BaseCollider(ColliderType colliderType)
	: Component(COMPONENT_TYPE::COLLIDER), _colliderType(colliderType)
{

}

BaseCollider::~BaseCollider()
{

}