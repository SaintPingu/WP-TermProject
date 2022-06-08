#include "stdafx.h"
#include "boss.h"
#include "player.h"
#include "bullet.h"
#include "timer.h"
#include "effect.h"
#include "interface.h"
#include "skill.h"
#include "scene.h"

extern GameData gameData;
extern Player* player;
extern EffectManager* effects;
extern SceneManager* sceneManager;

void Boss::SetMove(Vector2 unitVector)
{
	this->unitVector = unitVector;
	StartMove();
}
void Boss::Death()
{
	data.isDeath = true;
}
void Boss::StartAttack()
{
	//act = static_cast<BossAct>(rand() % 7);
	act = BossAct::Skill1;
	if (data.type != Type::Dark)
	{
		SetAction(Action::Attack, data.frameNum_Atk);
	}

	switch (act)
	{
	case BossAct::Skill1:
	case BossAct::Skill2:
		skill->UseSkill();
		return;
	case BossAct::Line:
	{
		int random = rand() % 2;
		if (random != 0)
		{
			SetMove(Vector2::Left());
		}
		else
		{
			SetMove(Vector2::Right());
		}
	}
	break;
	}
	skillCount = maxSkillCount[static_cast<int>(act)];
}
void Boss::Shot()
{
	switch (act)
	{
	case BossAct::Line:
		ShotByLine();
		break;
	case BossAct::Sector:
		ShotBySector();
		break;
	case BossAct::Circle:
		ShotByCircle();
		break;
	case BossAct::Spiral:
		ShotBySpiral();
		break;
	case BossAct::Spread:
		ShotBySpread();
		break;
	default:
		return;
	}

	if (--skillCount <= 0)
	{
		skillCount = 0;
		act = BossAct::Idle;
	}
}
BulletData Boss::GetBulletData()
{
	const int index = static_cast<int>(act);
	if (index >= BOSS_BULLET_LIST)
	{
		assert(0);
	}

	BulletData bulletData;
	bulletData.bulletType = data.type;
	bulletData.damage = data.damage;
	bulletData.speed = data.bulletSpeed[index];

	return bulletData;
}
void Boss::ResetAttackDelay()
{
	const int index = static_cast<int>(act);
	if (index >= BOSS_BULLET_LIST)
	{
		return;
	}
	data.crntAttackDelay = data.attackDelay[index];
}


Boss::Boss()
{
	const RECT rectDisplay = sceneManager->GetRectDisplay();

	ObjectImage imgBullet;
	image = new ObjectImage();
	
	switch (gameData.stage)
	{
	case Stage::Elec:
		image->Load(_T("images\\sprite_boss_elec.png"), { 73,68 }, { 3,7 }, { 69,50 });
		image->ScaleImage(4, 4);
		imgBullet.Load(_T("images\\bullet_boss_elec.png"), { 400,400 });
		imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case Stage::Water:
		image->Load(_T("images\\sprite_boss_water.png"), { 65,41 }, { 2,3 }, { 63,36 });
		image->ScaleImage(4, 4);
		imgBullet.Load(_T("images\\bullet_boss_water.png"), { 400,400 });
		imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case Stage::Fire:
		image->Load(_T("images\\sprite_boss_fire.png"), { 54,44 }, { 6,12 }, { 44,29 });
		image->ScaleImage(6, 6);
		imgBullet.Load(_T("images\\bullet_boss_fire.png"), { 400,400 });
		imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case Stage::Dark:
		image->Load(_T("images\\sprite_boss_dark.png"), { 110,110 }, { 20,30 }, { 67,50 });
		image->ScaleImage(5, 5);
		imgBullet.Load(_T("images\\bullet_boss_dark.png"), { 700,700 });
		imgBullet.ScaleImage(0.03f, 0.03f);
		break;
	default:
		assert(0);
		break;
	}

	bullets = new EnemyBullet(imgBullet);
	maxSkillCount[static_cast<unsigned int>(BossAct::Line)] = INT_MAX;
	maxSkillCount[static_cast<unsigned int>(BossAct::Circle)] = 10;
	maxSkillCount[static_cast<unsigned int>(BossAct::Spiral)] = 360;
	maxSkillCount[static_cast<unsigned int>(BossAct::Sector)] = 15;
	maxSkillCount[static_cast<unsigned int>(BossAct::Spread)] = 720;
}
Boss::~Boss()
{
	if (skill != nullptr)
	{
		delete skill;
	}
	delete image;
	delete bullets;
}

