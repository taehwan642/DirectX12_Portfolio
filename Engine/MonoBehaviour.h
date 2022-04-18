#pragma once
#include "Component.h"

enum class MonoBehaviourType
{
	TestCameraScript,
	TestDragon,
	TerrainScript,
	GameManagerScript,
	Sea,
	EnemyBullet,
	EffectManagerScript,
	Effect,
	InfinitePassage,
	RenderTargetScript,
	PathTraceCameraScript,
	END
};

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

	virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) = 0;
	virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) = 0;
	virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) = 0;

protected:
	friend class ImGuiManager;
	std::wstring _className = L"";

private:
	virtual void FinalUpdate() final { }

#ifdef TOOL
	virtual void DragAndDrop(size_t hash) {};
#endif

	RTTR_ENABLE()
};

