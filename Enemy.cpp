#include "stdafx.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "timer.h"
#include "effect.h"
#include "interface.h"

extern GameData gameData;
extern Player* player;
extern EffectManager* effects;

Enemy::Enemy(ObjectImage& image, Vector2 pos, EnemyData data) : GameObject(image, pos)
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
		player->Hit(data.damage, GetType());
		effects->CreateHitEffect(player->GetPosCenter(), GetType());
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
		else if (isRevFrame == true && frame < data.frameNum_AtkRev)
		{
			isRevFrame = false;
			SetAction(Action::Idle, data.frameNum_Idle);
		}
		break;
	default:
		assert(0);
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
bool Enemy::Hit(float damage)
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

	BulletData bulletData;
	bulletData.bulletType = GetType();
	bulletData.damage = data.damage;
	bulletData.speed = data.bulletSpeed;

	Vector2 unitVector = Vector2::Down();
	int randDegree = (rand() % 10) - 5;

	unitVector = Rotate(unitVector, randDegree);
	enemies->CreateBullet(bulletPos, bulletData, unitVector);
	unitVector = Rotate(unitVector, 20);
	enemies->CreateBullet(bulletPos, bulletData, unitVector);
	unitVector = Rotate(unitVector, -40);
	enemies->CreateBullet(bulletPos, bulletData, unitVector);
}






void EnemyController::Pop(size_t& index)
{
	effects->CreateExplodeEffect(enemies.at(index)->GetPosCenter(), enemies.at(index)->GetType());
	enemies[index--] = enemies.back();
	enemies.pop_back();
}
EnemyController::EnemyController()
{
	switch (gameData.stage)
	{
	case Stage::Electric:
		meleeData.type = Type::Elec;
		rangeData.type = Type::Elec;
		imgMelee.Load(L"images\\sprite_beedrill.png", { 33,33 }, { 7,6 }, { 21,22 });
		imgMelee.ScaleImage(1.2f, 1.2f);
		imgRange.Load(L"images\\sprite_zapdos.png", { 58,58 }, { 12,12 }, { 36,46 });
		imgRangeBullet.Load(L"images\\bullet_zapdos.png", { 14,14 });
		imgRangeBullet.ScaleImage(0.9f, 0.9f);
		createDelay_Melee = 1500;
		createDelay_Range = 3000; 
		createAmount_Melee = 6;
		createAmount_Range = 7;

		meleeData.hp = 5;
		meleeData.speed = 1.5f;
		meleeData.atkDelay = 1000;
		meleeData.damage = 2;

		meleeData.frameNum_Idle = 0;
		meleeData.frameNum_IdleMax = 2;
		meleeData.frameNum_Atk = 3;
		meleeData.frameNum_AtkMax = 4;

		rangeData.hp = 4.25f;
		rangeData.speed = 2;
		rangeData.atkDelay = 2000;
		rangeData.damage = 1;

		rangeData.frameNum_Idle = 0;
		rangeData.frameNum_IdleMax = 2;
		rangeData.frameNum_Atk = 3;
		rangeData.frameNum_AtkMax = 4;
		rangeData.frameNum_AtkRev = 4;
		rangeData.bulletSpeed = 4;
		meleeData.frameNum_AtkRev = 3;
		break;
	case Stage::Water:
		meleeData.type = Type::Water;
		rangeData.type = Type::Water;
		imgMelee.Load(L"images\\sprite_wingull.png", { 34,33 }, { 4,6 }, { 28,22 });
		imgMelee.ScaleImage(1.2f, 1.2f);
		imgRange.Load(L"images\\sprite_seadra.png", { 29,31 }, { 3,3 }, { 25,28 });
		imgRange.ScaleImage(1.2f, 1.2f);
		imgRangeBullet.Load(L"images\\bullet_seadra.png", { 14,14 });
		imgRangeBullet.ScaleImage(1.2f, 1.2f);
		createDelay_Melee = 1150;
		createDelay_Range = 3250;
		createAmount_Melee = 8;
		createAmount_Range = 6;

		meleeData.hp = 2.85f;
		meleeData.speed = 2;
		meleeData.atkDelay = 700;
		meleeData.damage = 1.5f;

		meleeData.frameNum_Idle = 0;
		meleeData.frameNum_IdleMax = 2;
		meleeData.frameNum_Atk = 3;
		meleeData.frameNum_AtkMax = 4;
		meleeData.frameNum_AtkRev = 3;

		rangeData.hp = 5;
		rangeData.speed = 0.7f;
		rangeData.atkDelay = 2000;
		rangeData.damage = 1.8f;

		rangeData.frameNum_Idle = 0;
		rangeData.frameNum_IdleMax = 2;
		rangeData.frameNum_Atk = 3;
		rangeData.frameNum_AtkMax = 6;
		rangeData.frameNum_AtkRev = 3;
		rangeData.bulletSpeed = 3;
		break;
	case Stage::Fire:
		meleeData.type = Type::Fire;
		rangeData.type = Type::Fire;
		imgMelee.Load(L"images\\sprite_ledyba.png", { 37,37 }, { 6,6 }, { 27,27 });
		imgMelee.ScaleImage(1.2f, 1.2f);
		imgRange.Load(L"images\\sprite_latias.png", { 44,34 }, { 2,4 }, { 42,29 });
		imgRange.ScaleImage(1.3f, 1.3f);
		imgRangeBullet.Load(L"images\\bullet_latias.png", { 14,14 });
		imgRangeBullet.ScaleImage(0.8f, 0.8f);
		createDelay_Melee = 2000;
		createDelay_Range = 3000;
		createAmount_Melee = 6;
		createAmount_Range = 5;

		meleeData.hp = 7;
		meleeData.speed = 1.65f;
		meleeData.atkDelay = 1250;
		meleeData.damage = 1.2f;

		meleeData.frameNum_Idle = 0;
		meleeData.frameNum_IdleMax = 1;
		meleeData.frameNum_Atk = 2;
		meleeData.frameNum_AtkMax = 5;
		meleeData.frameNum_AtkRev = 5;

		rangeData.hp = 7.25f;
		rangeData.speed = 0.5f;
		rangeData.atkDelay = 1250;
		rangeData.damage = 0.75f;

		rangeData.frameNum_Idle = 0;
		rangeData.frameNum_IdleMax = 0;
		rangeData.frameNum_Atk = 1;
		rangeData.frameNum_AtkMax = 2;
		rangeData.frameNum_AtkRev = 2;
		rangeData.bulletSpeed = 3.5f;
		break;
	default:
		assert(0);
		break;
	}

	const float randHP_Melee = (float)(rand() % 6) / 10;
	const float randHP_Range = (float)(rand() % 6) / 10;
	meleeData.hp += randHP_Melee;
	rangeData.hp += randHP_Range;
}
void EnemyController::Init(const RECT& rectDisplay)
{
	bullets = new EnemyBullet(rectDisplay, imgRangeBullet);
}

