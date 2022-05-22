#pragma once
#include "object.h"

class Player;

class Enemy : public GameObject, public IAnimatable, public IMovable {
private:
	Vector2 posDest = { 0, };
	Vector2 unitVector = { 0, };

	int attackDelay = 0;
	int crntDelay = 0;

	int hp = 0;
	float speed = 0;

	Dir GetDir() const;
	void SetPosDest() override;
	void ResetAttackDelay();
	bool IsDelayOver();
public:
	Enemy(ObjectImage& image, float scaleX, float scaleY, Vector2 pos, int hp, float speed, int attackDelay);
	void Paint(HDC hdc);
	void Move() override;
	inline void StopMove() override
	{
		isMove = false;
	}

	void SetRectImage(int frame) override;
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