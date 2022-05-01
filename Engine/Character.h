#pragma once
class Character
{
public:
	void GetDamage(int damage) { _hp -= damage; }
	virtual void Move() = 0;
	virtual void Attack() = 0;

public:
	int _hp;
	float _speed;
	int _damage;

};