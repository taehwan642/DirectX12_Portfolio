#include "pch.h"
#include "Animator.h"
#include "Timer.h"
#include "Resources.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "StructuredBuffer.h"
#include "TransformComponent.h"

Animator::Animator() : Component(COMPONENT_TYPE::ANIMATOR)
{
	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeAnimation");
	_boneFinalMatrix = std::make_shared<StructuredBuffer>();
}

Animator::~Animator()
{
}

void Animator::FinalUpdate()
{
	if (_paused == true)
		return;

	_updateTime += DELTA_TIME;

	const AnimClipInfo& animClip = _animClips->at(_clipIndex);
	if (_updateTime >= animClip.duration)
		_updateTime = 0.f;

	const int32 ratio = static_cast<int32>(animClip.frameCount / animClip.duration);
	_frame = static_cast<int32>(_updateTime * ratio);
	_frame = min(_frame, animClip.frameCount - 1);
	_nextFrame = min(_frame + 1, animClip.frameCount - 1);
	int32 nextFrameUpdateTime = (_frame + 1);
	_frameRatio = static_cast<float>(1.f - (nextFrameUpdateTime - (_updateTime * ratio)));
}

void Animator::SetAnimClip(const std::vector<AnimClipInfo>* animClips)
{
	_animClips = animClips;
}

void Animator::PushData()
{
	uint32 boneCount = static_cast<uint32>(_bones->size());
	if (_boneFinalMatrix->GetElementCount() < boneCount)
		_boneFinalMatrix->Init(sizeof(Matrix), boneCount);

	// Compute Shader
	std::shared_ptr<Mesh> mesh = GetGameObject()->GetMeshRenderer()->GetMesh();
	mesh->GetBoneFrameDataBuffer(_clipIndex)->PushComputeSRVData(SRV_REGISTER::t8);
	mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);

	_boneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);

	_computeMaterial->SetInt(0, boneCount);
	_computeMaterial->SetInt(1, _frame);
	_computeMaterial->SetInt(2, _nextFrame);
	_computeMaterial->SetFloat(0, _frameRatio);

	uint32 groupCount = (boneCount / 256) + 1;
	_computeMaterial->Dispatch(groupCount, 1, 1);

	// Graphics Shader
	_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

const Vec3& Animator::GetBonePosition(const std::string boneName)
{
	Vec3 result = Vec3::Zero;

	auto iter = std::find_if(_bones->begin(), _bones->end(), [&](const BoneInfo& info) {return boneName == info.boneName; });
	assert(iter != _bones->end());

	if ((*_animClips)[_clipIndex].keyFrames[std::distance(_bones->begin(), iter)].size() != 0)
	{
		const AnimClipInfo& animClipInfo = (*_animClips)[_clipIndex];
		const std::vector<KeyFrameInfo>& keyFrames = animClipInfo.keyFrames[std::distance(_bones->begin(), iter)];
		const KeyFrameInfo& keyFrame = keyFrames[_frame];

		Matrix matBone = Matrix::CreateScale(keyFrame.scale.x, keyFrame.scale.y, keyFrame.scale.z);
		matBone *= Matrix::CreateFromQuaternion(SimpleMath::Quaternion(
			keyFrame.rotation.x,
			keyFrame.rotation.y,
			keyFrame.rotation.z,
			keyFrame.rotation.w));
		matBone *= Matrix::CreateTranslation(keyFrame.translate.x, keyFrame.translate.y, keyFrame.translate.z);

		// Local -> World
		matBone *= GetGameObject()->GetTransform()->GetWorldMatrix();

		result.x = matBone._41;
		result.y = matBone._42;
		result.z = matBone._43;
	}

	return result;
}

void Animator::Play(uint32 idx)
{
	assert(idx < _animClips->size());
	_clipIndex = idx;
	_updateTime = 0.f;
}