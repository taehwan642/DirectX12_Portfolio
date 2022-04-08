#include "pch.h"
#include "Camera.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include "Material.h"
#include "Shader.h"
#include "ParticleSystem.h"
#include "InstancingManager.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "Visualizer.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
	_width = static_cast<float>(GEngine->GetWindow().width);
	_height = static_cast<float>(GEngine->GetWindow().height);
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	_matView = GetTransform()->GetWorldMatrix().Invert();

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(_width * _scale, _height * _scale, _near, _far);

	_frustum.FinalUpdate(_matView, _matProjection);
}

void Camera::SortGameObject()
{
	std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const std::vector<std::shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_vecForward.clear();
	_vecDeferred.clear();
	_vecParticle.clear();
	_vecVisualizerMesh.clear();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetActive() == false)
			continue;

		if (gameObject->GetCollider() != nullptr && gameObject->GetCollider()->IsDrawMesh() == true)
		{
			for (int i = 0; i < gameObject->GetCollider()->GetColliderVisualizers().size(); ++i)
			{
				std::shared_ptr<GameObject> gm = std::make_shared<GameObject>();
				gm->AddComponent(gameObject->GetCollider()->GetColliderVisualizers()[i]->_transform);
				gm->AddComponent(gameObject->GetCollider()->GetColliderVisualizers()[i]->_meshRenderer);
				_vecVisualizerMesh.push_back(gm);
			}
		}

		if (gameObject->GetDrawFrustumRadius() == true)
		{
			std::shared_ptr<GameObject> gm = std::make_shared<GameObject>();
			gm->AddComponent(gameObject->GetVisualizer()->_transform);
			gm->AddComponent(gameObject->GetVisualizer()->_meshRenderer);
			_vecVisualizerMesh.push_back(gm);
		}

		if (gameObject->GetMeshRenderer() == nullptr && gameObject->GetParticleSystem() == nullptr)
			continue;

		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetCheckFrustum())
		{
			if (_frustum.ContainsSphere(
				gameObject->GetTransform()->GetWorldPosition(),
				gameObject->GetFrustumRadius()) == false)
			{
				continue;
			}
		}

		if (gameObject->GetMeshRenderer())
		{
			SHADER_TYPE shaderType = gameObject->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
			switch (shaderType)
			{
			case SHADER_TYPE::DEFERRED:
				_vecDeferred.push_back(gameObject);
				break;
			case SHADER_TYPE::FORWARD:
				_vecForward.push_back(gameObject);
				break;
			}
		}
		else
		{
			_vecParticle.push_back(gameObject);
		}
	}
}

void Camera::SortShadowObject()
{
	std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const std::vector<std::shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_vecShadow.clear();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetActive() == false)
			continue;

		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		if (gameObject->IsStatic())
			continue;

		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetCheckFrustum())
		{
			if (_frustum.ContainsSphere(
				gameObject->GetTransform()->GetWorldPosition(),
				gameObject->GetTransform()->GetWorldTransform()->GetBoundingSphereRadius()) == false)
			{
				continue;
			}
		}

		_vecShadow.push_back(gameObject);
	}
}

void Camera::Render_Deferred()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	// Instancing그룹의 Deferred
	// Non-Instancing 그룹의 Deferred로 나눠야함.
	GET_SINGLE(InstancingManager)->Render(_vecDeferred);
	for (auto& iter : _vecVisualizerMesh)
	{
		iter->GetMeshRenderer()->Render();
	}

	std::shared_ptr<GameObject> gm = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> bvo = GET_SINGLE(SceneManager)->_boneVisualizerObject;
	bvo->SetDrawFrustumRadius(true);
	bvo->FinalUpdate();
	gm->AddComponent(bvo->GetVisualizer()->_transform);
	gm->GetTransform()->SetWorldScale(Vec3(5, 5, 5));
	gm->AddComponent(bvo->GetVisualizer()->_meshRenderer);
	gm->GetMeshRenderer()->Render();
}

void Camera::Render_Forward()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	GET_SINGLE(InstancingManager)->Render(_vecForward);

	for (auto& gameObject : _vecParticle)
	{
		gameObject->GetParticleSystem()->Render();
	}
}

void Camera::Render_Shadow()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (auto& gameObject : _vecShadow)
	{
		gameObject->GetMeshRenderer()->RenderShadow();
	}
}