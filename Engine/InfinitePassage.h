#pragma once
#include "MonoBehaviour.h"
class InfinitePassage :
    public MonoBehaviour
{
    // 통로 옮기는 거
public:
	MONOBEHAVIOUR(InfinitePassage)

	virtual ~InfinitePassage();

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

