#pragma once
#pragma warning(disable:28251)
#pragma warning(disable:4244)
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <tchar.h>
#include <gdiplus.h>
#include <atlImage.h>
#include <vector>
#include <cassert>


#define WINDOWSIZE_X 500
#define WINDOWSIZE_Y 750

#define	WIN32_LEAN_AND_MEAN
#undef WINVER
#define WINVER 0x6000

enum class Difficulty { Easy = 0, Normal, Hard };
enum class Scene { Start = 0, Loading, Lobby, Stage, Battle };
enum class Stage { Empty = 0, Fire, Water, Electric, Dark };
enum class Action { Idle = 0, Attack, Hurt, Death };

enum class Pokemon : int { Moltres = 0, Articuno, Thunder };
enum class SubPokemon : int { Pikachu = 0, Squirtle, Charmander };
enum class Type { Empty = 0, Fire, Elec, Water };
enum class Skill : int { Empty = 0, Identity, Sector, Circle, Ultimate };

#define PI 3.141592
#define DEGREE_TO_RADIAN(degree) ((PI/180) * (degree))
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
	float x = 0;
	float y = 0;

	inline constexpr Vector2 operator+(const Vector2& rhs)
	{
		return { this->x + rhs.x, this->y + rhs.y };
	}
	inline constexpr Vector2 operator-(const Vector2& rhs)
	{
		return { this->x - rhs.x, this->y - rhs.y };
	}
	inline constexpr Vector2 operator*(const float& rhs)
	{
		return { this->x * rhs, this->y * rhs };
	}
	inline constexpr Vector2 operator/(const float& rhs)
	{
		return { this->x / rhs, this->y / rhs };
	}
	inline constexpr operator POINT ()
	{
		return { (LONG)this->x, (LONG)this->y };
	}
	inline constexpr Vector2 operator=(const POINT& rhs)
	{
		return { static_cast<float>(rhs.x), static_cast<float>(rhs.y) };
	}

	inline Vector2 Normalized()
	{
		return *this / GetNorm(*this);
	}
	inline float Norm()
	{
		return sqrtf(x * x + y * y);
	}
	static inline float GetNorm(Vector2 v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}
	static inline constexpr float Dot(const Vector2 lhs, const Vector2 rhs)
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y);
	}
	static inline float GetTheta(const Vector2 lhs, const Vector2 rhs)
	{
		float dot = Vector2::Dot(lhs, rhs);
		return acos(dot / (Vector2::GetNorm(lhs) * Vector2::GetNorm(rhs)));
	}

	static inline constexpr Vector2 Forward()
	{
		return { 0, -1 };
	}
	static inline constexpr Vector2 Back()
	{
		return { 0, 1 };
	}
	static inline constexpr Vector2 Left()
	{
		return { -1, 0 };
	}
	static inline constexpr Vector2 Right()
	{
		return { 1, 0 };
	}
};

Vector2 Rotate(Vector2 vector, float degree);
bool OutOfRange(const RECT& rect, const RECT& rectRange);
void GetRotationPos(const RECT& rect, const Vector2& unitVector, Vector2 vPoints[3]);
RECT GetRotatedBody(Vector2 vPoints[3]);