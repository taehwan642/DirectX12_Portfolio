#pragma once
#include "Component.h"

class Transform
{
public:

public:
	const Vec3& GetPosition() { return _position; }
	const Vec3& GetRotation() { return _rotation; }
	const Vec3& GetScale() { return _scale; }

	// TEMP
	float GetBoundingSphereRadius() { return max(max(_scale.x, _scale.y), _scale.z); }

	void UpdateMatrix()
	{
		Matrix matRotation = Matrix::CreateRotationX(DegreeToRadian(_rotation.x));
		matRotation *= Matrix::CreateRotationY(DegreeToRadian(_rotation.y));
		matRotation *= Matrix::CreateRotationZ(DegreeToRadian(_rotation.z));

		_matrix._11 = matRotation.Right().x * _scale.x;
		_matrix._12 = matRotation.Right().y * _scale.x;
		_matrix._13 = matRotation.Right().z * _scale.x;
		_matrix._14 = 0;
		_matrix._21 = matRotation.Up().x * _scale.y;
		_matrix._22 = matRotation.Up().y * _scale.y;
		_matrix._23 = matRotation.Up().z * _scale.y;
		_matrix._24 = 0;
		_matrix._31 = matRotation.Backward().x * _scale.z;
		_matrix._32 = matRotation.Backward().y * _scale.z;
		_matrix._33 = matRotation.Backward().z * _scale.z;
		_matrix._34 = 0;
		_matrix._41 = _position.x;
		_matrix._42 = _position.y;
		_matrix._43 = _position.z;
		_matrix._44 = 1;

		//_matrix = matScale * matRotation * matTranslation;
	}
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
