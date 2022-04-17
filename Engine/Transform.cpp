#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"

Transform::Transform()
{

}

void Transform::UpdateMatrix()
{
	Matrix matScale = Matrix::CreateScale(_scale);

	SimpleMath::Quaternion q;

	float sp = 0.f, sy = 0.f, sr = 0.f;
	float cp = 0.f, cy = 0.f, cr = 0.f;

	sp = sinf(DegreeToRadian(_rotation.x) * 0.5f);
	cp = cosf(DegreeToRadian(_rotation.x) * 0.5f);

	sy = sinf(DegreeToRadian(_rotation.y) * 0.5f);
	cy = cosf(DegreeToRadian(_rotation.y) * 0.5f);

	sr = sinf(DegreeToRadian(_rotation.z) * 0.5f);
	cr = cosf(DegreeToRadian(_rotation.z) * 0.5f);

	q.w = sy * sp * sr + cy * cp * cr;
	q.x = sy * sr * cp + sp * cy * cr;
	q.y = sy * cp * cr - sp * sr * cy;
	q.z = -sy * sp * cr + sr * cy * cp;

	Matrix matRotation = Matrix::CreateFromQuaternion(q);
	Matrix matTranslation = Matrix::CreateTranslation(_position);

	_matrix = matScale * matRotation * matTranslation;
}

void Transform::LookAt(const Vec3& dir)
{
	Vec3 front = dir;
	front.Normalize();

	Vec3 right = Vec3::Up.Cross(dir);
	if (right == Vec3::Zero)
		right = Vec3::Forward.Cross(dir);

	right.Normalize();

	Vec3 up = front.Cross(right);
	up.Normalize();

	Matrix matrix = XMMatrixIdentity();
	matrix.Right(right);
	matrix.Up(up);
	matrix.Backward(front);

	_rotation = DecomposeRotationMatrix(matrix);
}

bool Transform::CloseEnough(const float& a, const float& b, const float& epsilon)
{
	return (epsilon > std::abs(a - b));
}

Vec3 Transform::DecomposeRotationMatrix(const Matrix& rotation)
{
	Vec4 v[4];
	XMStoreFloat4(&v[0], rotation.Right());
	XMStoreFloat4(&v[1], rotation.Up());
	XMStoreFloat4(&v[2], rotation.Backward());
	XMStoreFloat4(&v[3], rotation.Translation());

	Vec3 ret;
	if (CloseEnough(v[0].z, -1.0f))
	{
		float x = 0;
		float y = XM_PI / 2;
		float z = x + atan2(v[1].x, v[2].x);
		ret = Vec3{ x, y, z };
	}
	else if (CloseEnough(v[0].z, 1.0f))
	{
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2(-v[1].x, -v[2].x);
		ret = Vec3{ x, y, z };
	}
	else
	{
		float y1 = -asin(v[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(v[1].z / cos(y1), v[2].z / cos(y1));
		float x2 = atan2f(v[1].z / cos(y2), v[2].z / cos(y2));

		float z1 = atan2f(v[0].y / cos(y1), v[0].x / cos(y1));
		float z2 = atan2f(v[0].y / cos(y2), v[0].x / cos(y2));

		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			ret = Vec3{ x1, y1, z1 };
		}
		else
		{
			ret = Vec3{ x2, y2, z2 };
		}
	}

	return ret;
}

std::shared_ptr<Transform> Transform::Inverse()
{
	// 로컬 정보만 남기기 위한 트랜스폼 ( 역행렬 )
	Vec3 reciprocalScale = Vec3::Zero;
	if (_scale.x != 0.f) reciprocalScale.x = 1.f / _scale.x;
	if (_scale.y != 0.f) reciprocalScale.y = 1.f / _scale.y;
	if (_scale.z != 0.f) reciprocalScale.z = 1.f / _scale.z;

	std::shared_ptr<Transform> result = Transform::Create();
	result->_rotation = -_rotation;
	result->_scale = reciprocalScale;

	Vec3 positionResult = Vec3::Zero;

	Vec3 AxisX = Vec3(GetRight().x, GetUp().x, GetLook().x);
	Vec3 AxisY = Vec3(GetRight().y, GetUp().y, GetLook().y);
	Vec3 AxisZ = Vec3(GetRight().z, GetUp().z, GetLook().z);

	AxisX.Normalize();
	AxisY.Normalize();
	AxisZ.Normalize();

	Vec3 pos = -_position * result->_scale;

	positionResult.x = AxisX.Dot(pos);
	positionResult.y = AxisY.Dot(pos);
	positionResult.z = AxisZ.Dot(pos);

	result->_position = positionResult;
	return result;
}

std::shared_ptr<Transform> Transform::LocalToWorld(std::shared_ptr<Transform> parentWorldTransform)
{
	// 현재 트랜스폼 정보가 로컬인 경우
	std::shared_ptr<Transform> result = Transform::Create();
	result->_rotation = parentWorldTransform->_rotation + _rotation;
	result->_scale = parentWorldTransform->_scale * _scale;

	Vec3 positionResult = Vec3::Zero;

	Vec3 AxisX = Vec3(parentWorldTransform->GetRight().x, parentWorldTransform->GetUp().x, parentWorldTransform->GetLook().x);
	Vec3 AxisY = Vec3(parentWorldTransform->GetRight().y, parentWorldTransform->GetUp().y, parentWorldTransform->GetLook().y);
	Vec3 AxisZ = Vec3(parentWorldTransform->GetRight().z, parentWorldTransform->GetUp().z, parentWorldTransform->GetLook().z);

	AxisX.Normalize();
	AxisY.Normalize();
	AxisZ.Normalize();

	Vec3 pos = _position;

	positionResult.x = AxisX.Dot(pos);
	positionResult.y = AxisY.Dot(pos);
	positionResult.z = AxisZ.Dot(pos);

	result->_position = positionResult * parentWorldTransform->_scale + parentWorldTransform->_position;
	return result;
}

std::shared_ptr<Transform> Transform::WorldToLocal(std::shared_ptr<Transform> parentWorldTransform)
{
	// 현재 트랜스폼 정보가 월드인 경우
	std::shared_ptr<Transform> invParent = parentWorldTransform->Inverse();

	std::shared_ptr<Transform> result = Transform::Create();
	result->_scale = invParent->GetScale() * _scale;
	result->_rotation = invParent->GetRotation() + _rotation;

	Vec3 positionResult = Vec3::Zero;

	Vec3 AxisX = Vec3(invParent->GetRight().x, invParent->GetUp().x, invParent->GetLook().x);
	Vec3 AxisY = Vec3(invParent->GetRight().y, invParent->GetUp().y, invParent->GetLook().y);
	Vec3 AxisZ = Vec3(invParent->GetRight().z, invParent->GetUp().z, invParent->GetLook().z);

	// 중요한 부분.
	AxisX.Normalize();
	AxisY.Normalize();
	AxisZ.Normalize();

	Vec3 pos = _position * invParent->_scale;

	positionResult.x = AxisX.Dot(pos);
	positionResult.y = AxisY.Dot(pos);
	positionResult.z = AxisZ.Dot(pos);

	result->_position = positionResult + invParent->_position;
	return result;
}