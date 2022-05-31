#pragma once
#include "image.h"

#define EXPLODE_FIRE 0
#define EXPLODE_ELEC 1

class EffectManager {
private:
	class Effect : public ISprite {
	private:
		const EffectImage* effectImage;
		POINT pos;
	public:
		Effect(const EffectImage& effectImage, POINT pos);
		void Paint(HDC hdc) const;
		bool Animate();
	};

	EffectImage explode_fire;
	EffectImage explode_elec;

	std::vector<Effect> effects;
public:
	EffectManager();
	void Paint(HDC hdc) const;
	void CreateEffect(int id, POINT pos);
	void Animate();
};