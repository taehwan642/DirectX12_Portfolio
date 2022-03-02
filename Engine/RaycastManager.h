#pragma once
class RaycastManager
{
	DECLARE_SINGLE(RaycastManager);

	// Scene�� �ִ� Ray ��� �����ͼ� ���̱�.
	// ���� ������� �����������.

public:
	std::shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);

};