void Boss::Create()
{
	data = GetBossData();
	data.isCreated = true;
	skill = new BossSkillManager();

	Vector2 posCenter = { WINDOWSIZE_X / 2 , -300 };
	GameObject::Init(*image, posCenter);
	SetMove(Vector2::Down());

}
void Boss::SetPosDest()
{
	if (IsMove() == false)
	{
		return;
	}

	const RECT rectDisplay = sceneManager->GetRectDisplay();

	posDest = Vector2::GetDest(GetPosCenter(), unitVector, data.speed);
	if (act == BossAct::Idle)
	{
		const RECT rectBody = GetRectBody();
		const int maxYPos = (rectBody.bottom - rectBody.top) / 2;

		Vector2 unitVector = Vector2::Down();
		
		if (posDest.y > maxYPos)
		{
			StopMove();
			data.speed = 5;
		}
	}
	else if (act == BossAct::Line)
	{
		constexpr int moveCount = 6;
		static int crntMoveCount = moveCount;
		static bool isReturn = false;

		const RECT rectBody = GetRectBody();
		const int minXPos = bullets->GetBulletSize().x * 4;
		const int maxXPos = rectDisplay.right - minXPos;
		const Vector2 posCenter = GetPosCenter();

		if (posCenter.x < minXPos)
		{
			if (unitVector == Vector2::Left() && --crntMoveCount <= 0)
			{
				isReturn = true;
			}
			SetMove(Vector2::Right());
		}
		else if (posCenter.x > maxXPos)
		{
			if (unitVector == Vector2::Right() && --crntMoveCount <= 0)
			{
				isReturn = true;
			}
			SetMove(Vector2::Left());
		}
		else if (isReturn == true)
		{
			crntMoveCount = moveCount;

			constexpr int centerX = WINDOWSIZE_X / 2;
			if ((unitVector == Vector2::Left() && posCenter.x <= centerX) ||
				(unitVector == Vector2::Right() && posCenter.x >= centerX))
			{
				isReturn = false;
				act = BossAct::Idle;
				StopMove();
			}
		}
	}
}

void Boss::Paint(HDC hdc)
{
	if (data.isCreated == false)
	{
		return;
	}

	const RECT rectImage = ISprite::GetRectImage(GetImage(), frame);
	GameObject::Paint(hdc, &rectImage);

	bullets->Paint(hdc);
	skill->Paint(hdc);
}

void Boss::Move()
{
	if (data.isCreated == false)
	{
		return;
	}
	bullets->Move();

	const RECT rectBody = GetRectBody();
	if (player->IsCollide(rectBody) == true)
	{
		player->Hit(data.damage / 10, GetType());
	}

	if (IsMove() == false)
	{
		return;
	}

	SetPosDest();
	SetPos(posDest);
}
void Boss::CheckAttackDelay()
{
	if (data.isCreated == false)
	{
		return;
	}
	else if (act != BossAct::Idle)
	{
		data.crntAttackDelay -= ELAPSE_BATTLE_INVALIDATE;
		if (IsClearAttackDelay() == true)
		{
			Shot();
			ResetAttackDelay();
		}
	}
}
void Boss::CheckActDelay()
{
	if (data.isCreated == false)
	{
		return;
	}
	else if (IsMove() == false && act == BossAct::Idle)
	{
		data.crntActDelay -= ELAPSE_BATTLE_INVALIDATE;
		if (IsClearActDelay() == true)
		{
			StartAttack();
			ResetActDelay();
		}
	}
}

bool Boss::CheckHit(const RECT& rectSrc, float damage, Type hitType, POINT effectPoint)
{
	RECT rectInter = { 0, };
	if (GameObject::IsCollide(rectSrc, &rectInter) == true)
	{
		if (effectPoint.x == -1)
		{
			const int interWidth = rectInter.right - rectInter.left;
			const int interHeight = rectInter.bottom - rectInter.top;
			const int randX = rectInter.left + (rand() % interWidth);
			const int randY = rectInter.top + (rand() % interHeight);

			effectPoint = { randX, randY };
		}
		else
		{
			const RECT rectBody = GameObject::GetRectBody();
			const int maxYPos = rectBody.top + 30;
			if (maxYPos < rectSrc.top)
			{
				const int randHit = rand() % 12;
				if (randHit != 0)
				{
					return false;
				}
			}
		}

		effects->CreateHitEffect(effectPoint, hitType);
		const float calDamage = CalculateDamage(damage, GetType(), hitType);
		if (Hit(calDamage) == true)
		{
			Death();
		}
		return true;
	}

	return false;
}
bool Boss::Hit(float damage)
{
	if ((data.hp -= damage) <= 0)
	{
		return true;
	}

	return false;
}

void Boss::Animate()
{
	if (IsCreated() == false)
	{
		return;
	}
	else if (isRevFrame == true)
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
			if (data.type == Type::Dark)
			{
				frame = data.frameNum_Idle;
			}
			else
			{
				isRevFrame = true;
				--frame;
			}
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
			if (data.type != Type::Dark)
			{
				SetAction(Action::Idle, data.frameNum_Idle);
			}
		}
		break;
	default:
		assert(0);
		break;
	}
}
void Boss::AnimateSkill()
{
	skill->Animate();
}


