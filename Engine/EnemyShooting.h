#pragma once
class EnemyShooting
{
public:
	virtual void Shoot() = 0;
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
