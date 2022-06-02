#pragma once
#include "pch.h"

class GameObject;
class GameObjectState
{
public:
	explicit GameObjectState(std::weak_ptr<GameObject> obj) : _object(obj) {};
	virtual ~GameObjectState() = default;

protected:
	std::weak_ptr<GameObject> _object;

private:
	friend class StateManager;
	[[nodiscard]] virtual int handleInput() = 0;
};

class StateManager
{
public:
	explicit StateManager() = default;

	__forceinline void ChangeState(int key)
	{
		if (_stateMap.find(key) != _stateMap.end())
		{
			_currentState = _stateMap[key];
			_currentKey = key;
		}
	};

	__forceinline void AddState(int key, std::shared_ptr<GameObjectState> state)
	{
		if (_stateMap.find(key) == _stateMap.end())
		{
			_stateMap.emplace(key, state);
		}
	};

	void UpdateState()
	{
		if (nullptr == _currentState)
			return;

		int statekey = _currentState->handleInput();
		if (_currentKey != statekey)
		{
			ChangeState(statekey);
		}
	};

	int GetState() { return _currentKey; }

private:
	std::map<int, std::shared_ptr<GameObjectState>> _stateMap;
	std::shared_ptr<GameObjectState> _currentState = nullptr;
	int _currentKey = -1;

};