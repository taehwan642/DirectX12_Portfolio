#pragma once
#include "MonoBehaviour.h"
class EffectManagerScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(EffectManagerScript)

	virtual ~EffectManagerScript();

	virtual void OnCollisionEnter(CollisionOutput collider) override {}
	virtual void OnCollisionStay(CollisionOutput collider) override {}
	virtual void OnCollisionExit(CollisionOutput collider) override {}

	virtual void LateUpdate() override;

	void SpawnEffect(const std::string& path, const Vec3& worldPosition);

#ifdef TOOL
	virtual void DragAndDrop(size_t hash) override;
#endif

	size_t _testObject;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

