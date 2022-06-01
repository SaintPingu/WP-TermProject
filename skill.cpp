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
	RECT rectImage = ISprite::GetRectImage(*imgSkill, frame);
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
		assert(0);
		break;
	}

	

	return true;
}








SkillManager::SkillManager(Player* player, Type type)
{
	this->player = player;
	this->type = type;

	imgSkill_Elec_Q.Load(_T("images\\sprite_skill_elec.png"), { 34,226 }, 10, 0xdf);
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
	switch (type)
	{
	case Type::Elec:
	{
		skillEffect->Paint(hdc, rectBody);
		break;
	default:
		assert(0);
		break;
	}
		
	}
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
		enemies->CheckHitAll(rectBody, player->GetDamage_Q(), type);
	}
}