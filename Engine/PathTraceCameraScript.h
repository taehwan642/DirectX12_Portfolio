#pragma once
#include "MonoBehaviour.h"
class PathTraceCameraScript :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(PathTraceCameraScript)

	virtual ~PathTraceCameraScript();

	virtual void OnCollisionEnter(const CollisionOutput& collider) override {}
	virtual void OnCollisionStay(const CollisionOutput& collider) override {}
	virtual void OnCollisionExit(const CollisionOutput& collider) override {}

	virtual void LateUpdate() override;

#ifdef TOOL
	virtual void DragAndDrop(size_t hash) override;
#endif

	void ShakeCamera();

private:
	int FindCurrentArea();
	const Vec3& GetRandomVec3();

private:
	std::vector<size_t> _objects;
	float _shakeTime = 0.f;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

