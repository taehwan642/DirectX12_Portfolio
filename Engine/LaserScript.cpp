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
	// �ִϸ��̼� �ҷ����� d
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

	// ���� Lock ������ ��?
	if (_laserState == LaserState::LOCKON)
	{
		// �׿� ���� �ؽ�ó �Ѱ��ֱ�
		GetMeshRenderer()->GetMaterial()->SetTexture(0, _animator->GetTexture(0));
		_scrollSpeed = 1.f;
	}
	// ���� Fire ������ ��?
	else if (_laserState == LaserState::FIRE)
	{
		GetMeshRenderer()->GetMaterial()->SetTexture(0, _animator->GetTexture(1));
		_scrollSpeed = 2.f;
	}
	// ���� ������?
	// End�� ����?
	else if (_laserState == LaserState::END)
	{

	}
	
	// state�� ���� uv ��ũ�� �ӵ� ��ȯ
	// �浹ó�� 

	static float inputTime = 0;
	inputTime += DELTA_TIME;
	GetMeshRenderer()->GetMaterial()->SetFloat(0, inputTime * _scrollSpeed);
}