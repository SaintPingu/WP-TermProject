#include "stdafx.h"
#include "effect.h"

EffectManager::EffectManager()
{
	explode_fire.Load(_T("images\\explode_fire.png"), { 56, 56 }, 8);
	explode_fire.ScaleImage(0.7f, 0.7f);
	explode_elec.Load(_T("images\\explode_elec.png"), { 81, 73 }, 8);
	explode_elec.ScaleImage(0.6f, 0.6f);
	explode_water.Load(_T("images\\explode_water.png"), { 72, 64 }, 9);
	explode_water.ScaleImage(0.7f, 0.7f);
}
EffectManager::Effect::Effect(const EffectImage& effectImage, POINT pos)
{
	this->effectImage = &effectImage;
	this->pos = pos;
}

void EffectManager::Paint(HDC hdc) const
{
	for (const Effect& effect : effects)
	{
		effect.Paint(hdc);
	}
}

void EffectManager::CreateEffect(POINT pos, Type type)
{
	switch (type)
	{
	case Type::Fire:
		effects.emplace_back(explode_fire, pos);
		break;
	case Type::Elec:
		effects.emplace_back(explode_elec, pos);
		break;
	case Type::Water:
		effects.emplace_back(explode_water, pos);
		break;
	default:
		assert(0);
		break;
	}
}

void EffectManager::Animate()
{
	for (size_t i = 0; i < effects.size(); ++i)
	{
		if (effects.at(i).Animate() == false)
		{
			effects[i--] = effects.back();
			effects.pop_back();
		}
	}
}


void EffectManager::Effect::Paint(HDC hdc) const
{
	RECT rectImage = ISprite::GetRectImage(*effectImage, frame, 0);
	effectImage->Paint(hdc, pos, &rectImage);
}

bool EffectManager::Effect::Animate()
{
	if (++frame >= effectImage->GetMaxFrame())
	{
		return false;
	}

	return true;
}