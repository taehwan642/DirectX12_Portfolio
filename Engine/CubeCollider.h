#pragma once
#include "BaseCollider.h"
class CubeCollider :
    public BaseCollider
{
public:
	static int _num;

public:
	CubeCollider();
	virtual ~CubeCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;

private:
	friend class ImGuiManager;

	BoundingBox _boundingBox;
};

