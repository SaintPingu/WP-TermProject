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

	Player* player = nullptr;
	Effect* skillEffect = nullptr;
	Skill crntSkill = Skill::Empty;
	Type type = Type::Empty;

	EffectImage imgSkill_Elec_Q;
	EffectImage imgSkill_Fire_Q;
	EffectImage imgSkill_Water_Q;

	RECT GetRectBody() const;
public:
	SkillManager(Player* player, Type type);

	void UseSkill(Skill skill);
	void Paint(HDC hdc) const;
	void Animate();

	inline void ActiveSkill(Skill skill)
	{
		this->crntSkill = skill;
	}
};