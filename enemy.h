#pragma once
#include "object.h"

struct BulletData;
class Player;
class EnemyBullet;

typedef struct EnemyData {
	Type type = Type::Empty;

	int attackDelay = 0;
	int crntAttackDelay = 0;
	float bulletSpeed = 0;

	float hp = 0;
	float speed = 0;
	float damage = 0;

	int maxYPos = 0;
	int frameNum_Idle = 0;
	int frameNum_IdleMax = 0;
	int frameNum_Atk = 0;
	int frameNum_AtkMax = 0;
	int frameNum_AtkRev = 0;
}EnemyData;

class Enemy abstract : public GameObject, public IAnimatable, public IMovable {
protected:
	EnemyData data;
	Vector2 posDest = { 0, };
	Vector2 unitVector = { 0, };

	void Paint(HDC hdc, int spriteRow);
	Dir GetDir() const;
	virtual void SetPosDest() abstract override;

	inline void ResetAttackDelay()
	{
		data.crntAttackDelay = data.attackDelay;
	}
	inline bool IsClearAttackDelay() const
	{
		return (data.crntAttackDelay <= 0);
	}
public:
	Enemy(ObjectImage& image, Vector2 pos, EnemyData data);
	virtual void Paint(HDC hdc) abstract;
	virtual void Move() override;
	virtual void CheckAttackDelay() abstract;

	int GetSpriteRow();
	void Animate() override;
	bool Hit(float damage);

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
	Melee(ObjectImage& image, Vector2 pos, EnemyData data) : Enemy(image, pos, data) {};
	void Paint(HDC hdc) override;
	void Move() override;
	void CheckAttackDelay() override;
};

class Range : public Enemy {
private:
	const ObjectImage* bulletImage = nullptr;

	void SetPosDest();
	void Fire();
public:
	Range(ObjectImage& image, Vector2 pos, EnemyData data) : Enemy(image, pos, data) {};
	void Paint(HDC hdc) override;
	void Move() override;
	void CheckAttackDelay() override;
};

class EnemyController {
private:
	std::vector<Enemy*> enemies;

	EnemyBullet* bullets = nullptr;

	EnemyData meleeData;
	EnemyData rangeData;

	int createDelay_Melee = 0;
	int createDelay_Range = 0;
	int delay_Melee = 0;
	int delay_Range = 0;
	int createAmount_Melee = 0;
	int createAmount_Range = 0;

	ObjectImage imgMelee;
	ObjectImage imgRange;

	void Pop(size_t& index);
public:
	EnemyController();
	~EnemyController();
	void CreateCheckMelee();
	void CreateCheckRange();
	void Paint(HDC hdc);
	void Move();
	void Animate();
	bool CheckHit(const RECT& rectSrc, float damage, Type hitType, POINT effectPoint);
	void CheckHitAll(const RECT& rectSrc, float damage, Type hitType);

	void CreateBullet(POINT center, const BulletData& data, Vector2 unitVector);
	void CreateBullet(POINT center, const BulletData& data, Dir dir);
	void MoveBullets();
	void DestroyCollideBullet(const RECT& rect);

	inline void CheckAttackDelay()
	{
		for (Enemy* enemy : enemies)
		{
			enemy->CheckAttackDelay();
		}
	}
};