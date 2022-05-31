#pragma once
#include "object.h"

class PlayerBullet;

enum class Skill : int { Empty = 0, Identity, Sector, Circle, Ultimate };
enum class Pokemon : int { Moltres = 0, Articuno, Thunder};
enum class SubPokemon : int { Pikachu = 0, Squirtle, Charmander };

typedef struct PlayerData {
	int hp = 0;
	int speed = 0;
}PlayerData;

class Player : public GameObject, public IControllable, public IAnimatable {
private:
	const RECT* rectWindow = nullptr;

	PlayerData data;
	PlayerBullet* bullets = nullptr;
	PlayerBullet* subBullets = nullptr;
	Type type = Type::Empty;
	Type subType = Type::Empty;
	Vector2 posDest = { 0, };
	Vector2 vectorMove = { 0, };
	float alpha = 0;

	Skill crntSkill = Skill::Empty;
	int skillCount = 0;

	Pokemon pokemon;
	SubPokemon subPokemon;
	ObjectImage img_subPokemon[3];

	void SetPosDest() override;
	void FireBySector();
	void FireByCircle();
public:
	Player(HWND hWnd, const RECT& rectWindow, ObjectImage& image, float scaleX, float scaleY, Vector2 pos, PlayerData data);
	void Paint(HDC hdc);

	void SetDirection(Dir dir);
	void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) override;
	void Move(HWND hWnd, int timerID) override;
	void Stop(Dir dir) override;
	Vector2 CheckCollideWindow(Vector2 pos) const;

	void Animate() override;
	void Fire();
	void MoveBullets();
	void GetDamage(int damage);

	void UseSkill(Skill skill);
};