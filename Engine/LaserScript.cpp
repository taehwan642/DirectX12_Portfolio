#include "pch.h"
#include "LaserScript.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Timer.h"
#include "TextureAnimator.h"
#include "Input.h"

void LaserScript::Init()
{
	// 애니메이션 불러오기 d
	_animator->LoadAnimation("LaserTextures");
	_animator->SetLoop(false);
}

LaserScript::~LaserScript()
{
}

void LaserScript::LateUpdate()
{
	if(_init == false)
	{
		_init = true;
		Init();
	}
	if (INPUT->GetButtonDown(KEY_TYPE::KEY_1))
	{
		_laserState = (_laserState == LaserState::LOCKON) ? LaserState::FIRE : LaserState::LOCKON;
	}

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
		_scrollSpeed = 2.f;
	}
	// 언제 끝나나?
	// End는 언제?
	else if (_laserState == LaserState::END)
	{

	}
	
	// state에 따른 uv 스크롤 속도 변환
	// 충돌처리 

	static float inputTime = 0;
	inputTime += DELTA_TIME;
	GetMeshRenderer()->GetMaterial()->SetFloat(0, inputTime * _scrollSpeed);
}
