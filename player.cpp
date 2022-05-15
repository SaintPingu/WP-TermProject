#include "player.h"


Player::Player(HWND hWnd, const RECT& rectWindow, ObjectImage image, double scaleX, double scaleY, Vector2 pos) : GameObject(image, scaleX, scaleY, pos)
{
	this->rectWindow = &rectWindow;

	direction = Dir::Empty;
	posDest = { 0, };
	vectorMove = { 0, };

	alpha = 0;
	isMove = false;

	rectImage = image.GetRectImage();

	ObjectImage bulletImage;
	bulletImage.Load(L"sprite_bullet.png", { 20,20 }, { 6,2 }, { 10,16 });
	bulletController = new BulletController(rectWindow, bulletImage);
}

void Player::Paint(HDC hdc)
{
	GameObject::Paint(hdc, &rectImage);
	bulletController->Paint(hdc);
}

void Player::SetVectorDest()
{
	constexpr int amount = 10;
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
	isMove = false;
}

void Player::SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc)
{
	if (isMove == true && alpha > 0)
	{
		return;
	}

	SetVectorDest();

	if (isMove == false && alpha == 0)
	{
		SetTimer(hWnd, timerID, elpase, timerProc);
	}
	else if (alpha > 0.5f)
	{
		alpha = 0.5f;
	}
	isMove = true;
}
void Player::Move(HWND hWnd, int timerID)
{
	Vector2 posCenter = GetPosCenter();
	Vector2 posNext = Lerp(posCenter, posDest, alpha);

	SetPos(posNext);
	posCenter = GetPosCenter();
	vectorMove = posDest - posCenter;

	alpha += 0.1f;
	if (alpha > 0.5f)
	{
		if (direction != Dir::Empty)
		{
			SetVectorDest();

			alpha = 0.5f;
		}
		else if (alpha > 1)
		{
			vectorMove = { 0, };
			isMove = false;
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

	const ObjectImage& image = GetImage();
	rectImage = GetRectImage(image, frame);
}

void Player::Shot()
{
	RECT rectBody = GetRectBody();
	POINT bulletPos;
	bulletPos.y = rectBody.top;

	bulletPos.x = rectBody.left - 10;
	bulletController->CreateBullet(bulletPos, Dir::Up);
	bulletPos.x = rectBody.right + 10;
	bulletController->CreateBullet(bulletPos, Dir::Up);
}

void Player::MoveBullets()
{
	bulletController->Move();
}