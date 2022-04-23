#include "pch.h"
#include "LaserScript.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Timer.h"
#include "TextureAnimator.h"
#include "Input.h"
#include "TransformComponent.h"

void LaserScript::Init()
{
	// 애니메이션 불러오기
	if (_animator == nullptr)
	{
		_animator = std::make_shared<TextureAnimator>();
		_animator->LoadAnimation("LaserTextures");
		_animator->SetLoop(false);
	}
	SetFireTime(3.f);
}

LaserScript::~LaserScript()
{
}

void LaserScript::LateUpdate()
{
	// 만약 Lock 상태일 때?
	if (_laserState == LaserState::LOCKON)
	{
		// 그에 따른 텍스처 넘겨주기
		GetMeshRenderer()->GetMaterial()->SetTexture(0, _animator->GetTexture(0));
		_scrollSpeed = 1.f;
	}
	// 만약 Fire 상태일 때?
	else if (_laserState == LaserState::FIRE)
	{
		GetMeshRenderer()->GetMaterial()->SetTexture(0, _animator->GetTexture(1));
		_scrollSpeed = 7.f;

		_fireTime -= DELTA_TIME;
		if (_fireTime < 0.f)
		{
			_laserState = LaserState::END;
		}
	}
	// 언제 끝나나?
	// End는 언제?
	else if (_laserState == LaserState::END)
	{
		GetMeshRenderer()->GetMaterial()->SetTexture(0, _animator->GetTexture(1));
		Vec3 scale = GetTransform()->GetWorldScale();
		scale.x -= DELTA_TIME * 6;
		if (scale.x < 0)
		{
			GetGameObject()->SetActive(false);
		}
		GetTransform()->SetWorldScale(scale);
		_scrollSpeed = 1.f;
	}
	
	static float inputTime = 0;
	inputTime += DELTA_TIME;
	GetMeshRenderer()->GetMaterial()->SetFloat(0, inputTime * _scrollSpeed);
}

void LaserScript::Fire()
{
	_laserState = LaserState::FIRE;
}
