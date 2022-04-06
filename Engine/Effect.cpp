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
	// �ִϸ��̼� �ҷ�����
	_animator->LoadAnimation("Explosion3");
	_animator->SetLoop(false);
}

void Effect::LateUpdate()
{
	if (_camera != nullptr)
	{
		// Spawn Effect
		GetTransform()->SetWorldRotation(_camera->GetTransform()->GetWorldRotation());
	}

	_animator->UpdateAnimation();
	// Meshrenderer�޾Ƽ�
	// texture �ȱ�
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