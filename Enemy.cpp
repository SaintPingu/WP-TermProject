#include "stdafx.h"
#include "enemy.h"
#include "player.h"
#include "timer.h"
#include "interface.h"

#define FRAME_NUM_IDLE 0
#define FRAME_NUM_ATTACK 3

extern GameData gameData;
extern Player* player;
Enemy::Enemy(ObjectImage& image, float scaleX, float scaleY, Vector2 pos, int hp, float speed, int attackDelay) : GameObject(image, scaleX, scaleY, pos)
{
	isMove = true;
	this->hp = hp;
	this->attackDelay = attackDelay;
	this->speed = speed;
}

Dir Enemy::GetDir() const
{
	float theta = atan2(unitVector.y, unitVector.x);
	float crntDegree = RADIAN_TO_DEGREE(theta);

	constexpr int unitDegree = 45;
	float degree = (float)unitDegree / 2;
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
		degree = -((float)unitDegree / 2);
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

	float radius = GetRadius(vectorToPlayer.x, vectorToPlayer.y);

	unitVector = vectorToPlayer / radius;

	posDest = posCenter - (unitVector * speed);
}

void Enemy::ResetAttackDelay()
{
	crntDelay = attackDelay;
}
bool Enemy::IsDelayOver()
{
	return (crntDelay <= 0);
}

void Enemy::Paint(HDC hdc)
{
	GameObject::Paint(hdc, &rectImage);
}

void Enemy::Move()
{
	if (IsMove() == false)
	{
		crntDelay -= ELAPSE_MOVE_OBJECT;
		if (IsDelayOver() == true)
		{
			isMove = true;
		}
		return;
	}
	else if (CheckCollidePlayer() == true)
	{
		return;
	}

	SetPosDest();

	SetPos(posDest);

}

void Enemy::SetRectImage(int frame)
{
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

	rectImage = GetRectImage(GetImage(), frame, spriteRow);
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
		else if (frame < FRAME_NUM_IDLE)
		{
			isRevFrame = false;
			++frame;
		}
		break;
	case Action::Attack:
		if (frame > FRAME_NUM_ATTACK + 1)
		{
			isRevFrame = true;
			--frame;
		}
		else if (frame < FRAME_NUM_ATTACK)
		{
			isRevFrame = false;
			SetAction(Action::Idle, FRAME_NUM_IDLE);
		}
		break;
	}

	SetRectImage(frame);
}

bool Enemy::CheckCollidePlayer()
{
	RECT rectBody = GetRectBody();
	if (player->IsCollide(rectBody) == true)
	{
		StopMove();
		SetAction(Action::Attack, FRAME_NUM_ATTACK);
		ResetAttackDelay();

		return true;
	}

	return false;
}
bool Enemy::GetDamage(int damage)
{
	if ((hp -= damage) <= 0)
	{
		return true;
	}

	return false;
}








EnemyController::EnemyController()
{
	Image_beedrill.Load(L"sprite_beedrill.png", { 35,35 }, { 7,6 }, { 21,22 });
}

void EnemyController::CreateMelee()
{
	int createCount = 5;
	for (int i = 0; i < createCount; ++i)
	{
		float xPos = rand() % WINDOWSIZE_X;
		float yPos = -(rand() % 100);
		switch (gameData.stage)
		{
		case Stage::Electric:
			Enemy* enemy = new Enemy(Image_beedrill, 1.5f, 1.5f, { xPos, yPos }, 5, 3, 1000);
			enemies.emplace_back(*enemy);
			delete enemy;
			break;
		}
	}
}

void EnemyController::Paint(HDC hdc)
{
	for (Enemy& enemy : enemies)
	{
		enemy.Paint(hdc);
	}
}
void EnemyController::Move()
{
	for (Enemy& enemy : enemies)
	{
		enemy.Move();
	}
}
void EnemyController::Animate()
{
	for (Enemy& enemy : enemies)
	{
		enemy.Animate();
	}
}
bool EnemyController::CheckHit(const RECT& rectSrc, int damage)
{
	for (size_t i = 0;i<enemies.size();++i)
	{
		if (enemies.at(i).IsCollide(rectSrc) == true)
		{
			if (enemies.at(i).GetDamage(damage) == true)
			{
				enemies[i--] = enemies.back();
				enemies.pop_back();
			}
			return true;
		}
	}

	return false;
}