void Boss::ShotByLine()
{
	constexpr int bulletCount = 7;

	const BulletData bulletData = GetBulletData();

	const RECT rectBody = GetRectBody();
	const int bulletSizeX = bullets->GetBulletSize().x;
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.bottom;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2) + (((bulletCount / 2) * bulletSizeX) / 2);

	const int bulletMoveAmount = bulletSizeX / 2;
	const Vector2 unitVector = Vector2::Down();
	for (int i = 0; i < bulletCount; ++i)
	{
		bullets->CreateBullet(bulletPos, bulletData, unitVector);
		bulletPos.x -= bulletMoveAmount;
	}
}
void Boss::ShotByCircle()
{
	constexpr int bulletCount = 36;

	const BulletData bulletData = GetBulletData();

	const POINT bulletPos = GetPosCenter();

	Vector2 unitVector = Vector2::Up();
	unitVector = Rotate(unitVector, skillCount * 6); // Make different degree each fire
	for (int i = 0; i < bulletCount; ++i)
	{
		bullets->CreateBullet(bulletPos, bulletData, unitVector);
		unitVector = Rotate(unitVector, 360 / bulletCount);
	}
}
void Boss::ShotBySpiral()
{
	const BulletData bulletData = GetBulletData();

	const RECT rectBody = GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.bottom;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	static int rotation = 8;
	static Vector2 unitVector = Vector2::Down();
	if (skillCount == 360)
	{
		unitVector = Vector2::Down();
	}
	if (abs(skillCount % (180 / rotation)) == 0)
	{
		rotation = ((rand() % 3) + 4) * 2;
	}
	unitVector = Rotate(unitVector, rotation);
	bullets->CreateBullet(bulletPos, bulletData, unitVector);
}
void Boss::ShotBySector()
{
	constexpr int bulletCount = 12;

	const BulletData bulletData = GetBulletData();

	const RECT rectBody = GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.bottom;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	Vector2 unitVector = Vector2::Down();
	const float startDegree = 50 + (((skillCount % 4) * 16));
	const int rotationDegree = -(startDegree * 2) / bulletCount;
	unitVector = Rotate(unitVector, startDegree);
	for (int i = 0; i < bulletCount + 1; ++i)
	{
		bullets->CreateBullet(bulletPos, bulletData, unitVector);
		unitVector = Rotate(unitVector, rotationDegree);
	}
}
void Boss::ShotBySpread()
{
	const BulletData bulletData = GetBulletData();

	const RECT rectBody = GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.bottom;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	const int rotation = 30 + (rand() % 300);

	Vector2 unitVector = Rotate(Vector2::Up(), rotation);
	bullets->CreateBullet(bulletPos, bulletData, unitVector);
}

BossData Boss::GetBossData()
{
	BossData bossData;

	bossData.speed = 1;

	bossData.bulletSpeed[static_cast<int>(BossAct::Line)] = 6;
	bossData.bulletSpeed[static_cast<int>(BossAct::Sector)] = 3;
	bossData.bulletSpeed[static_cast<int>(BossAct::Circle)] = 4;
	bossData.bulletSpeed[static_cast<int>(BossAct::Spiral)] = 5;
	bossData.bulletSpeed[static_cast<int>(BossAct::Spread)] = 6;

	bossData.attackDelay[static_cast<int>(BossAct::Line)] = 40;
	bossData.attackDelay[static_cast<int>(BossAct::Sector)] = 250;
	bossData.attackDelay[static_cast<int>(BossAct::Circle)] = 200;
	bossData.attackDelay[static_cast<int>(BossAct::Spiral)] = 10;
	bossData.attackDelay[static_cast<int>(BossAct::Spread)] = 10;

	bossData.frameNum_Idle = 0;
	switch (gameData.stage)
	{
	case Stage::Elec:
		bossData.type = Type::Elec;

		bossData.hp = 5000;
		bossData.damage = 2;
		bossData.damage_skill1 = 4.5f;
		bossData.damage_skill2 = 0.5f;
		bossData.actDelay = 1250;

		bossData.frameNum_IdleMax = 2;
		bossData.frameNum_Atk = 1;
		bossData.frameNum_AtkMax = 5;
		bossData.frameNum_AtkRev = 5;
		break;
	case Stage::Water:
		bossData.type = Type::Water;

		bossData.hp = 5000;
		bossData.damage = 2;
		bossData.damage_skill1 = 4.5f;
		bossData.damage_skill2 = 2.5f;
		bossData.actDelay = 1750;

		bossData.frameNum_IdleMax = 2;
		bossData.frameNum_Atk = 2;
		bossData.frameNum_AtkMax = 3;
		bossData.frameNum_AtkRev = 3;
		break;
	case Stage::Fire:
		bossData.type = Type::Fire;

		bossData.hp = 5000;
		bossData.damage = 2;
		bossData.damage_skill1 = 7.5f;
		bossData.damage_skill2 = 15.0f;
		bossData.actDelay = 1500;

		bossData.frameNum_IdleMax = 1;
		bossData.frameNum_Atk = 1;
		bossData.frameNum_AtkMax = 6;
		bossData.frameNum_AtkRev = 6;
		break;
	case Stage::Dark:
		bossData.type = Type::Dark;

		bossData.hp = 5000;
		bossData.damage = 5;
		bossData.damage_skill1 = 4.5f;
		bossData.damage_skill2 = 2.5f;
		bossData.actDelay = 1500;

		bossData.frameNum_IdleMax = 14;
		bossData.frameNum_Atk = 0;
		bossData.frameNum_AtkMax = 0;
		bossData.frameNum_AtkRev = 0;
		break;
	}
	bossData.crntActDelay = bossData.actDelay;

	return bossData;
}