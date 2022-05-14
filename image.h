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

public:
	void Load(const WCHAR* fileName, POINT imgSize, POINT bodyDrawPoint);
	void Paint(HDC hdc, const RECT* rectBody);
};