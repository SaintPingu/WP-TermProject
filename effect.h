#pragma once
#include "image.h"

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
	EffectImage explode_water;

	std::vector<Effect> effects;
public:
	EffectManager();
	void Paint(HDC hdc) const;
	void CreateEffect(POINT pos, Type type);
	void Animate();
};