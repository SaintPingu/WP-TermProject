#pragma once
#include "image.h"

typedef struct BulletData {
	Type bulletType = Type::Empty;
	int damage = 0;
	int speed = 0;
}BulletData;

class BulletController abstract : public ISprite {
protected:
	class Bullet {
	private:
		BulletData data;
		bool isRotateImg = false;

		Dir dir = Dir::Empty;
		Vector2 unitVector;

		RECT rectBody = { 0, };
		RECT rectRotBody = { 0, };
		RECT rectImage = { 0, };
		POINT posCenter = { 0, };
		Bullet(POINT center, POINT bulletSize, RECT rectImage, const BulletData& data);
	public:
		Bullet(POINT center, POINT bulletSize, RECT rectImage, const BulletData& data, Dir dir) : Bullet(center, bulletSize, rectImage, data) { this->dir = dir; };
		Bullet(POINT center, POINT bulletSize, RECT rectImage, const BulletData& data, Vector2 unitVector, bool isRotateImg);
		~Bullet() {};

		void Paint(HDC hdc, const ObjectImage& bulletImage, const RECT& rectWindow);
		bool Move(const RECT& rectWindow);

		inline int GetDamage() const
		{
			return data.damage;
		}
		inline Dir GetDir() const
		{
			return dir;
		}
		inline RECT GetRect() const
		{
			return rectBody;
		}
		inline POINT GetPos() const
		{
			return posCenter;
		}
		inline Type GetType() const
		{
			return data.bulletType;
		}
	};

	BulletController(const RECT& rectWindow, const ObjectImage& bulletImage);

	std::vector<Bullet*> bullets;
	ObjectImage bulletImage;
	POINT bulletSize = { 0, };

	const RECT* rectWindow = nullptr;

	RECT GetRectImage(Dir dir) const;
	void SetRectImage(int frame);
public:

	void Paint(HDC hdc);

	void CreateBullet(POINT center, const BulletData& data, Dir dir);
	void CreateBullet(POINT center, const BulletData& data, Vector2 unitVector, bool isRotateImg);

	virtual void Move() abstract;
};

class PlayerBullet : public BulletController {
public:
	PlayerBullet(const RECT& rectWindow, const ObjectImage& bulletImage) : BulletController(rectWindow, bulletImage) {};
	void Move() override;
};
class EnemyBullet : public BulletController {
public:
	EnemyBullet(const RECT& rectWindow, const ObjectImage& bulletImage) : BulletController(rectWindow, bulletImage) {};
	void Move() override;
};