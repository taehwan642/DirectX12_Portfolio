#pragma once
#include "Component.h"

enum class MonoBehaviourType
{
	TestCameraScript,
	TestDragon,
	TerrainScript,
	GameManagerScript,
	END
};

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

protected:
	std::wstring _className = L"";

private:
	virtual void FinalUpdate() final { }

	RTTR_ENABLE()
};