void EnemyController::CreateMelee()
{
	delay_Melee += ELAPSE_INVALIDATE;
	if (delay_Melee < createDelay_Melee)
	{
		return;
	}
	delay_Melee = 0;

	for (int i = 0; i < createAmount_Melee; ++i)
	{
		float xPos = rand() % WINDOWSIZE_X;
		float yPos = -(rand() % 100);

		Melee* enemy = new Melee(imgMelee, { xPos, yPos }, meleeData);
		enemies.emplace_back(enemy);
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

	for (int i = 0; i < createAmount_Range; ++i)
	{
		rangeData.maxYPos = (rand() % 100) + 50;
		float xPos = (rand() % (WINDOWSIZE_X - 100)) + 50;
		float yPos = -(rand() % 100);

		Range* enemy = new Range(imgRange, { xPos, yPos }, rangeData);
		enemies.emplace_back(enemy);
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
bool EnemyController::CheckHit(const RECT& rectSrc, float damage, Type hitType, POINT effectPoint)
{
	for (size_t i = 0;i<enemies.size();++i)
	{
		if (enemies.at(i)->IsCollide(rectSrc) == true)
		{
			effects->CreateHitEffect(effectPoint, hitType);
			float calDamage = CalculateDamage(damage, enemies.at(i)->GetType(), hitType);
			if (enemies.at(i)->Hit(calDamage) == true)
			{
				EnemyController::Pop(i);
			}
			return true;
		}
	}

	return false;
}
void EnemyController::CheckHitAll(const RECT& rectSrc, float damage, Type hitType)
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (enemies.at(i)->IsCollide(rectSrc) == true)
		{
			POINT effectPoint = enemies.at(i)->GetPosCenter();
			effectPoint.x += (rand() % 20) - 10;
			effectPoint.y += (rand() % 20) - 10;
			effects->CreateHitEffect(effectPoint, hitType);
			float calDamage = CalculateDamage(damage, enemies.at(i)->GetType(), hitType);
			if (enemies.at(i)->Hit(calDamage) == true)
			{
				EnemyController::Pop(i);
			}
		}
	}
}

void EnemyController::CheckAtkDelay()
{
	for (Enemy* enemy : enemies)
	{
		enemy->CheckAtkDelay();
	}
}
void EnemyController::CreateBullet(POINT center, const BulletData& data, Vector2 unitVector, bool isRotateImg)
{
	bullets->CreateBullet(center, data, unitVector, isRotateImg);
}
void EnemyController::CreateBullet(POINT center, const BulletData& data, Dir dir)
{
	bullets->CreateBullet(center, data, dir);
}
void EnemyController::MoveBullets()
{
	bullets->Move();
}
void EnemyController::DestroyCollideBullet(const RECT& rect)
{
	bullets->DestroyCollideBullet(rect);
}