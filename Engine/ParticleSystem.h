#pragma once
#include "Component.h"

class Material;
class Mesh;
class StructuredBuffer;
class ImGuiManager;
class TextureAnimator;

struct ParticleInfo
{
	Vec3	worldPos;
	float	curTime;
	Vec3	worldDir;
	float	lifeTime;
	int32	alive;
	int32	padding[3];
};

struct ComputeSharedInfo
{
	int32 addCount;
	int32 padding[3];
};

enum class ParticleMode
{
	RandomPos_RandomDir,
	RandomPos_SetDir,
	ZeroPos_ZeroDir
};

class ParticleSystem : public Component
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

public:
	virtual void FinalUpdate();
	void Render();

	void SpawnParticle(Vec3 worldPosition);

public:
	virtual void Load(const std::wstring& path) override { }
	virtual void Save(const std::wstring& path) override { }

private:
	friend class ImGuiManager;
	friend class RTTRParticleSystemValue;
	friend class JsonManager;

	std::shared_ptr<StructuredBuffer>	_particleBuffer;
	std::shared_ptr<StructuredBuffer>	_computeSharedBuffer;
	uint32							_maxParticle = 1000;

	std::shared_ptr<Material>		_computeMaterial;
	std::shared_ptr<Material>		_material;
	std::shared_ptr<Mesh>			_mesh;

	bool _addParticle = false;

	float				_createInterval = 0.005f;
	float				_accTime = 0.f;

	float				_minLifeTime = 0.5f;
	float				_maxLifeTime = 1.f;
	float				_minSpeed = 100;
	float				_maxSpeed = 50;
	float				_startScale = 10.f;
	float				_endScale = 5.f;

	std::shared_ptr<TextureAnimator> _textureAnimator;

	ParticleMode _mode = ParticleMode::RandomPos_RandomDir;
	Vec3 _ranges = Vec3(30, 30, 30);
	Vec3 _direction = Vec3(0, 0, -1);
	Vec3 _worldPosition = Vec3(0, 0, 0);

	RTTR_REGISTRATION_FRIEND
};
