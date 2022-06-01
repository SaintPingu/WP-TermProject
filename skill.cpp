#include "stdafx.h"
#include "skill.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"

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
	static int frameLoopCount = 2;

	if (++frame >= imgSkill->GetMaxFrame())
	{
		frameLoopCount = 2;
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








SkillManager::SkillManager(Player* player, Type type)
{
	this->player = player;
	this->type = type;

	switch (type)
	{
	case Type::Elec:
		imgSkill_Elec_Q.Load(_T("images\\sprite_skill_elec.png"), { 34,226 }, 10, 0xdf);
		break;
	case Type::Fire:
		imgSkill_Fire_Q.Load(_T("images\\sprite_skill_fire.png"), { 80,96 }, 50);
		break;
	case Type::Water:
		imgSkill_Water_Q.Load(_T("images\\skill_water.png"), { 273,843 }, 50);
		break;
	}
}

RECT SkillManager::GetRectBody() const
{
	RECT rectBody = { 0, };
	switch (type)
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
		rectBody.top = WINDOWSIZE_Y - (((float)frame / maxFrame) * WINDOWSIZE_Y*2);
		rectBody.bottom = rectBody.top + WINDOWSIZE_Y;
	}
		break;
	default:
		assert(0);
		break;
	}

	return rectBody;
}

void SkillManager::UseSkill(Skill skill)
{
	if (crntSkill != Skill::Empty)
	{
		return;
	}
	crntSkill = skill;

	if (crntSkill == Skill::Identity)
	{
		switch (type)
		{
		case Type::Elec:
			skillEffect = new Effect(imgSkill_Elec_Q, type);
			break;
		case Type::Fire:
			skillEffect = new Effect(imgSkill_Fire_Q, type);
			break;
		case Type::Water:
			skillEffect = new Effect(imgSkill_Water_Q, type);
			break;
		default:
			assert(0);
			break;
		}
	}
	
}

void SkillManager::Paint(HDC hdc) const
{
	if (crntSkill == Skill::Empty)
	{
		return;
	}
	else if (skillEffect == nullptr)
	{
		return;
	}

	RECT rectBody = SkillManager::GetRectBody();
	skillEffect->Paint(hdc, rectBody);
}

void SkillManager::Animate()
{
	if (skillEffect != nullptr)
	{
		if (skillEffect->Animate() == false)
		{
			delete skillEffect;
			skillEffect = nullptr;
			crntSkill = Skill::Empty;
			return;
		}

		RECT rectBody = GetRectBody();
		rectBody.top += 20;
		if (type == Type::Fire)
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
		enemies->CheckHitAll(rectBody, player->GetDamage_Q(), type);
	}
}