#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <cassert>
#define	WIN32_LEAN_AND_MEAN
#undef WINVER
#define WINVER 0x6000

class ObjectImage {
private:
	HBITMAP hBitmap{};
	BLENDFUNCTION bFunction{};

	RECT rectImg = { 0, };
	POINT bodyDrawPoint = { 0, };
	POINT drawSize = { 0, };
	POINT bodySize = { 0, };

public:
	void Load(const WCHAR* fileName, POINT imgSize, POINT bodyDrawPoint, POINT bodySize);
	void Paint(HDC hdc, const RECT* rectBody, const RECT* rectImg = nullptr) const;
	void ScaleImage(double scaleX, double scaleY);

	inline RECT GetRectImg() const
	{
		return rectImg;
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

class IAnimatable abstract {
protected:
	int frame = 0;
	RECT GetRectImage(const ObjectImage* image) const;
public:
	virtual void Animate(HWND hWnd) abstract;
};