#pragma once
#include "BaseCollider.h"
class MeshCollider :
    public BaseCollider
{
public:
	static int _num;

public:
	MeshCollider();
	virtual ~MeshCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;

private:
	friend class ImGuiManager;

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};

