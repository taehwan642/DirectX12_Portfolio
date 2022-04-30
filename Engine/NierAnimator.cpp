#include "pch.h"
#include "NierAnimator.h"
#include "Timer.h"

NierAnimator::NierAnimator() : Animator()
{

}

NierAnimator::~NierAnimator()
{
}

void NierAnimator::SetAnimationFrames(const std::vector<Vec2>& animFrames)
{
	// 받아온 animation Frame을 기준으로 animation 돌리기
	_animFrames = animFrames;
	_fullAnimFrame = (*_animClips)[0].frameCount;
}

void NierAnimator::SetAnimationIndex(int index)
{
	_currentAnimIndex = index;
	_frame = _animFrames[_currentAnimIndex].x;
	_updateTime = _frame / 30.f; // frame / 30FPS
}

void NierAnimator::FinalUpdate()
{
	if (_paused == true)
		return;

	_updateTime += DELTA_TIME;

	const AnimClipInfo& animClip = _animClips->at(_clipIndex);
	if (_updateTime >= animClip.duration)
		_updateTime = 0.f;

	const int32 ratio = static_cast<int32>(animClip.frameCount / animClip.duration);
	_frame = static_cast<int32>(_updateTime * ratio);
	_frame = min(_frame, animClip.frameCount - 1);
	_nextFrame = min(_frame + 1, animClip.frameCount - 1);
	_frameRatio = static_cast<float>(_frame - _frame);

	// _clipIndex를 통해서 바꾸기
	if (_frame >= _animFrames[_currentAnimIndex].y)
	{
		_frame = _animFrames[_currentAnimIndex].x;
		_updateTime = _frame / 30.f; // frame / 30FPS
	}
}
