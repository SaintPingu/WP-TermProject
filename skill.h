#pragma once
#include "image.h"

class Player;

class SkillManager {
private:
	class Effect : public ISprite {
	private:
		const EffectImage* imgSkill;
		Type type;
	public:
		Effect(const EffectImage& imgSkill, Type type);
		void Paint(HDC hdc, const RECT& rectBody) const;
		bool Animate();

		inline int GetFrame() const
		{
			return frame;
		}
	};

	Effect* skillEffect = nullptr;
	Skill crntSkill = Skill::Empty;

	int skillCount = 0;

	EffectImage imgSkill_Elec_Q;
	EffectImage imgSkill_Fire_Q;
	EffectImage imgSkill_Water_Q;

	void FireBySector();
	void FireByCircle();

	RECT GetRectBody() const;
public:
	SkillManager();

	void UseSkill();
	void Paint(HDC hdc) const;
	void Animate();

	void ActiveSkill(Skill skill);
	
	inline bool IsUsingSkill() const
	{
		return (crntSkill != Skill::Empty) ? true : false;
	}
};