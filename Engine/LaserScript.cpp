#include "pch.h"
#include "LaserScript.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Timer.h"
#include "TextureAnimator.h"
#include "Input.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "PathTraceCameraScript.h"

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
	_camera = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"Main_Camera");
	_cameraScript = _camera.lock()->GetComponent<PathTraceCameraScript>();
}

LaserScript::~LaserScript()
{
}

void LaserScript::Spawn(const Vec3& worldPosition)
{
	_hp = 1;
	GetTransform()->SetWorldPosition(worldPosition);
	SetFireTime(5.0f);
	_laserState = LaserState::LOCKON;
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
		_cameraScript->ShakeCamera();
		GetMeshRenderer()->GetMaterial()->SetTexture(0, _animator->GetTexture(1));
		_scrollSpeed = 7.f;

		_aliveTime -= DELTA_TIME;
		if (_aliveTime < 0.f)
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
		// _maxSizeX가 0보다 작다면, 초기 상태이기 때문에 이 때 scale.x를 넣어준다면 그게 최대 X 크기이다.
		if (_maxSizeX < 0)
			_maxSizeX = scale.x;
		scale.x -= DELTA_TIME * _maxSizeX * _endSpeed;
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
