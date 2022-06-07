#pragma once
#include "image.h"

enum class EffectType { Empty = 0, Explode_Fire, Explode_Water, Explode_Elec, Cloud_Fire, Cloud_Water, Cloud_Elec };

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
	EffectImage explode_dark;

	EffectImage cloud_fire;
	EffectImage cloud_elec;
	EffectImage cloud_water;
	EffectImage cloud_dark;

	std::vector<Effect> effects;
public:
	EffectManager();
	void Paint(HDC hdc) const;
	void CreateHitEffect(POINT pos, Type type);
	void CreateExplodeEffect(POINT pos, Type type);
	void Animate();
};

void GetRandEffectPoint(POINT& effectPoint);