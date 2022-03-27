#include "pch.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "Camera.h"
#include "GameObject.h"

TransformComponent::TransformComponent() : Component(COMPONENT_TYPE::TRANSFORM)
{
	_localTransform = std::make_shared<Transform>();
	_worldTransform = std::make_shared<Transform>();
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

void TransformComponent::SetLocalPosition(const Vec3& InPosition)
{
	_localTransform->SetPosition(InPosition);
	UpdateWorld();
}

void TransformComponent::AddLocalYawRotation(float InDegree)
{
	_localTransform->AddYawRotation(InDegree);
	UpdateWorld();
}

void TransformComponent::AddLocalRollRotation(float InDegree)
{
	_localTransform->AddRollRotation(InDegree);
	UpdateWorld();
}

void TransformComponent::AddLocalPitchRotation(float InDegree)
{
	_localTransform->AddPitchRotation(InDegree);
	UpdateWorld();
}

void TransformComponent::SetLocalRotation(const Vec3& InQuaternion)
{
	_localTransform->SetRotation(InQuaternion);
	UpdateWorld();
}

void TransformComponent::SetLocalScale(const Vec3& InScale)
{
	_localTransform->SetScale(InScale);
	UpdateWorld();
}

void TransformComponent::SetWorldPosition(const Vec3& InPosition)
{
	_worldTransform->SetPosition(InPosition);
	UpdateLocal();
}

void TransformComponent::AddWorldYawRotation(float InDegree)
{
	_worldTransform->AddYawRotation(InDegree);
	UpdateLocal();
}

void TransformComponent::AddWorldRollRotation(float InDegree)
{
	_worldTransform->AddRollRotation(InDegree);
	UpdateLocal();
}

void TransformComponent::AddWorldPitchRotation(float InDegree)
{
	_worldTransform->AddPitchRotation(InDegree);
	UpdateLocal();
}

void TransformComponent::SetWorldRotation(const Vec3& InQuaternion)
{
	_worldTransform->SetRotation(InQuaternion);
	UpdateLocal();
}

void TransformComponent::SetWorldScale(const Vec3& InScale)
{
	_worldTransform->SetScale(InScale);
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
