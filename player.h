#pragma once
#include "object.h"

struct BulletData;
class PlayerBullet;
class SkillManager;

typedef struct PlayerData {
	Type type = Type::Empty;
	Type subType = Type::Empty;

	float maxhp = 0;
	float maxmp = 0;
	float hp = 0;
	float mp = 0;
	float speed = 0;

	float damage = 0;
	float subDamage = 0;
	float damage_Q = 0; // per sec
	float damage_WE = 0;
}PlayerData;

class Player : public GameObject, public IControllable, public IAnimatable {
private:
	const RECT* rectDisplay = nullptr;

	PlayerData data;
	PlayerBullet* bullets = nullptr;
	PlayerBullet* subBullets = nullptr;
	Vector2 posDest = { 0, };
	Vector2 vectorMove = { 0, };
	float alpha = 0;

	SkillManager* skillManager = nullptr;
	int skillCount = 0;

	Pokemon pokemon = Pokemon::Null;
	SubPokemon subPokemon = SubPokemon::Null;
	ObjectImage img_subPokemon;

	void SetPosDest() override;
public:
	Player(PlayerData data);
	void Init(const RECT& rectDisplay);
	void Paint(HDC hdc);
	void PaintSkill(HDC hdc);

	void SetDirection(Dir dir);
	void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) override;
	void Move(HWND hWnd, int timerID) override;
	void Stop(Dir dir) override;
	Vector2 CheckCollideWindow(Vector2 pos) const;

	void Animate() override;
	void Fire();
	void CreateSubBullet(POINT center, const BulletData& data, Vector2 unitVector, bool isRotateImg);
	void MoveBullets();
	void Hit(float damage, Type hitType);

	void ActiveSkill(Skill skill);


	bool IsUsingSkill() const;

	inline float GetDamage_Q() const
	{
		return data.damage_Q;
	}
	inline float GetDamage_WE() const
	{
		return data.damage_WE;
	}
	inline Type GetType() const
	{
		return data.type;
	}
	inline Type GetSubType() const
	{
		return data.subType;
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
	inline void AddHP(float amount)
	{
		data.hp += amount;
		if (data.hp > data.maxhp)
		{
			data.hp = data.maxhp;
		}
	}
	inline void AddMP(float amount)
	{
		data.mp += amount;
		if (data.mp > data.maxmp)
		{
			data.mp = data.maxmp;
		}
	}
	inline bool ReduceMP(float amount)
	{
		if ((data.mp - amount) < 0)
		{
			return false;
		}
		data.mp -= amount;
		return true;
	}
};