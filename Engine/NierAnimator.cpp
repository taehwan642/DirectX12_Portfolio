#include "pch.h"
#include "NierAnimator.h"
#include "Timer.h"
#include "Engine.h"
#include "GameObject.h"

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
	if (index == _currentAnimIndex)
		return;

	if (!GetGameObject()->GetActive())
		return;

	_isAnimEnd = false;
	_currentAnimIndex = index;
	_nextFrame = _animFrames[_currentAnimIndex].x;
	_changedFrame = _frame;

	ADDLOG("CHANGE ANI! frame : %d, nextframe : %d, changedframe : %d, animindex : %d\n", _frame, _nextFrame, _changedFrame, index);
}

void NierAnimator::Update()
{
	if (_paused == true)
		return;

	_isAnimEnd = false;

	const AnimClipInfo& animClip = _animClips->at(_clipIndex);
	const int32 ratio = static_cast<int32>(animClip.frameCount / animClip.duration);
	// _clipIndex를 통해서 바꾸기
	if (_frame >= _animFrames[_currentAnimIndex].y)
	{
		if (_changedFrame == -1)
			_isAnimEnd = true;
		ADDLOG("END\n");
	}
}

void NierAnimator::FinalUpdate()
{
	if (_paused == true)
		return;

	const AnimClipInfo& animClip = _animClips->at(_clipIndex);
	const int32 ratio = static_cast<int32>(animClip.frameCount / animClip.duration);

	if (_frame >= _animFrames[_currentAnimIndex].y)
	{
		_frame = _animFrames[_currentAnimIndex].x;
		_nextFrame = _animFrames[_currentAnimIndex].x + 1;
		_updateTime = _frame / static_cast<float>(ratio); // frame / 30FPS
		ADDLOG("FRAME RELOAD\n");
	}

	_updateTime += DELTA_TIME;

	if (_updateTime >= animClip.duration)
		_updateTime = 0.f;

	
	_frame = static_cast<int32>(_updateTime * ratio);
	_frame = min(_frame, animClip.frameCount - 1);
	if (_changedFrame != -1 )
	{
		if (_changedFrame != _frame)
		{
			_changedFrame = -1;
			_frame = _nextFrame;

			if (_frame + 1 > animClip.frameCount - 1)
				_nextFrame = 0;
			else if (_frame + 1 > _animFrames[_currentAnimIndex].y)
				_nextFrame = _animFrames[_currentAnimIndex].x + 1;
			else
				_nextFrame = _frame + 1;

			_updateTime = _frame / static_cast<float>(ratio);
		}
	}
	else
	{
		if (_frame + 1 > animClip.frameCount - 1)
			_nextFrame = 0;
		else if (_frame + 1 > _animFrames[_currentAnimIndex].y)
			_nextFrame = _animFrames[_currentAnimIndex].x + 1;
		else
			_nextFrame = _frame + 1;
	}
	
	int32 nextFrameUpdateTime = (_frame + 1);
	_frameRatio = static_cast<float>(1.f - (nextFrameUpdateTime - (_updateTime * ratio)));

	ADDLOG("frame : %d, nextframe : %d, changedframe : %d\n", _frame, _nextFrame, _changedFrame);
}
