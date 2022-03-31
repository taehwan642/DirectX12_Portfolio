#pragma once
#include "Component.h"
#include "Transform.h"

class TransformComponent : public Component
{
public:
	TransformComponent();
	
	virtual ~TransformComponent();

	void PushData();
	virtual void FinalUpdate() override;

public: // 로컬 트랜스폼 관련 함수
	std::shared_ptr<Transform> GetLocalTransform() { return _localTransform; }
	void SetLocalTransform(std::shared_ptr<Transform> localTransform) { _localTransform = localTransform; UpdateWorld(); }

	void SetLocalPosition(const Vec3& position);
	void AddLocalYawRotation(float degree);
	void AddLocalRollRotation(float degree);
	void AddLocalPitchRotation(float degree);
	void SetLocalRotation(const Vec3& rotation);
	void SetLocalScale(const Vec3& scale);

	Vec3 GetLocalPosition() const { return _localTransform->GetPosition(); }
	Vec3 GetLocalRotation() const { return _localTransform->GetRotation(); }
	Vec3 GetLocalScale() const { return _localTransform->GetScale(); }

	Vec3 GetLocalX() const { return _localTransform->GetRight(); }
	Vec3 GetLocalY() const { return _localTransform->GetUp(); }
	Vec3 GetLocalZ() const { return _localTransform->GetLook(); }
	Matrix GetLocalMatrix() const { return _localTransform->GetMatrix(); }

public: // 월드 트랜스폼 관련 함수
	std::shared_ptr<Transform> GetWorldTransform() { return _worldTransform; }
	void SetWorldTransform(std::shared_ptr<Transform> transform) { _worldTransform = transform; UpdateLocal(); }

	void SetWorldPosition(const Vec3& position);
	void AddWorldYawRotation(float degree);
	void AddWorldRollRotation(float degree);
	void AddWorldPitchRotation(float degree);
	void SetWorldRotation(const Vec3& rotation);
	void SetWorldScale(const Vec3& scale);

	Vec3 GetWorldPosition() const { return _worldTransform->GetPosition(); }
	Vec3 GetWorldRotation() const { return _worldTransform->GetRotation(); }
	Vec3 GetWorldScale() const { return _worldTransform->GetScale(); }

	Matrix GetWorldMatrix() const { return _worldTransform->GetMatrix(); }

	void SetLookAtRotation(const Vec3& direction);

public: // 계층 구조 관련 함수
	bool HasParent() const { return _parent.lock() != nullptr; }

private: // 내부에서만 호출하는 함수
	void UpdateLocal();
	void UpdateWorld();
	void UpdateChildrenWorld();

public:
	std::weak_ptr<TransformComponent> GetParent() { return _parent; }
	void SetParent(std::shared_ptr<TransformComponent> parent);
	void RemoveParent();

	std::shared_ptr<TransformComponent> GetChild(int i) { return _childVector[i].lock(); }


private: // 계층 구조를 위한 변수
	friend class ImGuiManager;

	std::shared_ptr<Transform> _localTransform;
	std::shared_ptr<Transform> _worldTransform;

	std::weak_ptr<TransformComponent> _parent;
	std::vector<std::weak_ptr<TransformComponent>> _childVector;

	RTTR_REGISTRATION_FRIEND
};

