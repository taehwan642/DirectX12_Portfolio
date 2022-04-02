#pragma once
#include "Component.h"
#include "Object.h"

class TransformComponent;
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

	std::shared_ptr<TransformComponent> GetTransform();
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

	size_t GetHash() { return _hash; }

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
		return nullptr;
	}

	void GenerateHash()
	{
		// Hash�� �ֱ�
		_hash = std::hash<std::wstring>()(_name);
	}

	bool GetActive() { return _isActive; }
	void SetActive(bool active);

private:
	friend class ImGuiManager;
	friend class RTTRGameObjectValue;
	friend class RTTRMeshRendererValue;
	friend class JsonManager;

	std::array<std::shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	std::vector<std::shared_ptr<MonoBehaviour>> _scripts;

	bool _checkFrustum = true;
	uint8 _layerIndex = 0;
	bool _static = true;

	size_t _hash = 0;

	std::shared_ptr<TransformComponent> _transform;
	std::shared_ptr<MeshRenderer> _meshRenderer;
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Light> _light;
	std::shared_ptr<ParticleSystem> _particleSystem;
	std::shared_ptr<Terrain> _terrain;
	std::shared_ptr<BaseCollider> _baseCollider;
	std::shared_ptr<Animator> _animator;

	bool _isActive = true;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};


