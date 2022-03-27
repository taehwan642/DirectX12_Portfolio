#include "pch.h"
#include "MeshCollider.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Shader.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Mesh.h"

MeshCollider::MeshCollider() : BaseCollider(ColliderType::Mesh)
{
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::Init()
{
	std::shared_ptr<Mesh> mesh = GetGameObject()->GetMeshRenderer()->GetMesh();

	_vertices = std::shared_ptr<Vertex[]>(new Vertex[mesh->_vertexCount]);

	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	uint32 bufferSize = mesh->_vertexCount * sizeof(Vertex);
	mesh->_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(&_vertices[0], vertexDataBuffer, bufferSize);
	mesh->_vertexBuffer->Unmap(0, nullptr);

	int totalIndicesCount = 0;
	for (int i = 0; i < mesh->_vecIndexInfo.size(); ++i)
	{
		totalIndicesCount += mesh->_vecIndexInfo[i].count;
	}
	
	_indices = std::shared_ptr<uint32[]>(new uint32[totalIndicesCount]);

	int currentIndicesCount = 0;
	for (int i = 0; i < mesh->_vecIndexInfo.size(); ++i)
	{
		void* indexDataBuffer = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		uint32 bufferSize = mesh->_vecIndexInfo[i].count * sizeof(uint32);
		mesh->_vecIndexInfo[i].buffer->Map(0, &readRange, &indexDataBuffer);
		::memcpy(&_indices[currentIndicesCount], indexDataBuffer, bufferSize);
		mesh->_vecIndexInfo[i].buffer->Unmap(0, nullptr);

		currentIndicesCount += mesh->_vecIndexInfo[i].count;
	}

	_triCount = totalIndicesCount / 3;
}

void MeshCollider::FinalUpdate()
{

}

// rayOrigin, rayDir 모두 local.
bool MeshCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	Matrix worldMatrix = GetTransform()->GetWorldMatrix();
	Matrix worldInvMatrix = worldMatrix.Invert();
	// LocalSpace에서의 Ray 정의
	Vec4 localRayOrigin = XMVector3TransformCoord(rayOrigin, worldInvMatrix);
	Vec4 localRayDir = XMVector3TransformNormal(rayDir, worldInvMatrix);
	localRayDir.Normalize();

	float min = D3D12_FLOAT32_MAX;
	bool intersected = false;
	for (uint32 i = 0; i < _triCount; ++i)
	{
		uint32 i0 = _indices[i * 3 + 0];
		uint32 i1 = _indices[i * 3 + 1];
		uint32 i2 = _indices[i * 3 + 2];
		
		Vec3 v0 = _vertices[i0].pos;
		Vec3 v1 = _vertices[i1].pos;
		Vec3 v2 = _vertices[i2].pos;

		float t;
		if (TriangleTests::Intersects(localRayOrigin, localRayDir, v0, v1, v2, t))
		{
			if (t < min)
			{
				min = t;
				uint32 pickedTriangle = i;
				distance = t;
				return true;
			}
		}
	}
    return false;
}
