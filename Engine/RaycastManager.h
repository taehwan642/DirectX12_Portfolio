#pragma once
class RaycastManager
{
	DECLARE_SINGLE(RaycastManager);

	// Scene에 있는 Ray 기능 가져와서 붙이기.
	// 누굴 맞췄는지 리턴해줘야함.

public:
	std::shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);

};

