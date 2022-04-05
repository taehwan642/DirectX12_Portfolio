#pragma once
#include "MonoBehaviour.h"
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

	size_t _testObject;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

