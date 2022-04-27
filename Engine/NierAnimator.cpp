#include "pch.h"
#include "NierAnimator.h"
#include "Timer.h"

NierAnimator::NierAnimator() : Animator()
{

}

NierAnimator::~NierAnimator()
{
}

void NierAnimator::SetAnimationFrames(const std::vector<int>& animFrames)
{
	// 받아온 animation Frame을 기준으로 animation 돌리기
	_animFrames = animFrames;
	_fullAnimTrack = (*_animClips)[0].frameCount;
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
	if (_frame >= _animFrames[_currentAnimIndex])
	{
		// 0번째 애니메이션 트랙 : 0 ~ [1]
		// 1번째 애니메이션 트랙 : 1 ~ [120]
		// 총 애니메이션 트랙 : 121
		// 1번째 애니메이션 트랙이라면 1부터 시작해야 하기 때문에, 120을 더해서 _fullAnimTrack이랑 빼면 1이 나온다.
		int sum = std::accumulate((_animFrames.begin() + _currentAnimIndex), _animFrames.end(), 0);
		_frame = _fullAnimTrack - sum;
		_updateTime = _frame / 30.f; // frame / 30FPS
	}
}
