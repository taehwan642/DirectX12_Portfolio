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

class ImGuiManager;
class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

protected:
	friend class ImGuiManager;
	std::wstring _className = L"";

private:
	virtual void FinalUpdate() final { }

// #ifdef TOOL
	virtual void DragAndDrop(size_t hash) {};
// #endif

	RTTR_ENABLE()
};

