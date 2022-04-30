#pragma once
class GameObject;
class ObjectPool
{
	DECLARE_SINGLE(ObjectPool);

public:
	void AddPoolObject(const std::string& objectTag, std::shared_ptr<GameObject> object);
	std::shared_ptr<GameObject> GetPoolObject(const std::string& objectTag);

private:
	std::map<std::string, std::vector<std::shared_ptr<GameObject>>> _objectPool;
};

