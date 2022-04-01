#pragma once
#include "MonoBehaviour.h"

class TerrainScript : public MonoBehaviour
{
public:
	MONOBEHAVIOUR(TerrainScript)
	virtual ~TerrainScript();

	virtual void LateUpdate() override;
	void Picked(Vec4 rayOrigin, Vec4 rayDir, float distance);

	virtual void DragAndDrop(size_t hash) override;

	size_t _testObject;

private:
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND

};

