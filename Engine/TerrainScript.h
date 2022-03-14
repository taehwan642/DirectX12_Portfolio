#pragma once
#include "MonoBehaviour.h"

class TerrainScript : public MonoBehaviour
{
public:
	MONOBEHAVIOUR(TerrainScript)
	virtual ~TerrainScript();

	virtual void LateUpdate() override;
	void Picked(Vec4 rayOrigin, Vec4 rayDir, float distance);

	std::shared_ptr<GameObject> _testObject;

private:
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND

};

