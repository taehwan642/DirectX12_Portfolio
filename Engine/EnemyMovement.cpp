#include "pch.h"
#include "EnemyMovement.h"
#include "Timer.h"

Vec3 EnemyLerpMovement::GetMovmentResult()
{
	_delta += DELTA_TIME;
	return Vec3::Lerp(_startPoint, _endPoint, _delta);
}

Vec3 EnemyCatmullRomMovement::GetMovmentResult()
{
	_delta += DELTA_TIME;
	//												�������-----------------������� ������
	return Vec3::CatmullRom(_catmullRomPoint[0], _catmullRomPoint[1], _catmullRomPoint[2], _catmullRomPoint[3], _delta);
}