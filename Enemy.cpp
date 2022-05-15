#include "enemy.h"

Enemy::Enemy(const Player& player, ObjectImage image, double scaleX, double scaleY, Vector2 pos) : GameObject(image, scaleX, scaleY, pos)
{
	this->player = &player;
}

void Enemy::SetPosDest()
{
	Vector2 posCenter = GetPosCenter();
	Vector2 vectorToPlayer = posCenter - player->GetPosCenter();

	double radius = GetRadius(vectorToPlayer.x, vectorToPlayer.y);

	Vector2 unitVector = vectorToPlayer / radius;

	constexpr int moveScale = 3;

	posDest = posCenter - (unitVector * moveScale);
}

void Enemy::Move()
{
	SetPosDest();

	SetPos(posDest);
}
void Enemy::Stop()
{
	
}

void Enemy::Animate()
{

}