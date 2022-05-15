#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <cassert>
#define	WIN32_LEAN_AND_MEAN
#undef WINVER
#define WINVER 0x6000

enum class Action { Idle, Attack, Hurt, Death };

class ObjectImage {
private:
	HBITMAP hBitmap{};
	BLENDFUNCTION bFunction{};

	RECT rectImage = { 0, };
	POINT bodyDrawPoint = { 0, };
	POINT drawSize = { 0, };
	POINT bodySize = { 0, };

public:
	void Load(const WCHAR* fileName, POINT imgSize, POINT bodyDrawPoint, POINT bodySize);
	void Paint(HDC hdc, const RECT& rectBody, const RECT* rectImage = nullptr) const;
	void ScaleImage(double scaleX, double scaleY);

	inline RECT GetRectImage() const
	{
		return rectImage;
	}
	inline POINT GetBodyDrawPoint() const
	{
		return bodyDrawPoint;
	}
	inline POINT GetDrawSize() const
	{
		return drawSize;
	}
	inline POINT GetBodySize() const
	{
		return bodySize;
	}
};

class ISprite abstract {
protected:
	RECT GetRectImage(const ObjectImage& image, int frame, int spriteRow = 0) const;
};

class IAnimatable abstract : public ISprite {
private:
	Action action = Action::Idle;
protected:
	RECT rectImage = { 0, };
	int frame = 0;
	bool isRevFrame = false;

	inline Action GetAction() const
	{
		return action;
	}
public:
	virtual void Animate() abstract;
};