#pragma once
#include "MonoBehaviour.h"
class InfinitePassage :
    public MonoBehaviour
{
    // 통로 옮기는 거
public:
	MONOBEHAVIOUR(InfinitePassage)

	virtual ~InfinitePassage();

	virtual void OnCollisionEnter(CollisionOutput collider) override {}
	virtual void OnCollisionStay(CollisionOutput collider) override {}
	virtual void OnCollisionExit(CollisionOutput collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

