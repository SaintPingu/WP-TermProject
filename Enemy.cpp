#include "enemy.h"

Enemy::Enemy(const Player& player, ObjectImage image, double scaleX, double scaleY, POINT pos) : GameObject(image, scaleX, scaleY, pos)
{
	this->player = &player;
}

void Enemy::SetPosDest()
{
	POINT posCenter = GetPosCenter();
	POINT vector = posCenter - player->GetPosCenter();
	POINT vectorOrigin = vector;

	constexpr int moveAmount = 3;
	int moveX = 0;
	int moveY = 0;

	if (abs(vector.y) > abs(vector.x))
	{
		if (vector.y > 0)
		{
			moveY = moveAmount;
		}
		else
		{
			moveY = -moveAmount;
		}
	}
	else
	{
		if (vector.x > 0)
		{
			moveX = moveAmount;
		}
		else
		{
			moveX = -moveAmount;
		}
	}

	const double theta = atan2(vector.y, vector.x);	// 절대 각도를 구한다.
	double radius = GetSqrt(vector.x, vector.y);

	vector.x += moveX;
	vector.y += moveY;

	radius = GetSqrt(vector.x, vector.y);
	vector.y = (LONG)(radius * sin(theta));
	vector.x = (LONG)(radius * cos(theta));

	posDst = posCenter + (vectorOrigin - vector);
}

void Enemy::Move()
{
	SetPosDest();

	SetPos(posDst);
}
void Enemy::Stop()
{
	
}

void Enemy::Animate()
{

}