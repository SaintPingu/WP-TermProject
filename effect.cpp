#include "stdafx.h"
#include "effect.h"

EffectManager::EffectManager()
{
	explode_fire.Load(_T("explode_fire.png"), { 56, 56 }, 8);
	explode_fire.ScaleImage(0.7f, 0.7f);
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

void EffectManager::CreateEffect(int id, POINT pos)
{
	switch (id)
	{
	case EXPLODE_FIRE:
		effects.emplace_back(explode_fire, pos);
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
	RECT rectImage = GetRectImage(*effectImage, frame, 0);
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