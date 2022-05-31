#include "stdafx.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"

BulletController::Bullet::Bullet(POINT center, POINT bulletSize, Dir dir, RECT rectImage, int damage, int speed)
{
	this->dir = dir;
	this->rectImage = rectImage;
	this->posCenter = center;
	this->damage = damage;
	this->speed = speed;

	switch (dir)
	{
	case Dir::Up:
	case Dir::Down:
	case Dir::LD:
	case Dir::LU:
	case Dir::RU:
	case Dir::RD:
		rectBody.left = center.x - (bulletSize.x / 2);
		rectBody.right = rectBody.left + bulletSize.x;
		rectBody.top = center.y - (bulletSize.y / 2);
		rectBody.bottom = rectBody.top + bulletSize.y;
		break;
	case Dir::Left:
	case Dir::Right:
		rectBody.left = center.x - (bulletSize.y / 2);
		rectBody.right = rectBody.left + bulletSize.y;
		rectBody.top = center.y - (bulletSize.x / 2);
		rectBody.bottom = rectBody.top + bulletSize.x;
		break;
	default:
		assert(0);
		break;
	}
}
void BulletController::Bullet::Paint(HDC hdc, const ObjectImage& bulletImage) const
{
	bulletImage.Paint(hdc, rectBody, &rectImage);
}
bool BulletController::Bullet::Move(const RECT& rectWindow)
{
	int moveX = 0;
	int moveY = 0;
	switch (dir)
	{
	case Dir::Left:
		moveX = -speed;
		break;
	case Dir::Right:
		moveX = speed;
		break;
	case Dir::Up:
		moveY = -speed;
		break;
	case Dir::Down:
		moveY = speed;
		break;
	case Dir::LU:
		moveX = -speed;
		moveY = -speed;
		break;
	case Dir::LD:
		moveX = -speed;
		moveY = speed;
		break;
	case Dir::RU:
		moveX = speed;
		moveY = -speed;
		break;
	case Dir::RD:
		moveX = speed;
		moveY = speed;
		break;
	default:
		assert(0);
		break;
	}

	rectBody.left += moveX;
	rectBody.right += moveX;
	rectBody.top += moveY;
	rectBody.bottom += moveY;
	posCenter.x += moveX;
	posCenter.y += moveY;

	switch (dir)
	{
	case Dir::Left:
	case Dir::LU:
	case Dir::LD:
		if (rectBody.right < 0)
		{
			return false;
		}
		break;
	case Dir::Right:
	case Dir::RU:
	case Dir::RD:
		if (rectBody.left > rectWindow.right)
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
		if (rectBody.bottom < 0)
		{
			return false;
		}
		break;
	case Dir::Down:
	case Dir::LD:
	case Dir::RD:
		if (rectBody.top > rectWindow.bottom)
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
void BulletController::SetRectImage(int frame) {};

BulletController::BulletController(const RECT& rectWindow, const ObjectImage& bulletImage)
{
	this->rectWindow = &rectWindow;

	this->bulletImage = bulletImage;

	bulletSize = bulletImage.GetBodySize();
}

void BulletController::Paint(HDC hdc) const
{
	for (const Bullet* bullet : bullets)
	{
		bullet->Paint(hdc, bulletImage);
	}
}

void BulletController::CreateBullet(POINT center, Dir dir, int damage, int speed, bool hasFrame)
{
	RECT rectImage;
	if (hasFrame == true)
	{
		rectImage = GetRectImage(dir);
	}
	else
	{
		rectImage = GetRectImage(Dir::Up);
	}
	Bullet* bullet = new Bullet(center, bulletSize, dir, rectImage, damage, speed);
	bullets.emplace_back(bullet);
}

extern EnemyController* enemies;
extern Player* player;
extern EffectManager* effects;
void PlayerBullet::Move()
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (enemies->CheckHit(bullets.at(i)->GetRect(), bullets.at(i)->GetDamage()) == true)
		{
			effects->CreateEffect(EXPLODE_FIRE, bullets[i]->GetPos());
			bullets[i--] = bullets.back();
			bullets.pop_back();
		}
		else if(bullets.at(i)->Move(*rectWindow) == false)
		{
			bullets[i--] = bullets.back();
			bullets.pop_back();
		}
	}
}
void EnemyBullet::Move()
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (player->IsCollide(bullets.at(i)->GetRect()) == true)
		{
			player->GetDamage(bullets.at(i)->GetDamage());
			effects->CreateEffect(EXPLODE_FIRE, bullets[i]->GetPos());
			bullets[i--] = bullets.back();
			bullets.pop_back();
		}
		else if (bullets.at(i)->Move(*rectWindow) == false)
		{
			bullets[i--] = bullets.back();
			bullets.pop_back();
		}
	}
}