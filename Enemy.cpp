#include "stdafx.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "timer.h"
#include "interface.h"

extern GameData gameData;
extern Player* player;
Enemy::Enemy(ObjectImage& image, float scaleX, float scaleY, Vector2 pos, EnemyData data) : GameObject(image, scaleX, scaleY, pos)
{
	StartMove();
	this->data = data;
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

void Melee::SetPosDest()
{
	if (IsMove() == false)
	{
		return;
	}

	Vector2 posCenter = GetPosCenter();
	Vector2 vectorToPlayer = posCenter - player->GetPosCenter();

	float radius = GetRadius(vectorToPlayer.x, vectorToPlayer.y);

	unitVector = vectorToPlayer / radius;

	posDest = posCenter - (unitVector * data.speed);
}
void Range::SetPosDest()
{
	if (IsMove() == false)
	{
		return;
	}

	Vector2 posCenter = GetPosCenter();

	unitVector = { 0, -1 };
	posDest = posCenter - (unitVector * data.speed);
	if (posDest.y > data.maxYPos)
	{
		StopMove();
	}
}

void Enemy::ResetAtkDelay()
{
	data.crnt_atkDelay = data.atkDelay;
}
bool Enemy::IsAtkDelayClear()
{
	return (data.crnt_atkDelay <= 0);
}

void Enemy::Paint(HDC hdc, int spriteRow)
{
	RECT rectImage = GetRectImage(GetImage(), frame, spriteRow);
	GameObject::Paint(hdc, &rectImage);
}
void Melee::Paint(HDC hdc)
{
	int spriteRow = GetSpriteRow();
	Enemy::Paint(hdc, spriteRow);
}
void Range::Paint(HDC hdc)
{
	int spriteRow = 0;
	Enemy::Paint(hdc, spriteRow);
}

void Enemy::Move()
{
	if (IsMove() == false)
	{
		return;
	}

	SetPosDest();
	SetPos(posDest);
}
void Melee::Move()
{
	if (IsMove() == false)
	{
		return;
	}
	else if (CheckCollidePlayer() == true)
	{
		player->GetDamage(data.damage);
		return;
	}

	SetPosDest();
	SetPos(posDest);
}
void Range::Move()
{
	if (IsMove() == false)
	{
		return;
	}

	SetPosDest();
	SetPos(posDest);
}

int Enemy::GetSpriteRow()
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

	return spriteRow;
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
		if (frame > data.frameNum_IdleMax)
		{
			isRevFrame = true;
			--frame;
		}
		else if (frame < data.frameNum_Idle)
		{
			isRevFrame = false;
			++frame;
		}
		break;
	case Action::Attack:
		if (frame > data.frameNum_AtkMax)
		{
			isRevFrame = true;
			--frame;
		}
		else if (frame < data.frameNum_AtkRev)
		{
			isRevFrame = false;
			SetAction(Action::Idle, data.frameNum_Idle);
		}
		break;
	}
}

bool Melee::CheckCollidePlayer()
{
	RECT rectBody = GetRectBody();
	if (player->IsCollide(rectBody) == true)
	{
		StopMove();
		SetAction(Action::Attack, data.frameNum_Atk);
		ResetAtkDelay();

		return true;
	}

	return false;
}
bool Enemy::GetDamage(int damage)
{
	if ((data.hp -= damage) <= 0)
	{
		return true;
	}

	return false;
}

void Melee::CheckAtkDelay()
{
	if (IsMove() == false)
	{
		data.crnt_atkDelay -= ELAPSE_MOVE_OBJECT;
		if (data.crnt_atkDelay <= 0)
		{
			StartMove();
		}
	}
}
void Range::CheckAtkDelay()
{
	if (IsMove() == false)
	{
		data.crnt_atkDelay -= ELAPSE_MOVE_OBJECT;
		if (IsAtkDelayClear() == true)
		{
			Fire();
			ResetAtkDelay();
		}
	}
}

