#pragma once
#include "Component.h"

class ImGuiManager;
class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

private:
	virtual void FinalUpdate() final { }

private:
	friend class ImGuiManager;
	std::wstring _className = L"";
};

