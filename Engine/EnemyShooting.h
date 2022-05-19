#pragma once
class GameObject;
class EnemyShooting
{
public:
	virtual void Shoot() = 0;

	void SetFireSpeed(float fireSpeed) { _fireSpeed = fireSpeed; }
	void SetAttatchedObject(std::shared_ptr<GameObject> object) { _object = object; }

protected:
	float _deltaTime = 0.f;
	float _fireSpeed = 0.3f;

	std::weak_ptr<GameObject> _object;
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
