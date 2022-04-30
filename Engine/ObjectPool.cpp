#include "pch.h"
#include "ObjectPool.h"
#include "TransformComponent.h"
#include "GameObject.h"

void ObjectPool::AddPoolObject(const std::string& objectTag, std::shared_ptr<GameObject> object)
{
	const auto& iter = _objectPool.find(objectTag);

	if (iter == _objectPool.end())
		_objectPool.emplace(objectTag, std::vector<std::shared_ptr<GameObject>>());

	_objectPool[objectTag].push_back(object);
}

std::shared_ptr<GameObject> ObjectPool::GetPoolObject(const std::string& objectTag)
{
	for (int i = 0; i < _objectPool[objectTag].size(); ++i)
	{
		std::shared_ptr<GameObject> poolObject = _objectPool[objectTag][i];
		if (poolObject->GetActive() == false)
		{
			poolObject->SetActive(true);
			return poolObject;
		}
	}

	return nullptr;
}
