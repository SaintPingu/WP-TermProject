#include "image.h"

void ObjectImage::Load(const WCHAR* fileName, POINT imgSize, POINT bodyDrawPoint, POINT bodySize)
{
	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(fileName, false);
	bitmap->GetHBITMAP(NULL, &hBitmap);
	delete bitmap;

	if (hBitmap == NULL)
	{
		assert(0);
	}

	bFunction.BlendOp = AC_SRC_OVER;
	bFunction.BlendFlags = 0;
	bFunction.SourceConstantAlpha = 0xff;
	bFunction.AlphaFormat = AC_SRC_ALPHA;

	this->rectImg = { 0, 0, imgSize.x, imgSize.y };
	this->bodyDrawPoint = bodyDrawPoint;
	drawSize.x = rectImg.right - rectImg.left;
	drawSize.y = rectImg.bottom - rectImg.top;
	this->bodySize = bodySize;
}

void ObjectImage::Paint(HDC hdc, const RECT* rectBody)
{
	HDC memDC = CreateCompatibleDC(hdc);;

	BITMAP bitmap;
	GetObject(hBitmap, sizeof(bitmap), &bitmap);

	SelectObject(memDC, hBitmap);

	RECT rectDraw = { 0, };
	rectDraw.left = rectBody->left - bodyDrawPoint.x;
	rectDraw.top = rectBody->top - bodyDrawPoint.y;
	rectDraw.right = rectDraw.left + drawSize.x;
	rectDraw.bottom = rectDraw.top + drawSize.y;

	AlphaBlend(hdc, rectDraw.left, rectDraw.top, (rectDraw.right - rectDraw.left), (rectDraw.bottom - rectDraw.top),
		memDC, rectImg.left, rectImg.top, (rectImg.right - rectImg.left), (rectImg.bottom - rectImg.top), bFunction);
	FrameRect(hdc, &rectDraw, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DeleteDC(memDC);
}

void ObjectImage::ScaleImage(double scaleX, double scaleY)
{
	drawSize.x *= scaleX;
	drawSize.y *= scaleY;
	bodyDrawPoint.x *= scaleX;
	bodyDrawPoint.y *= scaleY;
	bodySize.x *= scaleX;
	bodySize.y *= scaleY;
}

POINT ObjectImage::GetBodySize() const
{
	return bodySize;
}
