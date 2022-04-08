#pragma once
#include "BaseCollider.h"
class MeshCollider :
    public BaseCollider
{
public:
	MeshCollider();
	virtual ~MeshCollider();

	void Init();

	virtual void FinalUpdate() override;
	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance) override;

private:
	friend class ImGuiManager;

	std::shared_ptr<Vertex[]> _vertices;
	std::shared_ptr<uint32[]> _indices;

	uint32 _triCount = 0;

	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE()
};

