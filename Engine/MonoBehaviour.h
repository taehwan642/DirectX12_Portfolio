#pragma once
#include "Component.h"

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
};

