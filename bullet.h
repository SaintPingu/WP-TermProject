#pragma once
#include "image.h"

class BulletController abstract : public ISprite {
protected:
	class Bullet {
	private:
		int damage = 0;
		int speed = 0;
		Dir dir = Dir::Empty;
		RECT rectBody = { 0, };
		RECT rectImage;
		POINT posCenter = { 0, };
	public:
		Bullet(POINT center, POINT bulletSize, Dir dir, RECT rectImage, int damage, int speed);
		void Paint(HDC hdc, const ObjectImage& bulletImage) const;
		bool Move(const RECT& rectWindow);

		inline int GetDamage() const
		{
			return damage;
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
	};

	BulletController(const RECT& rectWindow, const ObjectImage& bulletImage);

	std::vector<Bullet*> bullets;
	ObjectImage bulletImage;
	POINT bulletSize = { 0, };

	const RECT* rectWindow = nullptr;

	RECT GetRectImage(Dir dir) const;
	void SetRectImage(int frame);
public:

	void Paint(HDC hdc) const;

	void CreateBullet(POINT center, Dir dir, int damage, int speed, bool hasFrame = true);

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