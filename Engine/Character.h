#pragma once
class Character
{
public:
	// 죽었다면 true, 살아있다면 false
	bool GetDamage(int damage) {
		_hp -= damage;
		_invincibleDeltaTime = _invincibleTime;
		if (_hp <= 0)
			return true;
		return false;
	}
	virtual void Move() = 0;
	virtual void Attack() = 0;
	virtual void Spawn(int hp, float speed, int damage) { _hp = hp; _speed = speed; _damage = damage; };

	bool IsInvincible()
	{
		return (_invincibleDeltaTime > 0);
	}

	void UpdateInvincibleTime();

public:
	int _hp = 1;
	float _speed = 1.f;
	int _damage = 1;
	float _invincibleTime = 0.1f;
	float _invincibleDeltaTime = 0.f;

};

class Bullet : public Character
{
public:
	float _aliveTime = 0.f;
	Vec3 _direction = Vec3::Zero;
};