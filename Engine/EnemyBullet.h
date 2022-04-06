#pragma once
#include "MonoBehaviour.h"

enum class EnemyBulletState
{
	DESTROYABLE,
	NON_DESTROYABLE
};

class EnemyBullet :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(EnemyBullet)

	virtual ~EnemyBullet();

	virtual void LateUpdate() override;

#ifdef TOOL
	virtual void DragAndDrop(size_t hash) override;
#endif

private:
	EnemyBulletState _state = EnemyBulletState::DESTROYABLE;
	size_t _testObject;
	Vec3 _direction;


	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

