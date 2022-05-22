#include "stdafx.h"
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

	this->rectImage = { 0, 0, imgSize.x, imgSize.y };
	this->bodyDrawPoint = bodyDrawPoint;
	drawSize.x = rectImage.right - rectImage.left;
	drawSize.y = rectImage.bottom - rectImage.top;
	this->bodySize = bodySize;
}

void ObjectImage::Paint(HDC hdc, const RECT& rectBody, const RECT* rectImage) const
{
	if (rectImage == nullptr)
	{
		rectImage = &this->rectImage;
	}

	HDC memDC = CreateCompatibleDC(hdc);;

	BITMAP bitmap;
	GetObject(hBitmap, sizeof(bitmap), &bitmap);

	SelectObject(memDC, hBitmap);

	RECT rectDraw = { 0, };
	rectDraw.left = rectBody.left - bodyDrawPoint.x;
	rectDraw.top = rectBody.top - bodyDrawPoint.y;
	rectDraw.right = rectDraw.left + drawSize.x;
	rectDraw.bottom = rectDraw.top + drawSize.y;

	AlphaBlend(hdc, rectDraw.left, rectDraw.top, (rectDraw.right - rectDraw.left), (rectDraw.bottom - rectDraw.top),
		memDC, rectImage->left, rectImage->top, (rectImage->right - rectImage->left), (rectImage->bottom - rectImage->top), bFunction);
	//FrameRect(hdc, &rectDraw, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DeleteDC(memDC);
}

void ObjectImage::ScaleImage(float scaleX, float scaleY)
{
	if (isScaled == true)
	{
		return;
	}
	isScaled = true;

	drawSize.x = (LONG)((float)drawSize.x * scaleX);
	drawSize.y = (LONG)((float)drawSize.y * scaleY);
	bodyDrawPoint.x = (LONG)((float)bodyDrawPoint.x * scaleX);
	bodyDrawPoint.y = (LONG)((float)bodyDrawPoint.y * scaleY);
	bodySize.x = (LONG)((float)bodySize.x * scaleX);
	bodySize.y = (LONG)((float)bodySize.y * scaleY);
}


RECT ISprite::GetRectImage(const ObjectImage& image, int frame, int spriteRow) const
{
	POINT drawSize = image.GetDrawSize();
	RECT rectImage = image.GetRectImage();

	int width = (rectImage.right - rectImage.left) - 1;
	rectImage.left += (width * frame);
	rectImage.right += (width * frame);

	int height = (rectImage.bottom - rectImage.top) - 1;
	rectImage.top += (height * spriteRow);
	rectImage.bottom += (height * spriteRow);

	return rectImage;
}