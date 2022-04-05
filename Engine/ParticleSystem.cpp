#include "pch.h"
#include "ParticleSystem.h"
#include "StructuredBuffer.h"
#include "Mesh.h"
#include "Resources.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "TextureAnimator.h"

ParticleSystem::ParticleSystem() : Component(COMPONENT_TYPE::PARTICLE_SYSTEM)
{
	_particleBuffer = std::make_shared<StructuredBuffer>();
	_particleBuffer->Init(sizeof(ParticleInfo), _maxParticle);

	_computeSharedBuffer = std::make_shared<StructuredBuffer>();
	_computeSharedBuffer->Init(sizeof(ComputeSharedInfo), 1);
	
	_textureAnimator = std::make_shared<TextureAnimator>();
	//_textureAnimator->LoadAnimation(L"BubbleAnimation");
	_textureAnimator->LoadAnimation(L"Bubbles", L"..\\Resources\\Texture\\Explosion2\\013_%d.dds", 29);
	_textureAnimator->SetInterval(0.05f);
	_textureAnimator->SetLoop(true);
	std::shared_ptr<Texture> tex = _textureAnimator->GetTexture(0);

	_mesh = GET_SINGLE(Resources)->LoadPointMesh();
	_material = GET_SINGLE(Resources)->Get<Material>(L"Particle");

	_material->SetTexture(0, tex);

	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeParticle");
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::FinalUpdate()
{
	_accTime += DELTA_TIME;

	int32 add = 0;
	if (_mode == ParticleMode::RandomPos_RandomDir || _mode == ParticleMode::RandomPos_SetDir)
	{
		if (_createInterval < _accTime)
		{
			_accTime = _accTime - _createInterval;
			add = 1;
		}
	}
	else
	{
		// 특정 위치 건네주기
		// textureAnimation이 끝났다면 파티클이 꺼지게 하기 위해서
		// textureAnimation의 interval * Size를 곱해서 그 총량을  minlifetime maxlifetime에 넣어주기.
		_textureAnimator->UpdateAnimation();

		if (_addParticle == true)
		{
			add = 1;
			_addParticle = false;
		}

		_maxLifeTime = _textureAnimator->GetInterval() * _textureAnimator->GetAnimationSize();
	}

	_material->SetTexture(0, _textureAnimator->GetCurrentAnimationTexture());

	_particleBuffer->PushComputeUAVData(UAV_REGISTER::u0);
	_computeSharedBuffer->PushComputeUAVData(UAV_REGISTER::u1);

	_computeMaterial->SetInt(0, static_cast<int>(_mode));
	_computeMaterial->SetInt(1, _maxParticle);
	_computeMaterial->SetInt(2, add);

	_computeMaterial->SetVec2(0, Vec2(DELTA_TIME, _accTime));

	_computeMaterial->SetVec4(0, Vec4(_minLifeTime, _maxLifeTime, _minSpeed, _maxSpeed));
	_computeMaterial->SetVec4(1, Vec4(_ranges.x, _ranges.y, _ranges.z, 0));
	_direction.Normalize();
	_computeMaterial->SetVec4(2, Vec4(_direction.x, _direction.y, _direction.z, 0));
	_computeMaterial->SetVec4(3, Vec4(_worldPosition.x, _worldPosition.y, _worldPosition.z, 0));

	_computeMaterial->Dispatch(1, 1, 1);
}

void ParticleSystem::Render()
{
	GetTransform()->PushData();

	_particleBuffer->PushGraphicsData(SRV_REGISTER::t9);
	_material->SetFloat(0, _startScale);
	_material->SetFloat(1, _endScale);
	_material->PushGraphicsData();

	_mesh->Render(_maxParticle);
}

void ParticleSystem::SpawnParticle(Vec3 worldPosition)
{
	_addParticle = true;
	_worldPosition = worldPosition;
	_textureAnimator->SetCurrentIndex(0);
}
