#include "stdafx.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"

extern EnemyController* enemies;
extern Player* player;
extern EffectManager* effects;

BulletController::Bullet::Bullet(POINT center, POINT bulletSize, RECT rectImage, const BulletData& data)
{
	this->rectImage = rectImage;
	this->data = data;

	rectBody.left = (float)center.x - ((float)bulletSize.x / 2);
	rectBody.right = rectBody.left + bulletSize.x;
	rectBody.top = (float)center.y - ((float)bulletSize.y / 2);
	rectBody.bottom = rectBody.top + bulletSize.y;
	rectRotBody = rectBody;
}
BulletController::Bullet::Bullet(POINT center, POINT bulletSize, RECT rectImage, const BulletData& data, Vector2 unitVector, bool isRotateImg) : Bullet(center, bulletSize, rectImage, data)
{
	this->dir = Dir::Empty;
	this->unitVector = unitVector;
	this->isRotateImg = isRotateImg;

	Vector2 vPoints[4];
	GetRotationPos(rectBody, unitVector, vPoints);
	rectRotBody = GetRotatedBody(vPoints);
}


void BulletController::Bullet::Paint(HDC hdc, const ObjectImage& bulletImage, const RECT& rectWindow)
{
	if (isRotateImg == false)
	{
		bulletImage.Paint(hdc, rectRotBody, &rectImage);
	}
	else
	{
		Vector2 vPoints[4];
		GetRotationPos(rectBody, unitVector, vPoints);
		bulletImage.PaintRotation(hdc, vPoints);
	}
	//FrameRect(hdc, &rectRotBody, (HBRUSH)GetStockObject(BLACK_BRUSH));
}
bool BulletController::Bullet::Move(const RECT& rectDisplay)
{
	float moveX = 0;
	float moveY = 0;
	switch (dir)
	{
	case Dir::Empty:
		moveX = unitVector.x * data.speed;
		moveY = unitVector.y * data.speed;
		break;
	case Dir::Left:
		moveX = -data.speed;
		break;
	case Dir::Right:
		moveX = data.speed;
		break;
	case Dir::Up:
		moveY = -data.speed;
		break;
	case Dir::Down:
		moveY = data.speed;
		break;
	case Dir::LU:
		moveX = -data.speed;
		moveY = -data.speed;
		break;
	case Dir::LD:
		moveX = -data.speed;
		moveY = data.speed;
		break;
	case Dir::RU:
		moveX = data.speed;
		moveY = -data.speed;
		break;
	case Dir::RD:
		moveX = data.speed;
		moveY = data.speed;
		break;
	default:
		assert(0);
		break;
	}

	rectBody.left += moveX;
	rectBody.right += moveX;
	rectBody.top += moveY;
	rectBody.bottom += moveY;
	rectRotBody.left += moveX;
	rectRotBody.right += moveX;
	rectRotBody.top += moveY;
	rectRotBody.bottom += moveY;

	switch (dir)
	{
	case Dir::Empty:
		if (OutOfRange(rectRotBody, rectDisplay) == true)
		{
			return false;
		}
		break;
	case Dir::Left:
	case Dir::LU:
	case Dir::LD:
		if (rectRotBody.right < 0)
		{
			return false;
		}
		break;
	case Dir::Right:
	case Dir::RU:
	case Dir::RD:
		if (rectRotBody.left > rectDisplay.right)
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
		if (rectRotBody.bottom < 0)
		{
			return false;
		}
		break;
	case Dir::Down:
	case Dir::LD:
	case Dir::RD:
		if (rectRotBody.top > rectDisplay.bottom)
		{
			return false;
		}
		break;
	}

	return true;
}
bool BulletController::Bullet::IsCollide(const RECT& rect) const
{
	RECT notuse = { 0, };
	RECT rectBody = (RECT)rectRotBody;
	return IntersectRect(&notuse, &rect, &rectBody);
}

POINT BulletController::Bullet::GetPos() const
{
	const int width = rectBody.right - rectBody.left;
	const int height = rectBody.bottom - rectBody.top;
	
	return { (LONG)rectBody.left + (width / 2), (LONG)rectBody.top + (height / 2) };
}


RECT BulletController::GetRectImage(Dir dir) const
{
	return ISprite::GetRectImage(bulletImage, 0);
}
void BulletController::SetRectImage(int frame) {};

BulletController::BulletController(const RECT& rectDisplay, const ObjectImage& bulletImage)
{
	this->rectDisplay = &rectDisplay;
	this->bulletImage = bulletImage;

	bulletSize = bulletImage.GetBodySize();
}

void BulletController::Paint(HDC hdc)
{
	for (Bullet* bullet : bullets)
	{
		bullet->Paint(hdc, bulletImage, *rectDisplay);
	}
}

void BulletController::CreateBullet(POINT center, const BulletData& data, Dir dir)
{
	RECT rectImage = GetRectImage(dir);
	Bullet* bullet = new Bullet(center, bulletSize, rectImage, data, dir);
	bullets.emplace_back(bullet);
}
void BulletController::CreateBullet(POINT center, const BulletData& data, Vector2 unitVector, bool isRotateImg)
{
	RECT rectImage = GetRectImage(Dir::Up);
	Bullet* bullet = new Bullet(center, bulletSize, rectImage, data, unitVector, isRotateImg);
	bullets.emplace_back(bullet);
}

void PlayerBullet::Move()
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (enemies->CheckHit(bullets.at(i)->GetRect(), bullets.at(i)->GetDamage(), bullets.at(i)->GetType(), bullets[i]->GetPos()) == true)
		{
			if (player->IsUsingSkill() == false)
			{
				player->AddMP(0.15f);
			}
			BulletController::Pop(i);
		}
		else if(bullets.at(i)->Move(*rectDisplay) == false)
		{
			BulletController::Pop(i);
		}
	}
}
void EnemyBullet::Move()
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (player->IsCollide(bullets.at(i)->GetRect()) == true)
		{
			player->Hit(bullets.at(i)->GetDamage(), bullets.at(i)->GetType());
			effects->CreateHitEffect(bullets.at(i)->GetPos(), bullets.at(i)->GetType());
			BulletController::Pop(i);
		}
		else if (bullets.at(i)->Move(*rectDisplay) == false)
		{
			BulletController::Pop(i);
		}
	}
}
void BulletController::Pop(size_t& index)
{
	bullets[index--] = bullets.back();
	bullets.pop_back();
}
void BulletController::DestroyCollideBullet(const RECT& rect)
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (bullets.at(i)->IsCollide(rect) == true)
		{
			BulletController::Pop(i);
		}
	}
}