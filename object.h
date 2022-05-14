#pragma once
#include <Windows.h>
#include "image.h"

enum class Action { Idle, Attack, Hury, Death };
enum class Dir { Empty = 0, Left, Right, Up, Down, LD, LU, RD, RU };

// Direction 연산자 오버로딩
// ex) Left - LD = LD - Left = Down
//     Left + Up = LU
constexpr Dir operator-(Dir lhs, Dir rhs)
{
	if (lhs == rhs)
	{
		return Dir::Empty;
	}
	else if (lhs < rhs)
	{
		Dir temp = lhs;
		lhs = rhs;
		rhs = temp;
	}

	Dir result = Dir::Empty;
	switch (lhs)
	{
	case Dir::LD:
		if (rhs == Dir::Left)
		{
			result = Dir::Down;
		}
		else if (rhs == Dir::Down)
		{
			result = Dir::Left;
		}
		break;
	case Dir::LU:
		if (rhs == Dir::Left)
		{
			result = Dir::Up;
		}
		else if (rhs == Dir::Up)
		{
			result = Dir::Left;
		}
		break;
	case Dir::RD:
		if (rhs == Dir::Right)
		{
			result = Dir::Down;
		}
		else if (rhs == Dir::Down)
		{
			result = Dir::Right;
		}
		break;
	case Dir::RU:
		if (rhs == Dir::Right)
		{
			result = Dir::Up;
		}
		else if (rhs == Dir::Up)
		{
			result = Dir::Right;
		}
		break;
	default:
		break;
	}

	return result;
}
constexpr Dir operator+(Dir lhs, Dir rhs)
{
	if (lhs == rhs)
	{
		return lhs;
	}
	else if (lhs < rhs)
	{
		Dir temp = lhs;
		lhs = rhs;
		rhs = temp;
	}

	switch (rhs)
	{
	case Dir::Left:
	{
		switch (lhs)
		{
		case Dir::Up:	return Dir::LU;
		case Dir::Down:	return Dir::LD;
		}
	}
	case Dir::Right:
	{
		switch (lhs)
		{
		case Dir::Up:	return Dir::RU;
		case Dir::Down:	return Dir::RD;
		}
	}
	case Dir::Up:
	{
		switch (lhs)
		{
		case Dir::Left:		return Dir::LU;
		case Dir::Right:	return Dir::RU;
		}
	}
	case Dir::Down:
	{
		switch (lhs)
		{
		case Dir::Left:		return Dir::LD;
		case Dir::Right:	return Dir::RD;
		}
	}
	}

	return Dir::Empty;
}

class GameObject abstract {
private:
	ObjectImage image;

	POINT posCenter;
	POINT bodySize = { 0, };
	RECT rectBody = { 0, };

	bool isShowHitbox = false;

protected:
	Dir direction = Dir::Empty;

	GameObject(ObjectImage image, double scaleX, double scaleY, POINT pos = { 0, 0 });
	void SetPos(POINT pos);

	inline const ObjectImage* GetImage()
	{
		return &image;
	}
	inline RECT GetRectBody() const
	{
		return rectBody;
	}

public:
	void Paint(HDC hdc, const RECT* rectImage = nullptr);
	
	bool IsCollide(const RECT* rectSrc) const;

	inline POINT GetPosCenter() const
	{
		return posCenter;
	}

	inline void ShowHitbox()
	{
		isShowHitbox = !(isShowHitbox);
	}
};

class IMovable abstract {
private:
	virtual void SetPosDest() abstract;
public:
	virtual void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) abstract;
	virtual void Move(HWND hWnd, int timerID) abstract;
	virtual void Stop(HWND hWnd, Dir dir) abstract;
	virtual bool IsMove() const abstract;
};