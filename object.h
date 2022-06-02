#pragma once
#include "image.h"

class GameObject abstract {
private:
	ObjectImage* image;

	Vector2 posCenter;
	POINT bodySize = { 0, };
	RECT rectBody = { 0, };

	bool isShowHitbox = false;

protected:
	Dir direction = Dir::Empty;

	GameObject(ObjectImage& image, float scaleX, float scaleY, Vector2 pos);
	void SetPos(Vector2 pos);

	inline const ObjectImage& GetImage()
	{
		return *image;
	}

public:
	void Paint(HDC hdc, const RECT* rectImage = nullptr);
	RECT GetRectBody(POINT pos) const;

	bool IsCollide(const RECT& rectSrc) const;

	inline RECT GetRectBody() const
	{
		return rectBody;
	}
	inline Vector2 GetPosCenter() const
	{
		return posCenter;
	}

	inline void ShowHitbox()
	{
		isShowHitbox = !(isShowHitbox);
	}
};

Vector2 Lerp(Vector2 src, Vector2 dst, float alpha);

inline float GetRadius(float x, float y)
{
	return sqrtf((x * x) + (y * y));
}

class IControllable abstract {
private:
	virtual void SetPosDest() abstract;
	bool isMove = false;
public:
	virtual void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) abstract;
	virtual void Move(HWND hWnd, int timerID) abstract;
	virtual void Stop(Dir dir) abstract;
	inline void StartMove()
	{
		isMove = true;
	}
	inline void StopMove()
	{
		isMove = false;
	}
	inline bool IsMove() const
	{
		return isMove;
	}
};

class IMovable abstract {
private:
	virtual void SetPosDest() abstract;
	bool isMove = false;
public:
	virtual void Move() abstract;
	inline void StartMove()
	{
		isMove = true;
	}
	inline void StopMove()
	{
		isMove = false;
	}
	inline bool IsMove() const
	{
		return isMove;
	}
	
};

void CalculateDamage(float& damage, Type destType, Type srcType);