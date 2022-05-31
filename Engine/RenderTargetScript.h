#pragma once
#include "MonoBehaviour.h"
class RenderTargetScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(RenderTargetScript)

	virtual ~RenderTargetScript();

	virtual void OnCollisionEnter(const CollisionOutput& collider) override {}
	virtual void OnCollisionStay(const CollisionOutput& collider) override {}
	virtual void OnCollisionExit(const CollisionOutput& collider) override {}

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

