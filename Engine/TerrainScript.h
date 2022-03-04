#pragma once
#include "MonoBehaviour.h"

class TerrainScript : public MonoBehaviour
{
public:
	TerrainScript();
	virtual ~TerrainScript();

	virtual void LateUpdate() override;
	void Picked(Vec4 rayOrigin, Vec4 rayDir, float distance);

	std::shared_ptr<GameObject> _testObject;
private:

};

