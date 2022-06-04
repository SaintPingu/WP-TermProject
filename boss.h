#pragma once

//typedef struct BossData {
//	Type type = Type::Empty;
//
//	int atkDelay = 0;
//	int crnt_atkDelay = 0;
//	float bulletSpeed = 0;
//
//	float hp = 0;
//	float speed = 0;
//	float damage = 0;
//
//	int maxYPos = 0;
//	int frameNum_Idle = 0;
//	int frameNum_IdleMax = 0;
//	int frameNum_Atk = 0;
//	int frameNum_AtkMax = 0;
//	int frameNum_AtkRev = 0;
//}BossData;
//
//class Boss abstract : public GameObject, public IAnimatable, public IMovable {
//protected:
//	Vector2 posDest = { 0, };
//	Vector2 unitVector = { 0, };
//
//	Dir GetDir() const;
//	virtual void SetPosDest() abstract override;
//	void ResetAtkDelay();
//	bool IsAtkDelayClear();
//	EnemyData data;
//	void Paint(HDC hdc, int spriteRow);
//public:
//	Enemy(ObjectImage& image, Vector2 pos, EnemyData data);
//	virtual void Paint(HDC hdc) abstract;
//	virtual void Move() override;
//	virtual void CheckAtkDelay() abstract;
//
//	int GetSpriteRow();
//	void Animate() override;
//	bool Hit(float damage);
//
//	inline Type GetType() const
//	{
//		return data.type;
//	}
//};