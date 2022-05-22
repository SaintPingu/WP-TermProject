#include "stdafx.h"
#include "bullet.h"
#include "enemy.h"

BulletController::Bullet::Bullet(POINT center, POINT bulletSize, Dir dir)
{
	this->dir = dir;

	switch (dir)
	{
	case Dir::Up:
	case Dir::Down:
	case Dir::LD:
	case Dir::LU:
	case Dir::RU:
	case Dir::RD:
		rectBullet.left = center.x - (bulletSize.x / 2);
		rectBullet.right = rectBullet.left + bulletSize.x;
		rectBullet.top = center.y - (bulletSize.y / 2);
		rectBullet.bottom = rectBullet.top + bulletSize.y;
		break;
	case Dir::Left:
	case Dir::Right:
		rectBullet.left = center.x - (bulletSize.y / 2);
		rectBullet.right = rectBullet.left + bulletSize.y;
		rectBullet.top = center.y - (bulletSize.x / 2);
		rectBullet.bottom = rectBullet.top + bulletSize.x;
		break;
	default:
		assert(0);
		break;
	}
}
bool BulletController::Bullet::Move(const RECT& rectWindow)
{
	constexpr int moveAmount = 10;
	int moveX = 0;
	int moveY = 0;
	switch (dir)
	{
	case Dir::Left:
		moveX = -moveAmount;
		break;
	case Dir::Right:
		moveX = moveAmount;
		break;
	case Dir::Up:
		moveY = -moveAmount;
		break;
	case Dir::Down:
		moveY = moveAmount;
		break;
	case Dir::LU:
		moveX = -moveAmount;
		moveY = -moveAmount;
		break;
	case Dir::LD:
		moveX = -moveAmount;
		moveY = moveAmount;
		break;
	case Dir::RU:
		moveX = moveAmount;
		moveY = -moveAmount;
		break;
	case Dir::RD:
		moveX = moveAmount;
		moveY = moveAmount;
		break;
	default:
		assert(0);
		break;
	}

	rectBullet.left += moveX;
	rectBullet.right += moveX;
	rectBullet.top += moveY;
	rectBullet.bottom += moveY;

	switch (dir)
	{
	case Dir::Left:
	case Dir::LU:
	case Dir::LD:
		if (rectBullet.right < 0)
		{
			return false;
		}
		break;
	case Dir::Right:
	case Dir::RU:
	case Dir::RD:
		if (rectBullet.left > rectWindow.right)
		{
			return false;
		}
		break;
	}

	switch (dir)
	{
	case Dir::Up:
	case Dir::LU:
	case Dir::RU:
		if (rectBullet.bottom < 0)
		{
			return false;
		}
		break;
	case Dir::Down:
	case Dir::LD:
	case Dir::RD:
		if (rectBullet.top > rectWindow.bottom)
		{
			return false;
		}
		break;
	}

	return true;
}



RECT BulletController::GetRectImage(Dir dir) const
{
	RECT rectImage = bulletImage.GetRectImage();
	int frame = 0;
	switch (dir)
	{
	case Dir::Up:
		frame = 0;
		break;
	case Dir::RU:
		frame = 1;
		break;
	case Dir::Right:
		frame = 2;
		break;
	case Dir::RD:
		frame = 3;
		break;
	case Dir::Down:
		frame = 4;
		break;
	case Dir::LD:
		frame = 5;
		break;
	case Dir::Left:
		frame = 6;
		break;
	case Dir::LU:
		frame = 7;
		break;
	default:
		assert(0);
		break;
	}

	return ISprite::GetRectImage(bulletImage, frame);
}



BulletController::BulletController(const RECT& rectWindow, const ObjectImage& bulletImage)
{
	this->rectWindow = &rectWindow;

	this->bulletImage = bulletImage;

	bulletSize = bulletImage.GetBodySize();

	this->damage = 1;
}

void BulletController::Paint(HDC hdc) const
{

	RECT rectImage = { 0, };
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		rectImage = GetRectImage(bullets[i].GetDir());
		RECT rectBullet = bullets[i].GetRect();
		bulletImage.Paint(hdc, rectBullet, &rectImage);
	}
}

void BulletController::CreateBullet(POINT center, Dir dir)
{
	Bullet* bullet = new Bullet(center, bulletSize, dir);
	bullets.emplace_back(*bullet);
}

extern EnemyController* enemies;
void BulletController::Move()
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (enemies->CheckHit(bullets[i].GetRect(), damage) == true ||
			bullets[i].Move(*rectWindow) == false)
		{
			bullets[i--] = bullets.back();
			bullets.pop_back();
		}
	}
}