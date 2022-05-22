#pragma once
#include "image.h"

class BulletController : public ISprite {
private:
	class Bullet {
	private:
		Dir dir = Dir::Empty;
		RECT rectBody = { 0, };
		RECT rectImage;
		POINT posCenter = { 0, };
	public:
		Bullet(POINT center, POINT bulletSize, Dir dir, RECT rectImage);
		void Paint(HDC hdc, const ObjectImage& bulletImage) const;
		bool Move(const RECT& rectWindow);

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

	std::vector<Bullet> bullets;
	ObjectImage bulletImage;
	POINT bulletSize = { 0, };
	int damage = 0;

	const RECT* rectWindow = nullptr;

	RECT GetRectImage(Dir dir) const;
	void SetRectImage(int frame);
public:
	BulletController(const RECT& rectWindow, const ObjectImage& bulletImage);

	void Paint(HDC hdc) const;

	void CreateBullet(POINT center, Dir dir);

	void Move();
};