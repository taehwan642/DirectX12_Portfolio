#pragma once
#include "MonoBehaviour.h"
class InfinitePassage :
    public MonoBehaviour
{
    // ��� �ű�� ��
public:
	MONOBEHAVIOUR(InfinitePassage)

	virtual ~InfinitePassage();

	virtual void LateUpdate() override;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};
