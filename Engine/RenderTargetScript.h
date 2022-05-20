#pragma once
#include "MonoBehaviour.h"
class RenderTargetScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(RenderTargetScript)

	virtual ~RenderTargetScript();

	virtual void OnCollisionEnter(CollisionOutput collider) override {}
	virtual void OnCollisionStay(CollisionOutput collider) override {}
	virtual void OnCollisionExit(CollisionOutput collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

