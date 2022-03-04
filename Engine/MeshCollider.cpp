#include "pch.h"
#include "MeshCollider.h"

MeshCollider::MeshCollider() : BaseCollider(ColliderType::Cube)
{
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::FinalUpdate()
{

}

bool MeshCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
    return false;
}
