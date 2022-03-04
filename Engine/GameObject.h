#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class Camera;
class Light;
class MonoBehaviour;
class ParticleSystem;
class Terrain;
class BaseCollider;
class Animator;
class ImGuiManager;

class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	std::shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	std::shared_ptr<Transform> GetTransform();
	std::shared_ptr<MeshRenderer> GetMeshRenderer();
	std::shared_ptr<Camera> GetCamera();
	std::shared_ptr<Light> GetLight();
	std::shared_ptr<ParticleSystem> GetParticleSystem();
	std::shared_ptr<Terrain> GetTerrain();
	std::shared_ptr<BaseCollider> GetCollider();
	std::shared_ptr<Animator> GetAnimator();

	void AddComponent(std::shared_ptr<Component> component);

	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }

	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }

	void SetStatic(bool flag) { _static = flag; }
	bool IsStatic() { return _static; }

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		for (auto& iter : _scripts)
		{
			if (std::dynamic_pointer_cast<T>(iter) != nullptr)
			{
				return std::dynamic_pointer_cast<T>(iter);
			}
		}
	}

private:
	friend class ImGuiManager;

	std::array<std::shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	std::vector<std::shared_ptr<MonoBehaviour>> _scripts;

	bool _checkFrustum = true;
	uint8 _layerIndex = 0;
	bool _static = true;
};

