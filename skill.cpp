#include "stdafx.h"
#include "skill.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "bullet.h"
#include "boss.h"
#include "interface.h"

extern GameData gameData;
extern Player* player;
extern Boss* boss;
extern EnemyController* enemies;
extern EffectManager* effects;

SkillManager::Effect::Effect(const EffectImage& imgSkill, Type type)
{
	this->imgSkill = &imgSkill;
	this->type = type;
}
void SkillManager::Effect::Paint(HDC hdc, const RECT& rectBody) const
{
	RECT rectImage;
	if (type == Type::Water)
	{
		rectImage = ISprite::GetRectImage(*imgSkill, 0);
	}
	else
	{
		rectImage = ISprite::GetRectImage(*imgSkill, frame);
	}
	imgSkill->Paint(hdc, rectBody, &rectImage);
}
bool SkillManager::Effect::Animate()
{
	static int frameLoopCount = 4;

	if (++frame >= imgSkill->GetMaxFrame())
	{
		frame = 0;
		frameLoopCount = 4;
		return false;
	}

	switch (type)
	{
	case Type::Elec:
	{
		if (frame >= 8)
		{
			if (frameLoopCount-- > 0)
			{
				frame = 4;
				return true;
			}
		}
	}
	break;
	default:
		break;
	}

	

	return true;
}








SkillManager::SkillManager()
{
	Type type = player->GetType();
	switch (type)
	{
	case Type::Elec:
		imgSkill_Elec_Q.Load(_T("images\\sprite_skill_elec.png"), { 34,226 }, 10, 0xdf);
		skillEffect = new Effect(imgSkill_Elec_Q, type);
		break;
	case Type::Fire:
		imgSkill_Fire_Q.Load(_T("images\\sprite_skill_fire.png"), { 80,96 }, 50);
		skillEffect = new Effect(imgSkill_Fire_Q, type);
		break;
	case Type::Water:
		imgSkill_Water_Q.Load(_T("images\\skill_water.png"), { 273,843 }, 50, 0xaf);
		skillEffect = new Effect(imgSkill_Water_Q, type);
		break;
	}
}

RECT SkillManager::GetRectBody() const
{
	RECT rectBody = { 0, };
	switch (player->GetType())
	{
	case Type::Elec:
		rectBody = player->GetRectBody();
		rectBody.left -= 20;
		rectBody.right += 20;
		rectBody.bottom = rectBody.top;
		rectBody.top = rectBody.bottom - WINDOWSIZE_Y;
		break;
	case Type::Fire:
		rectBody = player->GetRectBody();
		rectBody.left -= 40;
		rectBody.right += 30;
		rectBody.bottom = rectBody.top + 10;
		rectBody.top = rectBody.bottom - 400;
		break;
	case Type::Water:
	{
		static const int maxFrame = imgSkill_Water_Q.GetMaxFrame();
		int frame = skillEffect->GetFrame();
		rectBody.left = 0;
		rectBody.right = WINDOWSIZE_X;
		rectBody.top = WINDOWSIZE_Y - (((float)frame / maxFrame) * WINDOWSIZE_Y * 2);
		rectBody.bottom = rectBody.top + WINDOWSIZE_Y;
	}
	break;
	default:
		assert(0);
		break;
	}

	return rectBody;
}

void SkillManager::UseSkill()
{
	if (IsUsingSkill() == false)
	{
		return;
	}

	switch (crntSkill)
	{
	case Skill::Sector:
		ShotBySector();
		break;
	case Skill::Circle:
		ShotByCircle();
		break;
	default:
		assert(0);
		break;
	}

	if (--skillCount <= 0)
	{
		skillCount = 0;
		crntSkill = Skill::Empty;
	}
}

void SkillManager::Paint(HDC hdc) const
{
	if (isIdentity == false)
	{
		return;
	}

	RECT rectBody = SkillManager::GetRectBody();
	skillEffect->Paint(hdc, rectBody);
}

void SkillManager::Animate()
{
	if (isIdentity == true)
	{
		if (skillEffect->Animate() == false)
		{
			isIdentity = false;
			return;
		}

		RECT rectBody = GetRectBody();
		rectBody.top += 20;
		const Type playerType = player->GetType();
		if (playerType == Type::Fire)
		{
			if (skillEffect->GetFrame() < 17)
			{
				rectBody.left += (rectBody.right - rectBody.left) / 2;
				rectBody.top += 100;
			}
			else if (skillEffect->GetFrame() > 27)
			{
				rectBody.right -= (rectBody.right - rectBody.left) / 2;
			}
		}

		const float damage = player->GetDamage_Q();
		for (int i = 0; i < 5; ++i)
		{
			if (boss->CheckHit(rectBody, damage, playerType) == false)
			{
				break;
			}
		}
		enemies->CheckHitAll(rectBody, damage, playerType);
		if (playerType == Type::Fire ||
			playerType == Type::Elec)
		{
			enemies->DestroyCollideBullet(rectBody);
		}
	}
}

void SkillManager::ActiveSkill(Skill skill)
{
	if (skill == Skill::Identity)
	{
		if (isIdentity == true)
		{
			return;
		}
	}
	else if (IsUsingSkill() == true)
	{
		return;
	}

	switch (skill)
	{
	case Skill::Sector:
		if (player->ReduceMP(15) == false)
		{
			return;
		}
		skillCount = 7;
		crntSkill = skill;
		break;
	case Skill::Circle:
		if (player->ReduceMP(10) == false)
		{
			return;
		}
		skillCount = 10;
		crntSkill = skill;
		break;
	case Skill::Identity:
		if (player->ReduceMP(30) == false)
		{
			return;
		}
		isIdentity = true;
		break;
	default:
		assert(0);
		break;
	}
}

void SkillManager::ShotBySector()
{
	constexpr int bulletCount = 12;

	BulletData bulletData;
	bulletData.bulletType = player->GetSubType();
	bulletData.damage = player->GetDamage_WE();
	bulletData.speed = 10;

	const RECT rectBody = player->GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.top;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	Vector2 unitVector = Vector2::Up();
	const float startDegree = 10 + (skillCount * 10);
	const float rotationDegree = -(startDegree * 2) / bulletCount;
	unitVector = Rotate(unitVector, startDegree);
	for (int i = 0; i < bulletCount + 1; ++i)
	{
		player->CreateSubBullet(bulletPos, bulletData, unitVector, true, true);
		unitVector = Rotate(unitVector, rotationDegree);
	}
}
void SkillManager::ShotByCircle()
{
	constexpr int bulletCount = 18;

	BulletData bulletData;
	bulletData.bulletType = player->GetSubType();
	bulletData.damage = player->GetDamage_WE();
	bulletData.speed = 10;

	const POINT bulletPos = player->GetPosCenter();

	Vector2 unitVector = Vector2::Up();
	unitVector = Rotate(unitVector, skillCount * 6); // Make different degree each fire
	for (int i = 0; i < bulletCount; ++i)
	{
		player->CreateSubBullet(bulletPos, bulletData, unitVector, true, true);
		unitVector = Rotate(unitVector, 360 / bulletCount);
	}
}