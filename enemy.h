#pragma once
#include "object.h"

class Player;

typedef struct EnemyData {
	int frameNum_Idle = 0;
	int frameNum_Attack = 3;
	
	int attackDelay = 0;
	int crntDelay = 0;

	int hp = 0;
	int speed = 0;
	int damage = 0;
}EnemyData;

class Enemy : public GameObject, public IAnimatable, public IMovable {
private:
	Vector2 posDest = { 0, };
	Vector2 unitVector = { 0, };

	EnemyData data;

	Dir GetDir() const;
	void SetPosDest() override;
	void ResetAttackDelay();
	bool IsDelayOver();
public:
	Enemy(ObjectImage& image, float scaleX, float scaleY, Vector2 pos, EnemyData data);
	void Paint(HDC hdc);
	void Move() override;
	inline void StopMove() override
	{
		isMove = false;
	}

	int GetSpriteRow();
	void Animate() override;

	bool CheckCollidePlayer();
	bool GetDamage(int damage);
};

class EnemyController {
private:
	std::vector<Enemy> enemies;
	ObjectImage Image_beedrill;
public:
	EnemyController();
	void CreateMelee();
	void Paint(HDC hdc);
	void Move();
	void Animate();
	bool CheckHit(const RECT& rectSrc, int damage);
};