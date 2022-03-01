#pragma once

class GameObject;


class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	std::shared_ptr<class Camera> GetMainCamera();

	void Render();

	void ClearRTV();

	void RenderShadow();
	void RenderDeferred();
	void RenderLights();
	void RenderFinal();

	void RenderForward();

private:
	void PushLightData();

public:
	void AddGameObject(std::shared_ptr<GameObject> gameObject);
	void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

	const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }

private:
	std::vector<std::shared_ptr<GameObject>>		_gameObjects;
	std::vector<std::shared_ptr<class Camera>>	_cameras;
	std::vector<std::shared_ptr<class Light>>		_lights;
};

