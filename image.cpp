#include "image.h"

void ObjectImage::Load(const WCHAR* fileName, POINT imgSize, POINT bodyDrawPoint)
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
		memDC, rectImg.left, rectImg.top, drawSize.x, drawSize.y, bFunction);
	DeleteDC(memDC);
}
