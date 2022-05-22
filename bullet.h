#pragma once
#include "image.h"

class BulletController : public ISprite {
private:
	class Bullet {
	private:
		Dir dir = Dir::Empty;
		RECT rectBullet = { 0, };
	public:
		Bullet(POINT center, POINT bulletSize, Dir dir);
		Bullet() {};
		bool Move(const RECT& rectWindow);
		inline Dir GetDir() const
		{
			return dir;
		}
		inline RECT GetRect() const
		{
			return rectBullet;
		}
	};

	std::vector<Bullet> bullets;
	ObjectImage bulletImage;
	POINT bulletSize = { 0, };
	int damage = 0;

	const RECT* rectWindow = nullptr;

	RECT GetRectImage(Dir dir) const;
public:
	BulletController(const RECT& rectWindow, const ObjectImage& bulletImage);

	void Paint(HDC hdc) const;

	void CreateBullet(POINT center, Dir dir);

	void Move();
};