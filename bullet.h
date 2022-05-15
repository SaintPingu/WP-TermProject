#pragma once
#include <Windows.h>
#include <vector>
#include "object.h"
#include "bullet.h"

#define MAX_BULLET 50

class BulletController : public ISprite {
private:
	class Bullet {
	private:
		Dir dir = Dir::Empty;
		RECT rectBullet = { 0, };
	public:
		void Create(POINT center, POINT bulletSize, Dir dir);
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
	int bulletCount = 0;
	ObjectImage bulletImage;
	POINT bulletSize = { 0, };

	const RECT* rectWindow = nullptr;

	RECT GetRectImage(Dir dir) const;
public:
	BulletController(const RECT& rectWindow, const ObjectImage& bulletImage, int maxBullet = MAX_BULLET);

	void Paint(HDC hdc) const;

	void CreateBullet(POINT center, Dir dir);

	void Move();
};