#pragma once
#include "MonoBehaviour.h"
class PathTraceCameraScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(PathTraceCameraScript)

	virtual ~PathTraceCameraScript();

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void LateUpdate() override;

private:

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

