#pragma once
#include "image.h"

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
	bool isIdentity = false;

	int skillCount = 0;

	EffectImage imgSkill_Elec_Q;
	EffectImage imgSkill_Fire_Q;
	EffectImage imgSkill_Water_Q;

	void ShotBySector();
	void ShotByCircle();

	RECT GetRectBody() const;
public:
	SkillManager();

	void UseSkill();
	void Paint(HDC hdc) const;
	void Animate();

	void ActiveSkill(Skill skill);
	
	inline bool IsUsingSkill() const
	{
		return (crntSkill == Skill::Sector || crntSkill == Skill::Circle) ? true : false;
	}
};


class BossSkillManager {
private:
	class Effect : public ISprite {
	private:
		EffectImage imgSkill;
		Vector2 posCenter = { 0, };
		Vector2 unitVector = Vector2::Down();
		bool isRotated = false;
		float damage = 0;

		RECT GetRectBody() const;
	public:
		Effect(const EffectImage& imgSkill, const Vector2 pos, float damage);
		Effect(const EffectImage& imgSkill, const Vector2 pos, float rotationDegree, float damage);
		Effect(const EffectImage& imgSkill, const Vector2 pos, Vector2 unitVector, float damage);
		void Paint(HDC hdc) const;
		bool Animate();
		inline void IncreaseAlpha(BYTE alpha)
		{
			imgSkill.IncreaseAlpha(alpha);
		}
		inline void Rotate(float degree)
		{
			isRotated = true;
			unitVector = ::Rotate(unitVector, degree);
		}
		inline Vector2 GetUnitVector() const
		{
			return unitVector;
		}
		inline Vector2 GetPosCenter() const
		{
			return posCenter;
		}
		inline float GetDamage() const
		{
			return damage;
		}
	};

	bool isWarning = false;
	float rotationDegree = 0.0f;

	EffectImage imgSkill1;
	EffectImage imgSkill1_Warning;
	EffectImage imgSkill2;
	EffectImage imgSkill2_Warning;

	std::vector<Effect>warningEffects;
	std::vector<Effect>skillEffects;
	void Skill1_Elec();
	void Skill1_Elec_Create();
	void Skill2_Elec();
	void Skill2_Elec_Create();
public:
	BossSkillManager();
	void Paint(HDC hdc);
	void Animate();

	void UseSkill();
};