#pragma once
#include "BaseCollider.h"

class Visualizer;
struct BoneColliderInfo
{
	int boneIndex;
	std::string boneName;
	BoundingSphere sphere;
};

class BoneCollider :
    public BaseCollider
{
public:
	BoneCollider();
	virtual ~BoneCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance) override;

	void AddBoneCollider(const std::string& boneName, float radius = 1.f);
	void RemoveBoneCollider(const std::string& boneName);
	void SetBoneColliderRadius(const std::string& boneName, float radius);

	void LoadCollider(const std::string& colliderName);
	void SaveCollider(const std::string& colliderName);

private:
	friend class ImGuiManager;

	std::vector<BoneColliderInfo> _boneColliders;

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};

