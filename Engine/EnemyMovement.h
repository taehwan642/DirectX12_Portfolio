#pragma once

class EnemyMovement
{
public:
	virtual Vec3 GetMovmentResult() = 0;
};

class EnemyLerpMovement : public EnemyMovement
{
public:
	Vec3 _startPoint = Vec3::Zero;
	Vec3 _endPoint = Vec3::Zero;
	float _delta = 0;
	EnemyLerpMovement(const Vec3& startPoint, const Vec3& endPoint) : _startPoint(startPoint), _endPoint(endPoint), _delta(0) {}
	virtual Vec3 GetMovmentResult() override;
};

class EnemyCatmullRomMovement : public EnemyMovement
{
public:
	std::array<Vec3, 4> _catmullRomPoint{};
	float _delta = 0;
	EnemyCatmullRomMovement(const std::array<Vec3, 4>& catmullRomPoint) : _catmullRomPoint(catmullRomPoint), _delta(0) {}
	virtual Vec3 GetMovmentResult() override;
};