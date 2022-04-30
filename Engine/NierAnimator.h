#pragma once
#include "Animator.h"
class NierAnimator :
    public Animator
{
public:
	NierAnimator();
	virtual ~NierAnimator();

public:
	void SetAnimationFrames(const std::vector<Vec2>& animFrames);
	void SetAnimationIndex(int index);

public:
	virtual void FinalUpdate() override;

private:
	friend class ImGuiManager;

	int _changedFrame = -1;
	int _currentAnimIndex = 0; // _clipIndex¶û Çò°¥¸®¸é ¾ÈµÊ.
	int _fullAnimFrame = 0;
	std::vector<Vec2> _animFrames;

RTTR_REGISTRATION_FRIEND
};

