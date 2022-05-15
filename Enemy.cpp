#include "enemy.h"

Enemy::Enemy(const Player& player, ObjectImage image, double scaleX, double scaleY, Vector2 pos) : GameObject(image, scaleX, scaleY, pos)
{
	this->player = &player;
}

Dir Enemy::GetDir() const
{
	double theta = atan2(unitVector.y, unitVector.x);
	double crntDegree = RADIAN_TO_DEGREE(theta);

	constexpr int unitDegree = 45;
	double degree = (double)unitDegree / 2;
	if (crntDegree > 0)
	{
		if (crntDegree < degree)
		{
			return Dir::Left;
		}
		else if (crntDegree < (degree += unitDegree))
		{
			return Dir::LU;
		}
		else if (crntDegree < (degree += unitDegree))
		{
			return Dir::Up;
		}
		else if (crntDegree < (degree += unitDegree))
		{
			return Dir::RU;
		}
		else
		{
			return Dir::Right;
		}
	}
	else
	{
		degree = -((double)unitDegree / 2);
		if (crntDegree > degree)
		{
			return Dir::Left;
		}
		else if (crntDegree > (degree -= unitDegree))
		{
			return Dir::LD;
		}
		else if (crntDegree > (degree -= unitDegree))
		{
			return Dir::Down;
		}
		else if (crntDegree > (degree -= unitDegree))
		{
			return Dir::RD;
		}
		else
		{
			return Dir::Right;
		}
	}
}

void Enemy::SetPosDest()
{
	Vector2 posCenter = GetPosCenter();
	Vector2 vectorToPlayer = posCenter - player->GetPosCenter();

	double radius = GetRadius(vectorToPlayer.x, vectorToPlayer.y);

	unitVector = vectorToPlayer / radius;

	constexpr int moveScale = 3;

	posDest = posCenter - (unitVector * moveScale);
}

void Enemy::Paint(HDC hdc)
{
	GameObject::Paint(hdc, &rectImage);
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
	if (isRevFrame == true)
	{
		--frame;
	}
	else
	{
		++frame;
	}

	switch (GetAction())
	{
	case Action::Idle:
		if (frame > 2)
		{
			isRevFrame = true;
			--frame;
		}
		else if (frame < 0)
		{
			isRevFrame = false;
			++frame;
		}
		break;
	}

	int spriteRow = 0;
	switch (GetDir())
	{
	case Dir::Up:
		spriteRow = 0;
		break;
	case Dir::Down:
		spriteRow = 1;
		break;
	case Dir::Left:
		spriteRow = 2;
		break;
	case Dir::Right:
		spriteRow = 3;
		break;
	case Dir::LU:
		spriteRow = 4;
		break;
	case Dir::RU:
		spriteRow = 5;
		break;
	case Dir::LD:
		spriteRow = 6;
		break;
	case Dir::RD:
		spriteRow = 7;
		break;
	default:
		assert(0);
		break;
	}

	const ObjectImage& image = GetImage();
	rectImage = GetRectImage(image, frame, spriteRow);
}