#include "pch.h"
#include "BoneCollider.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Shader.h"
#include "Material.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Visualizer.h"
#include "Animator.h"

BoneCollider::BoneCollider() : BaseCollider(ColliderType::Bone)
{
	
}

BoneCollider::~BoneCollider()
{
}

void BoneCollider::FinalUpdate()
{
	for (int i = 0; i < _boneColliders.size(); ++i)
	{
		BoneColliderInfo& info = _boneColliders[i];
		
		// 현재 애니메이션의 i번째 본의 현재 프레임 속 toRoot
		auto vec = GetMeshRenderer()->GetMesh()->GetAnimClip();
		std::shared_ptr<Animator> animator = GetGameObject()->GetAnimator();
		if ((*vec)[animator->GetCurrentClipIndex()].keyFrames[info.boneIndex].size() != 0)
		{
			const AnimClipInfo& animClipInfo = (*vec)[animator->GetCurrentClipIndex()];
			const std::vector<KeyFrameInfo>& keyFrames = animClipInfo.keyFrames[info.boneIndex];
			const KeyFrameInfo& keyFrame = keyFrames[animator->GetCurrentFrame()];

			Matrix matBone = Matrix::CreateScale(keyFrame.scale.x, keyFrame.scale.y, keyFrame.scale.z);
			matBone *= Matrix::CreateFromQuaternion(SimpleMath::Quaternion(
				keyFrame.rotation.x,
				keyFrame.rotation.y,
				keyFrame.rotation.z,
				keyFrame.rotation.w));
			matBone *= Matrix::CreateTranslation(keyFrame.translate.x, keyFrame.translate.y, keyFrame.translate.z);
			
			// Local -> World
			matBone *= GetTransform()->GetWorldMatrix();

			info.sphere.Center.x = matBone._41;
			info.sphere.Center.y = matBone._42;
			info.sphere.Center.z = matBone._43;
			
			if (_draw)
			{
				_colliderVisualizers[i]->FinalUpdate(Vec3(info.sphere.Center.x, info.sphere.Center.y, info.sphere.Center.z), Vec3(info.sphere.Radius));
			}
		}
	}
}

bool BoneCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	for (int i = 0; i < _boneColliders.size(); ++i)
	{
		BoneColliderInfo& info = _boneColliders[i];

		if (info.sphere.Intersects(rayOrigin, rayDir, OUT distance))
			return true;
	}
	return false;
}

void BoneCollider::AddBoneCollider(const std::string& boneName, float radius)
{
	auto iter = std::find_if(_boneColliders.begin(), _boneColliders.end(), [=](const BoneColliderInfo& info) {return (boneName == info.boneName); });
	if (iter != _boneColliders.end())
		return;
	auto vec = GetMeshRenderer()->GetMesh()->GetBones();
	for (int i = 0; i < vec->size(); ++i)
	{
		if (boneName == (*vec)[i].boneName)
		{
			BoneColliderInfo info;
			info.boneIndex = i;
			_colliderVisualizers.push_back(std::make_shared<Visualizer>());
			info.sphere.Radius = radius;
			info.boneName = boneName;
			_boneColliders.push_back(info);
			break;
		}
	}
}

void BoneCollider::RemoveBoneCollider(const std::string& boneName)
{
	auto iter = std::find_if(_boneColliders.begin(), _boneColliders.end(), [=](const BoneColliderInfo& info) {return (boneName == info.boneName); });
	if (iter != _boneColliders.end())
		_boneColliders.erase(iter);
}

void BoneCollider::SetBoneColliderRadius(const std::string& boneName, float radius)
{
	auto iter = std::find_if(_boneColliders.begin(), _boneColliders.end(), [=](const BoneColliderInfo& info) {return (boneName == info.boneName); });
	if (iter != _boneColliders.end())
	{
		iter->sphere.Radius = radius;
	}
}

void BoneCollider::LoadCollider(const std::string& colliderName)
{
}

void BoneCollider::SaveCollider(const std::string& colliderName)
{
}
