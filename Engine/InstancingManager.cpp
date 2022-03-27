#include "pch.h"
#include "InstancingManager.h"
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"

void InstancingManager::Render(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
	std::map<uint64, std::vector<std::shared_ptr<GameObject>>> cache;

	for (std::shared_ptr<GameObject>& gameObject : gameObjects)
	{
		const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const std::vector<std::shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			if (vec[0]->GetMeshRenderer()->GetMaterial(0)->_params.intParams[0] == 1)
			{
				for (int i = 0; i < vec[0]->GetMeshRenderer()->GetMesh()->GetSubsetCount(); ++i)
				{
					vec[0]->GetMeshRenderer()->GetMaterial(i)->SetInt(0, 0);
				}

				vec[0]->GetMeshRenderer()->Render();
				
				for (int i = 0; i < vec[0]->GetMeshRenderer()->GetMesh()->GetSubsetCount(); ++i)
				{
					vec[0]->GetMeshRenderer()->GetMaterial(i)->SetInt(0, 1);
				}
			}
			else
				vec[0]->GetMeshRenderer()->Render();
		}
		else
		{
			if (vec[0]->GetMeshRenderer()->GetMaterial(0)->_params.intParams[0] == 0)
			{
				for (auto& iter : pair.second)
				{
					iter->GetMeshRenderer()->Render();
				}
			}
			else
			{
				const uint64 instanceId = pair.first;

				for (const std::shared_ptr<GameObject>& gameObject : vec)
				{
					InstancingParams params;
					params.matWorld = gameObject->GetTransform()->GetWorldMatrix();
					params.matWV = params.matWorld * Camera::S_MatView;
					params.matWVP = params.matWorld * Camera::S_MatView * Camera::S_MatProjection;

					AddParam(instanceId, params);
				}

				std::shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
				vec[0]->GetMeshRenderer()->Render(buffer);
			}
		}
	}
}

void InstancingManager::ClearBuffer()
{
	for (auto& pair : _buffers)
	{
		std::shared_ptr<InstancingBuffer>& buffer = pair.second;
		buffer->Clear();
	}
}

void InstancingManager::AddParam(uint64 instanceId, InstancingParams& data)
{
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = std::make_shared<InstancingBuffer>();

	_buffers[instanceId]->AddData(data);
}