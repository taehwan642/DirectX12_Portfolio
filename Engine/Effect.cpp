#include "pch.h"
#include "Effect.h"
#include "TextureAnimator.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Scene.h"

Effect::~Effect()
{
}

void Effect::Init(const std::wstring& path)
{
	// 애니메이션 불러오기
	_animator->LoadAnimation("Explosion3");
	_animator->SetLoop(false);
}

void Effect::LateUpdate()
{
	if (_camera.lock() != nullptr)
	{
		// Spawn Effect
		GetTransform()->SetWorldRotation(_camera.lock()->GetTransform()->GetWorldRotation());
	}

	_animator->UpdateAnimation();
	// Meshrenderer받아서
	// texture 꽂기
	GetMeshRenderer()->GetMaterial()->SetTexture(0, _animator->GetCurrentAnimationTexture());

	if (_animator->GetAnimationEnd() == true)
	{
		GetGameObject()->SetActive(false);
	}
}

void Effect::Spawn(const std::string& path, const Vec3& worldPosition)
{
	if (_animator == nullptr)
		_animator = std::make_shared<TextureAnimator>();

	if (path != _animationTag)
	{
		_animationTag = path;
		_animator->LoadAnimation(path);
	}

	_animator->SetCurrentIndex(0);
	_animator->SetLoop(false);
	_animator->SetAnimationEnd(false);
	GetMeshRenderer()->GetMaterial()->SetTexture(0, _animator->GetTexture(0));

	GetTransform()->SetWorldPosition(worldPosition);
	GetGameObject()->SetActive(true);
}

#ifdef TOOL
void Effect::DragAndDrop(size_t hash)
{
}
#endif