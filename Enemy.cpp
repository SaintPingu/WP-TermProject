#include "enemy.h"

Enemy::Enemy(const Player& player, ObjectImage image, double scaleX, double scaleY, Vector2 pos) : GameObject(image, scaleX, scaleY, pos)
{
	this->player = &player;
}

void Enemy::SetVectorDest()
{
	Vector2 posCenter = GetPosCenter();
	Vector2 vectorMove = posCenter - player->GetPosCenter();

	const double theta = atan2(vectorMove.y, vectorMove.x);	// atan2 : Get absolute angle
	double radius = GetSqrt(vectorMove.x, vectorMove.y);

	Vector2 unitVector = vectorMove / radius;

	constexpr int moveScale = 3;

	posDest = posCenter - (unitVector * moveScale);
}

void Enemy::Move()
{
	SetVectorDest();

	SetPos(posDest);
}
void Enemy::Stop()
{
	
}

void Enemy::Animate()
{

}