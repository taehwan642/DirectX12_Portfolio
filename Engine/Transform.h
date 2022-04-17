#pragma once
#include "Component.h"

class Transform
{
public:
	const Vec3& GetPosition() { return _position; }
	const Vec3& GetRotation() { return _rotation; }
	const Vec3& GetScale() { return _scale; }

	// TEMP
	float GetBoundingSphereRadius() { return max(max(_scale.x, _scale.y), _scale.z); }

	void UpdateMatrix();
	const Matrix& GetMatrix() 
	{ 
		UpdateMatrix();
		return _matrix;
	}
	void SetMatrix(const Matrix& m) { _matrix = m; }

	Vec3 GetRight() { UpdateMatrix(); return _matrix.Right(); }
	Vec3 GetUp() { UpdateMatrix(); return _matrix.Up(); }
	Vec3 GetLook() { UpdateMatrix(); return _matrix.Backward(); }

	void SetPosition(const Vec3& position) { _position = position; }
	void SetRotation(const Vec3& rotation) { _rotation = rotation; }
	void SetScale(const Vec3& scale) { _scale = scale; }

public:
	void LookAt(const Vec3& dir);

	static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3 DecomposeRotationMatrix(const Matrix& rotation);

public:
	void AddYawRotation(float degree)
	{
		_rotation.y += degree;
	}
	void AddRollRotation(float degree)
	{
		_rotation.z += degree;
	}
	void AddPitchRotation(float degree)
	{
		_rotation.x += degree;
	}

public:
	std::shared_ptr<Transform> Inverse();
	std::shared_ptr<Transform> LocalToWorld(std::shared_ptr<Transform> parentWorldTransform);
	std::shared_ptr<Transform> WorldToLocal(std::shared_ptr<Transform> parentWorldTransform);

private:
	static std::shared_ptr<Transform> Create()
	{
		struct MakeSharedEnabler : public Transform
		{};
		return std::make_shared<MakeSharedEnabler>();
	}

private:
	Transform();
	friend class ImGuiManager;
	friend class TransformComponent;

	// Parent ±‚¡ÿ
	Vec3 _position = {};
	Vec3 _rotation = {};
	Vec3 _scale = { 1.f, 1.f, 1.f };

	Matrix _matrix = {};

	RTTR_REGISTRATION_FRIEND 
};
