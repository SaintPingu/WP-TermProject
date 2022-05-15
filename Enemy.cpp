#include "enemy.h"

Enemy::Enemy(const Player& player, ObjectImage image, double scaleX, double scaleY, Vector2 pos) : GameObject(image, scaleX, scaleY, pos)
{
	this->player = &player;
}

void Enemy::SetPosDest()
{
	Vector2 posCenter = GetPosCenter();
	Vector2 vector = posCenter - player->GetPosCenter();
	Vector2 vectorOrigin = vector;

	const double theta = atan2(vector.y, vector.x);	// Get absolute angle
	double radius = GetSqrt(vector.x, vector.y);

	Vector2 unitVector = { 0, }; // Get unit vector
	unitVector.x = vector.x / radius;
	unitVector.y = vector.y / radius;

	constexpr int moveScale = 3;

	posDst.x = posCenter.x - (unitVector.x * moveScale);
	posDst.y = posCenter.y - (unitVector.y * moveScale);
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