#pragma once
#include <Windows.h>
#include "math.h"
#include "image.h"

#define PI 3.141592
#define RADIAN_TO_DEGREE(radian) ((180/PI) * (radian))

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

// POINT 연산자 오버로딩
// ex) POINT a,b;
// possible : a - b, a + b;
inline constexpr POINT operator-(const POINT& lhs, const POINT& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}
inline constexpr POINT operator+(const POINT& lhs, const POINT& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

struct Vector2 {
	double x = 0;
	double y = 0;

	inline constexpr Vector2 operator+(const Vector2& rhs)
	{
		return { this->x + rhs.x, this->y + rhs.y };
	}
	inline constexpr Vector2 operator-(const Vector2& rhs)
	{
		return { this->x - rhs.x, this->y - rhs.y };
	}
	inline constexpr Vector2 operator*(const double& rhs)
	{
		return { this->x * rhs, this->y * rhs };
	}
	inline constexpr Vector2 operator/(const double& rhs)
	{
		return { this->x / rhs, this->y / rhs };
	}
	inline constexpr operator POINT ()
	{
		return { (LONG)this->x, (LONG)this->y };
	}
};


class GameObject abstract {
private:
	ObjectImage image;

	Vector2 posCenter;
	POINT bodySize = { 0, };
	RECT rectBody = { 0, };

	bool isShowHitbox = false;

protected:
	Dir direction = Dir::Empty;

	GameObject(ObjectImage image, double scaleX, double scaleY, Vector2 pos = { 0, 0 });
	void SetPos(Vector2 pos);

	inline const ObjectImage& GetImage()
	{
		return image;
	}
	inline RECT GetRectBody() const
	{
		return rectBody;
	}

public:
	void Paint(HDC hdc, const RECT* rectImage = nullptr);
	RECT GetRectBody(POINT pos) const;

	bool IsCollide(const RECT& rectSrc) const;

	inline Vector2 GetPosCenter() const
	{
		return posCenter;
	}

	inline void ShowHitbox()
	{
		isShowHitbox = !(isShowHitbox);
	}
};

Vector2 Lerp(Vector2 src, Vector2 dst, double alpha);
inline double GetRadius(double x, double y)
{
	return sqrt((x * x) + (y * y));
}

class IControllable abstract {
private:
	virtual void SetPosDest() abstract;
protected:
	bool isMove = false;
public:
	virtual void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) abstract;
	virtual void Move(HWND hWnd, int timerID) abstract;
	virtual void Stop(Dir dir) abstract;

	inline bool IsMove() const
	{
		return isMove;
	}
};

class IMovable abstract {
private:
	virtual void SetPosDest() abstract;
protected:
	bool isMove = false;
public:
	virtual void Move() abstract;
	virtual void Stop() abstract;

	inline bool IsMove() const
	{
		return isMove;
	}
};