extern EnemyController* enemies;
void Range::Fire()
{
	SetAction(Action::Attack, data.frameNum_Atk);

	RECT rectBody = GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);
	bulletPos.y = rectBody.bottom;

	enemies->CreateBullet(bulletPos, Dir::LD, 1);
	enemies->CreateBullet(bulletPos, Dir::RD, 1);
	enemies->CreateBullet(bulletPos, Dir::Down, 1);
}








EnemyController::EnemyController(const RECT& rectWindow)
{
	image_beedrill.Load(L"sprite_beedrill.png", { 33,33 }, { 7,6 }, { 21,22 });
	image_zapdos.Load(L"sprite_zapdos.png", { 58,58 }, { 12,12 }, { 36,46 });
	image_zapdos_bullet.Load(L"bullet_zapdos.png", { 14,14 }, { 0, 0 }, { 12, 12 });
	
	bullets = new EnemyBullet(rectWindow, image_zapdos_bullet);
}

void EnemyController::CreateMelee()
{
	delay_Melee += ELAPSE_INVALIDATE;
	if (delay_Melee < createDelay_Melee)
	{
		return;
	}
	delay_Melee = 0;

	int createCount = 5;
	EnemyData data;

	switch (gameData.stage)
	{
	case Stage::Electric:
		data.hp = 4;
		data.speed = 3;
		data.atkDelay = 1000;
		data.damage = 2;
		data.frameNum_Idle = 0;
		data.frameNum_IdleMax = 2;
		data.frameNum_Atk = 3;
		data.frameNum_AtkMax = 4;
		data.frameNum_AtkRev = 3;
		break;
	}

	for (int i = 0; i < createCount; ++i)
	{
		float xPos = rand() % WINDOWSIZE_X;
		float yPos = -(rand() % 100);
		switch (gameData.stage)
		{
		case Stage::Electric:
			Melee* enemy = new Melee(image_beedrill, 1.5f, 1.5f, { xPos, yPos }, data);
			enemies.emplace_back(enemy);
			break;
		}
	}
}
void EnemyController::CreateRange()
{
	delay_Range += ELAPSE_INVALIDATE;
	if (delay_Range < createDelay_Range)
	{
		return;
	}
	delay_Range = 0;

	int createCount = 3;
	EnemyData data;

	switch (gameData.stage)
	{
	case Stage::Electric:
		data.hp = 2;
		data.speed = 1;
		data.atkDelay = 2000;
		data.damage = 1;
		data.frameNum_Idle = 0;
		data.frameNum_IdleMax = 2;
		data.frameNum_Atk = 3;
		data.frameNum_AtkMax = 4;
		data.frameNum_AtkRev = 4;
		break;
	}

	for (int i = 0; i < createCount; ++i)
	{
		data.maxYPos = (rand() % 200) + 100;
		float xPos = (rand() % (WINDOWSIZE_X - 100)) + 50;
		float yPos = -(rand() % 100);
		switch (gameData.stage)
		{
		case Stage::Electric:
			Range* enemy = new Range(image_zapdos, 1, 1, { xPos, yPos }, data, image_zapdos_bullet);
			enemies.emplace_back(enemy);
			break;
		}
	}
}

void EnemyController::Paint(HDC hdc)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Paint(hdc);
	}
	bullets->Paint(hdc);
}
void EnemyController::Move()
{
	for (Enemy* enemy : enemies)
	{
		enemy->Move();
	}
}
void EnemyController::Animate()
{
	for (Enemy* enemy : enemies)
	{
		enemy->Animate();
	}
}
bool EnemyController::CheckHit(const RECT& rectSrc, int damage)
{
	for (size_t i = 0;i<enemies.size();++i)
	{
		if (enemies.at(i)->IsCollide(rectSrc) == true)
		{
			if (enemies.at(i)->GetDamage(damage) == true)
			{
				enemies[i--] = enemies.back();
				enemies.pop_back();
			}
			return true;
		}
	}

	return false;
}

void EnemyController::CheckAtkDelay()
{
	for (Enemy* enemy : enemies)
	{
		enemy->CheckAtkDelay();
	}
}
void EnemyController::CreateBullet(POINT center, Dir dir, int damage)
{
	bullets->CreateBullet(center, dir, damage, 6, false);
}
void EnemyController::MoveBullets()
{
	bullets->Move();
}