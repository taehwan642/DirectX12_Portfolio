#pragma once
#include "MonoBehaviour.h"

class TerrainScript : public MonoBehaviour
{
public:
	MONOBEHAVIOUR(TerrainScript)
	virtual ~TerrainScript();

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

	virtual void LateUpdate() override;
	void Picked(Vec4 rayOrigin, Vec4 rayDir, float distance);

#ifdef TOOL
	virtual void DragAndDrop(size_t hash) override;
#endif

	size_t _testObject;

private:
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND

};

