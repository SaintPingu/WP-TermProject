#include "stdafx.h"
#include "player.h"
#include "bullet.h"
#include "timer.h"


Player::Player(HWND hWnd, const RECT& rectWindow, ObjectImage& image, float scaleX, float scaleY, Vector2 pos, PlayerData data) : GameObject(image, scaleX, scaleY, pos)
{
	this->rectWindow = &rectWindow;
	this->data = data;

	direction = Dir::Empty;
	posDest = { 0, };
	vectorMove = { 0, };

	alpha = 0;
	StopMove();

	ObjectImage bulletImage;
	bulletImage.Load(_T("sprite_bullet.png"), { 20,20 }, { 6,2 }, { 10,16 });
	bullets = new PlayerBullet(rectWindow, bulletImage);
}

void Player::Paint(HDC hdc)
{
	RECT rectImage = GetRectImage(GetImage(), frame);
	GameObject::Paint(hdc, &rectImage);
	bullets->Paint(hdc);
}

void Player::SetPosDest()
{
	const int amount = data.speed * 2;
	switch (direction)
	{
	case Dir::Left:
		vectorMove.x = -amount;
		break;
	case Dir::Right:
		vectorMove.x = amount;
		break;
	case Dir::Up:
		vectorMove.y = -amount;
		break;
	case Dir::Down:
		vectorMove.y = amount;
		break;
	case Dir::LD:
		vectorMove.x = -amount;
		vectorMove.y = amount;
		break;
	case Dir::LU:
		vectorMove.x = -amount;
		vectorMove.y = -amount;
		break;
	case Dir::RD:
		vectorMove.x = amount;
		vectorMove.y = amount;
		break;
	case Dir::RU:
		vectorMove.x = amount;
		vectorMove.y = -amount;
		break;
	}

	Vector2 posCenter = GetPosCenter();
	posDest = posCenter + vectorMove;
}

void Player::SetDirection(Dir inputDir)
{
	if (direction == inputDir || direction == Dir::Empty)
	{
		direction = inputDir;
		return;
	}

	switch (direction)
	{
	case Dir::Left:
		if (inputDir == Dir::Up)		direction = Dir::LU;
		else if (inputDir == Dir::Down)	direction = Dir::LD;
		break;
	case Dir::Right:
		if (inputDir == Dir::Up)		direction = Dir::RU;
		else if (inputDir == Dir::Down)	direction = Dir::RD;
		break;
	case Dir::Up:
		if (inputDir == Dir::Left)		direction = Dir::LU;
		else if (inputDir == Dir::Right)direction = Dir::RU;
		break;
	case Dir::Down:
		if (inputDir == Dir::Left)		direction = Dir::LD;
		else if (inputDir == Dir::Right)direction = Dir::RD;
		break;
	default:
		return;
	}
	StopMove();
}

void Player::SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc)
{
	if (IsMove() == true && alpha > 0)
	{
		return;
	}

	SetPosDest();

	if (IsMove() == false && alpha == 0)
	{
		frame = 0;
		SetTimer(hWnd, timerID, elpase, timerProc);
	}
	else if (alpha > 0.5f)
	{
		alpha = 0.5f;
	}
	StartMove();
}

void Player::Move(HWND hWnd, int timerID)
{
	Vector2 posCenter = GetPosCenter();
	Vector2 posNext = Lerp(posCenter, posDest, alpha);

	posNext = CheckCollideWindow(posNext);
	SetPos(posNext);
	posCenter = GetPosCenter();
	vectorMove = posDest - posCenter;

	alpha += 0.1f;
	if (alpha > 0.5f)
	{
		if (direction != Dir::Empty)
		{
			SetPosDest();

			alpha = 0.5f;
		}
		else if (alpha > 1)
		{
			vectorMove = { 0, };
			StopMove();
			alpha = 0;
			KillTimer(hWnd, timerID);
		}
	}
}

void Player::Stop(Dir inputDir)
{
	switch (direction)
	{
	case Dir::Left:
		if (inputDir != Dir::Left) return;
		break;
	case Dir::Right:
		if (inputDir != Dir::Right) return;
		break;
	case Dir::Up:
		if (inputDir != Dir::Up) return;
		break;
	case Dir::Down:
		if (inputDir != Dir::Down) return;
		break;
	case Dir::LU:
		if (inputDir != Dir::Left && inputDir != Dir::Up) return;
		break;
	case Dir::LD:
		if (inputDir != Dir::Left && inputDir != Dir::Down) return;
		break;
	case Dir::RU:
		if (inputDir != Dir::Right && inputDir != Dir::Up) return;
		break;
	case Dir::RD:
		if (inputDir != Dir::Right && inputDir != Dir::Down) return;
		break;
	}
	direction = direction - inputDir;
}

Vector2 Player::CheckCollideWindow(Vector2 pos) const
{
	RECT rectBody = GetRectBody(pos);

	POINT corrValue = { 0, };
	if (rectBody.left < 0)
	{
		corrValue.x = -rectBody.left;
	}
	else if (rectBody.right > rectWindow->right)
	{
		corrValue.x = rectWindow->right - rectBody.right;
	}
	if (rectBody.top < 0)
	{
		corrValue.y = -rectBody.top;
	}
	else if (rectBody.bottom > rectWindow->bottom)
	{
		corrValue.y = rectWindow->bottom - rectBody.bottom;
	}

	return { pos.x + corrValue.x, pos.y + corrValue.y };
}

void Player::Animate()
{
	if (isRevFrame == true)
	{
		--frame;
	}
	else
	{
		++frame;
	}

	switch (GetAction())
	{
	case Action::Idle:
		if (frame > 2)
		{
			isRevFrame = true;
			--frame;
		}
		else if (frame < 0)
		{
			isRevFrame = false;
			++frame;
		}
		break;
	}
}
void Player::Shot()
{
	RECT rectBody = GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.top;

	bulletPos.x = rectBody.left - 10;
	bullets->CreateBullet(bulletPos, Dir::Up, 1, 10);
	bulletPos.x = rectBody.right + 10;
	bullets->CreateBullet(bulletPos, Dir::Up, 1, 10);
}

void Player::MoveBullets()
{
	bullets->Move();
}

void Player::GetDamage(int damage)
{
	if ((data.hp -= damage) <= 0)
	{
		data.speed = 0;
	}
}