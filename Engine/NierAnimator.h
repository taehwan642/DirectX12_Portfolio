#pragma once
#include "Animator.h"
class NierAnimator :
    public Animator
{
public:
	NierAnimator();
	virtual ~NierAnimator();

public:
	void SetAnimationFrames(const std::vector<int>& animFrames);

public:
	virtual void FinalUpdate() override;

private:
	friend class ImGuiManager;

	int _currentAnimIndex = 1; // _clipIndex¶û Çò°¥¸®¸é ¾ÈµÊ.
	int _fullAnimTrack = 0;
	std::vector<int> _animFrames;

RTTR_REGISTRATION_FRIEND
};

