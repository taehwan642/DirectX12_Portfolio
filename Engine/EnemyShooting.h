#pragma once
class EnemyShooting
{
public:
	virtual void Shoot() = 0;

	void SetFireTime(float fireTime) { _fireTime = fireTime; }

private:
	float _deltaTime = 0.f;
	float _fireTime = 0.3f;
};

class EnemyLaserShooting : public EnemyShooting
{
public:
	virtual void Shoot() override;
};

class EnemyRapidShooting : public EnemyShooting
{
public:
	virtual void Shoot() override;
};

class EnemyHeavyShooting : public EnemyShooting
{
public:
	virtual void Shoot() override;
};

class EnemyThreeWayShooting : public EnemyShooting
{
public:
	virtual void Shoot() override;
};
