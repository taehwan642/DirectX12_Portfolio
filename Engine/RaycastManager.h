#pragma once
class RaycastManager
{
	DECLARE_SINGLE(RaycastManager);

	// Scene에 있는 Ray 기능 가져와서 붙이기.
	// 누굴 맞췄는지 리턴해줘야함.

public:
	std::shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);

	// Public보다는 다른 접근제한자가 좀 더 맞지만, 일단 지금은 public. 급한 불부터 끄자.
	Vec4 rayOrigin{};
	Vec4 rayDir{};
	float distance = 0.f;
};

