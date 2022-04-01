#include "pch.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "Camera.h"
#include "GameObject.h"

TransformComponent::TransformComponent() : Component(COMPONENT_TYPE::TRANSFORM)
{
	_localTransform = Transform::Create();
	_worldTransform = Transform::Create();
}

void TransformComponent::FinalUpdate()
{
	
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::PushData()
{
	TransformParams transformParams = {};
	transformParams.matWorld = _worldTransform->GetMatrix();
	transformParams.matView = Camera::S_MatView;
	transformParams.matProjection = Camera::S_MatProjection;
	transformParams.matWV = _worldTransform->GetMatrix() * Camera::S_MatView;
	transformParams.matWVP = _worldTransform->GetMatrix() * Camera::S_MatView * Camera::S_MatProjection;
	transformParams.matViewInv = Camera::S_MatView.Invert();

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushGraphicsData(&transformParams, sizeof(transformParams));
}

void TransformComponent::SetLocalPosition(const Vec3& position)
{
	_localTransform->SetPosition(position);
	UpdateWorld();
}

void TransformComponent::AddLocalYawRotation(float degree)
{
	_localTransform->AddYawRotation(degree);
	UpdateWorld();
}

void TransformComponent::AddLocalRollRotation(float degree)
{
	_localTransform->AddRollRotation(degree);
	UpdateWorld();
}

void TransformComponent::AddLocalPitchRotation(float degree)
{
	_localTransform->AddPitchRotation(degree);
	UpdateWorld();
}

void TransformComponent::SetLocalRotation(const Vec3& rotation)
{
	_localTransform->SetRotation(rotation);
	UpdateWorld();
}

void TransformComponent::SetLocalScale(const Vec3& scale)
{
	_localTransform->SetScale(scale);
	UpdateWorld();
}

void TransformComponent::SetWorldPosition(const Vec3& position)
{
	_worldTransform->SetPosition(position);
	UpdateLocal();
}

void TransformComponent::AddWorldYawRotation(float degree)
{
	_worldTransform->AddYawRotation(degree);
	UpdateLocal();
}

void TransformComponent::AddWorldRollRotation(float degree)
{
	_worldTransform->AddRollRotation(degree);
	UpdateLocal();
}

void TransformComponent::AddWorldPitchRotation(float degree)
{
	_worldTransform->AddPitchRotation(degree);
	UpdateLocal();
}

void TransformComponent::SetWorldRotation(const Vec3& rotation)
{
	_worldTransform->SetRotation(rotation);
	UpdateLocal();
}

void TransformComponent::SetWorldScale(const Vec3& scale)
{
	_worldTransform->SetScale(scale);
	UpdateLocal();
}

void TransformComponent::SetLookAtRotation(const Vec3& direction)
{
	_worldTransform->LookAt(direction);
	UpdateLocal();
}

void TransformComponent::UpdateLocal()
{
	if (HasParent())
	{
		std::weak_ptr<TransformComponent> parent = GetParent();
		_localTransform = _worldTransform->WorldToLocal(parent.lock()->GetWorldTransform());
	}
	else
	{
		_localTransform = _worldTransform;
	}

	UpdateChildrenWorld();
}

void TransformComponent::UpdateWorld()
{
	// 자신의 월드 정보 업데이트
	if (HasParent())
	{
		std::weak_ptr<TransformComponent> parent = GetParent();
		_worldTransform = _localTransform->LocalToWorld(parent.lock()->GetWorldTransform());
	}
	else
	{
		_worldTransform = _localTransform;
	}

	// 월드 정보 변경 시 자식의 월드 정보도 업데이트 ( 로컬 정보는 변함 없음. )
	UpdateChildrenWorld();
}

void TransformComponent::UpdateChildrenWorld()
{
	for (auto it = _childVector.begin(); it != _childVector.end(); ++it)
	{
		it->lock()->UpdateWorld();
	}
}

void TransformComponent::SetParent(std::shared_ptr<TransformComponent> parent)
{
	UpdateLocal();
	
	// 자신의 옛 부모가 존재한다면
	if (HasParent())
	{
		// 그 옛 부모에서 자신을 뺀다.
		auto findIt =
			std::find_if(_parent.lock()->_childVector.begin(), _parent.lock()->_childVector.end(),
				[&](std::weak_ptr<TransformComponent> obj)
				{
					return (GetGameObject()->GetTransform() == obj.lock());
				});

		if (findIt != _parent.lock()->_childVector.end())
			_parent.lock()->_childVector.erase(findIt);
	}

	// 새 부모를 받는다.
	_parent = parent;
	parent->_childVector.push_back(GetGameObject()->GetTransform());

	UpdateLocal();
}

void TransformComponent::RemoveParent()
{
	if (!HasParent())
		return;

	// 그 옛 부모에서 자신을 뺀다.
	auto findIt =
		std::find_if(_parent.lock()->_childVector.begin(), _parent.lock()->_childVector.end(),
			[&](std::weak_ptr<TransformComponent> obj)
			{
				return (GetGameObject()->GetTransform() == obj.lock());
			});

	if (findIt != _parent.lock()->_childVector.end())
		_parent.lock()->_childVector.erase(findIt);

	_parent.reset();
}
