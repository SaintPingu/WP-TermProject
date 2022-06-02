#pragma once
#include "object.h"

class PlayerBullet;
class SkillManager;

typedef struct PlayerData {
	float maxhp = 0;
	float maxmp = 0;
	float hp = 0;
	float mp = 0;
	float speed = 0;

	float damage = 0;
	float damage_Q = 0;
}PlayerData;

class Player : public GameObject, public IControllable, public IAnimatable {
private:
	const RECT* rectDisplay = nullptr;

	PlayerData data;
	PlayerBullet* bullets = nullptr;
	PlayerBullet* subBullets = nullptr;
	Type type = Type::Empty;
	Type subType = Type::Empty;
	Vector2 posDest = { 0, };
	Vector2 vectorMove = { 0, };
	float alpha = 0;

	SkillManager* skillManager = nullptr;
	Skill crntSkill = Skill::Empty;
	int skillCount = 0;

	Pokemon pokemon;
	SubPokemon subPokemon;
	ObjectImage img_subPokemon[3];

	void SetPosDest() override;
	void FireBySector();
	void FireByCircle();
public:
	Player(HWND hWnd, const RECT& rectDisplay, ObjectImage& image, float scaleX, float scaleY, Vector2 pos, PlayerData data);
	void Paint(HDC hdc);
	void PaintSkill(HDC hdc);

	void SetDirection(Dir dir);
	void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) override;
	void Move(HWND hWnd, int timerID) override;
	void Stop(Dir dir) override;
	Vector2 CheckCollideWindow(Vector2 pos) const;

	void Animate() override;
	void Fire();
	void MoveBullets();
	void Hit(float damage, Type hitType);

	void UseSkill(Skill skill);

	inline float GetDamage_Q() const
	{
		return data.damage_Q;
	}
	inline Type GetType() const
	{
		return type;
	}
	inline float GetHP() const
	{
		return data.hp;
	}
	inline float GetMaxHP() const
	{
		return data.maxhp;
	}
	inline float GetMP() const
	{
		return data.mp;
	}
	inline float GetMaxMP() const
	{
		return data.maxmp;
	}
};