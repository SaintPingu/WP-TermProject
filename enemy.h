#pragma once
#include "object.h"

struct BulletData;
class Player;
class EnemyBullet;

typedef struct EnemyData {
	Type type = Type::Empty;
	int frameNum_Idle = 0;
	int frameNum_IdleMax = 0;
	int frameNum_Atk = 0;
	int frameNum_AtkMax = 0;
	int frameNum_AtkRev = 0;
	
	int atkDelay = 0;
	int crnt_atkDelay = 0;

	int hp = 0;
	float speed = 0;
	float damage = 0;

	int maxYPos = 0;
}EnemyData;

class Enemy abstract : public GameObject, public IAnimatable, public IMovable {
protected:
	Vector2 posDest = { 0, };
	Vector2 unitVector = { 0, };

	Dir GetDir() const;
	virtual void SetPosDest() abstract override;
	void ResetAtkDelay();
	bool IsAtkDelayClear();
	EnemyData data;
	void Paint(HDC hdc, int spriteRow);
public:
	Enemy(ObjectImage& image, float scaleX, float scaleY, Vector2 pos, EnemyData data);
	virtual void Paint(HDC hdc) abstract;
	virtual void Move() override;
	virtual void CheckAtkDelay() abstract;

	int GetSpriteRow();
	void Animate() override;
	bool GetDamage(int damage);

	inline Type GetType() const
	{
		return data.type;
	}
};

class Melee : public Enemy {
private:
	void SetPosDest();
	bool CheckCollidePlayer();
public:
	Melee(ObjectImage& image, float scaleX, float scaleY, Vector2 pos, EnemyData data) : Enemy(image, scaleX, scaleY, pos, data) {};
	void Paint(HDC hdc) override;
	void Move() override;
	void CheckAtkDelay() override;
};

class Range : public Enemy {
private:
	const ObjectImage* bulletImage = nullptr;

	void SetPosDest();
	void Fire();
public:
	Range(ObjectImage& image, float scaleX, float scaleY, Vector2 pos, EnemyData data, const ObjectImage& bulletImage) : Enemy(image, scaleX, scaleY, pos, data) {};
	void Paint(HDC hdc) override;
	void Move() override;
	void CheckAtkDelay() override;
};

class EnemyController {
private:
	std::vector<Enemy*> enemies;

	EnemyBullet* bullets = nullptr;

	const int createDelay_Melee = 2000;
	const int createDelay_Range = 2500;
	int delay_Melee = 0;
	int delay_Range = 0;

	ObjectImage image_beedrill;
	ObjectImage image_zapdos;
	ObjectImage image_zapdos_bullet;
public:
	EnemyController(const RECT& rectWindow);
	void CreateMelee();
	void CreateRange();
	void Paint(HDC hdc);
	void Move();
	void Animate();
	bool CheckHit(const RECT& rectSrc, int damage);
	void CheckAtkDelay();
	void CreateBullet(POINT center, const BulletData& data, Vector2 unitVector, bool isRotate = false);
	void CreateBullet(POINT center, const BulletData& data, Dir dir);
	void MoveBullets();
};