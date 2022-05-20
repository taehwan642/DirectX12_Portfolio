#pragma once
#include "Component.h"

enum class MonoBehaviourType
{
	TestCameraScript,
	GameManagerScript,
	Sea,
	EnemyBullet,
	EffectManagerScript,
	Effect,
	InfinitePassage,
	RenderTargetScript,
	PathTraceCameraScript,
	LaserScript,
	Player,
	Enemy,
	EnemySpawner,
	END
};

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

	virtual void OnCollisionEnter(CollisionOutput collider) = 0;
	virtual void OnCollisionStay(CollisionOutput collider) = 0;
	virtual void OnCollisionExit(CollisionOutput collider) = 0;

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

