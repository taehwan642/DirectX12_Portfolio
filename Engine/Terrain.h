#pragma once
#include "Component.h"

class ImGuiManager;
class Terrain : public Component
{
public:
	Terrain();
	virtual ~Terrain();

	void Init(int32 sizeX, int32 sizeZ);

	virtual void FinalUpdate() override;

private:
	friend class ImGuiManager;
	friend class RTTRTerrainValue;

	int32 _sizeX = 15;
	int32 _sizeZ = 15;
	float _maxTesselation = 4.f;

	std::shared_ptr<class Material> _material;

	RTTR_REGISTRATION_FRIEND
};


