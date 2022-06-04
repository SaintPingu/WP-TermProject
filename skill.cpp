#include "stdafx.h"
#include "skill.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "bullet.h"

extern Player* player;
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
		FireBySector();
		break;
	case Skill::Circle:
		FireByCircle();
		break;
	case Skill::Identity:
		break;
	default:
		assert(0);
		break;
	}
}

void SkillManager::Paint(HDC hdc) const
{
	if (crntSkill != Skill::Identity)
	{
		return;
	}

	RECT rectBody = SkillManager::GetRectBody();
	skillEffect->Paint(hdc, rectBody);
}

void SkillManager::Animate()
{
	if (crntSkill == Skill::Identity)
	{
		if (skillEffect->Animate() == false)
		{
			crntSkill = Skill::Empty;
			return;
		}

		RECT rectBody = GetRectBody();
		rectBody.top += 20;
		Type playerType = player->GetType();
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
		enemies->CheckHitAll(rectBody, player->GetDamage_Q(), player->GetType());
		if (playerType == Type::Fire ||
			playerType == Type::Elec)
		{
			enemies->DestroyCollideBullet(rectBody);
		}
	}
}

void SkillManager::ActiveSkill(Skill skill)
{
	if (IsUsingSkill() == true)
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
		break;
	case Skill::Circle:
		if (player->ReduceMP(10) == false)
		{
			return;
		}
		skillCount = 10;
		break;
	case Skill::Identity:
		if (player->ReduceMP(30) == false)
		{
			return;
		}
		break;
	default:
		assert(0);
		break;
	}

	crntSkill = skill;
}

void SkillManager::FireBySector()
{
	constexpr int bullets = 12;

	RECT rectBody = player->GetRectBody();
	BulletData bulletData;
	bulletData.bulletType = player->GetSubType();
	bulletData.damage = player->GetDamage_WE();
	bulletData.speed = 10;

	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.top;
	bulletPos.y = rectBody.top;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	Vector2 unitVector = Vector2::Up();
	int startDegree = -10 - (skillCount * 10);
	int rotationDegree = -(startDegree * 2) / bullets;
	unitVector = Rotate(unitVector, startDegree);
	for (int i = 0; i < bullets + 1; ++i)
	{
		player->CreateSubBullet(bulletPos, bulletData, unitVector, true);
		unitVector = Rotate(unitVector, rotationDegree);
	}

	if (--skillCount <= 0)
	{
		skillCount = 0;
		crntSkill = Skill::Empty;
	}
}
void SkillManager::FireByCircle()
{
	POINT bulletPos = player->GetPosCenter();
	BulletData bulletData;
	bulletData.bulletType = player->GetSubType();
	bulletData.damage = player->GetDamage_WE();
	bulletData.speed = 10;

	Vector2 unitVector = Vector2::Up();
	unitVector = Rotate(unitVector, skillCount * 6); // Make different degree each fire
	for (int i = 0; i < 18; ++i)
	{
		player->CreateSubBullet(bulletPos, bulletData, unitVector, true);
		unitVector = Rotate(unitVector, 20);
	}

	if (--skillCount <= 0)
	{
		skillCount = 0;
		crntSkill = Skill::Empty;
	}
}