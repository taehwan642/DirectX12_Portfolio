#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

public:
	// Parent 기준
	const Vec3& GetLocalPosition() { return _localPosition; }
	const Vec3& GetLocalRotation() { return _localRotation; }
	const Vec3& GetLocalScale() { return _localScale; }

	// TEMP
	float GetBoundingSphereRadius() { return max(max(_localScale.x, _localScale.y), _localScale.z); }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	Vec3 GetWorldPosition() { return _matWorld.Translation(); }
	Vec3 GetWorldScale() { return Vec3(_matWorld.Right().Length(), _matWorld.Up().Length(), _matWorld.Backward().Length()); }

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; }
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vec3& scale) { _localScale = scale; }

	void LookAt(const Vec3& dir);

	static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3 DecomposeRotationMatrix(const Matrix& rotation);

public:
	void SetParent(std::shared_ptr<Transform> parent) 
	{ 
		// 자신의 옛 부모가 존재한다면
		if (_parent.lock() != nullptr)
		{
			// 그 옛 부모에서 자신을 뺀다.
			auto findIt = 
				std::find_if(_parent.lock()->_childVector.begin(), _parent.lock()->_childVector.end(),
					[&](std::weak_ptr<GameObject> obj) 
					{
						return (GetGameObject() == obj.lock());
					});

			if (findIt != _parent.lock()->_childVector.end())
				_parent.lock()->_childVector.erase(findIt);
		}
		
		// 새 부모를 받는다.
		_parent = parent; 
		parent->_childVector.push_back(GetGameObject());
	}
	std::weak_ptr<Transform> GetParent() { return _parent; }

	std::shared_ptr<GameObject> GetChild(int i) { return _childVector[i].lock(); }

private:
	friend class ImGuiManager;

	// Parent 기준
	Vec3 _localPosition = {};
	Vec3 _localRotation = {};
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal= {};
	Matrix _matWorld = {};

	std::weak_ptr<Transform> _parent;

	std::vector<std::weak_ptr<GameObject>> _childVector;


	/// <summary>
	/// /////////////////////////////////////////

	/// </summary>
//public:
//	FORCEINLINE constexpr Transform() = default;
//	FORCEINLINE constexpr Transform(const Vector3 & InPosition) : Position(InPosition) { }
//	FORCEINLINE constexpr Transform(const Vector3 & InPosition, const Quaternion & InRotation) : Position(InPosition), Rotation(InRotation) { }
//	FORCEINLINE constexpr Transform(const Vector3 & InPosition, const Quaternion & InRotation, const Vector3 & InScale) : Position(InPosition), Rotation(InRotation), Scale(InScale) { }
//	Transform(const Matrix4x4 & InMatrix);
//
//public: // 트랜스폼 설정함수
//	constexpr void SetPosition(const Vector3 & InPosition) { Position = InPosition; }
//	constexpr void AddPosition(const Vector3 & InDeltaPosition) { Position += InDeltaPosition; }
//	void AddYawRotation(float InDegree)
//	{
//		Rotator r = Rotation.ToRotator();
//		r.Yaw += InDegree;
//		r.Clamp();
//		Rotation = Quaternion(r);
//	}
//	void AddRollRotation(float InDegree)
//	{
//		Rotator r = Rotation.ToRotator();
//		r.Roll += InDegree;
//		r.Clamp();
//		Rotation = Quaternion(r);
//	}
//	void AddPitchRotation(float InDegree)
//	{
//		Rotator r = Rotation.ToRotator();
//		r.Pitch += InDegree;
//		r.Clamp();
//		Rotation = Quaternion(r);
//	}
//
//	constexpr void SetRotation(const Rotator & InRotator) { Rotation = Quaternion(InRotator); }
//	void SetRotation(const Matrix3x3 & InMatrix) { Rotation = Quaternion(InMatrix); }
//	constexpr void SetRotation(const Quaternion & InQuaternion) { Rotation = InQuaternion; }
//	constexpr void SetScale(const Vector3 & InScale) { Scale = InScale; }
//
//	FORCEINLINE constexpr Vector3 GetXAxis() const { return Rotation * Vector3::UnitX; }
//	FORCEINLINE constexpr Vector3 GetYAxis() const { return Rotation * Vector3::UnitY; }
//	FORCEINLINE constexpr Vector3 GetZAxis() const { return Rotation * Vector3::UnitZ; }
//	constexpr Matrix4x4 GetMatrix() const;
//
//	FORCEINLINE constexpr Vector3 GetPosition() const { return Position; }
//	FORCEINLINE constexpr Quaternion GetRotation() const { return Rotation; }
//	FORCEINLINE constexpr Vector3 GetScale() const { return Scale; }
//
//	// 트랜스폼 변환
//	FORCEINLINE constexpr Transform Inverse() const;
//	FORCEINLINE constexpr Transform LocalToWorld(const Transform & InParentWorldTransform) const;
//	FORCEINLINE constexpr Transform WorldToLocal(const Transform & InParentWorldTransform) const;
//	FORCEINLINE constexpr Vector3 WorldToLocalVector(const Vector3 & InWorldVector) const;


	RTTR_REGISTRATION_FRIEND 
};
