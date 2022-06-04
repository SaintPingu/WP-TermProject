#include "stdafx.h"
#include "image.h"

void Image::Load(const WCHAR* fileName, POINT imgSize, int alpha)
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
	bFunction.SourceConstantAlpha = alpha;
	bFunction.AlphaFormat = AC_SRC_ALPHA;

	this->rectImage = { 0, 0, imgSize.x, imgSize.y };
}
void Image::Paint(HDC hdc, const RECT& rectDraw, const RECT& rectImage) const
{
	HDC memDC = CreateCompatibleDC(hdc);

	SelectObject(memDC, hBitmap);

	AlphaBlend(hdc, rectDraw.left, rectDraw.top, (rectDraw.right - rectDraw.left), (rectDraw.bottom - rectDraw.top),
		memDC, rectImage.left, rectImage.top, (rectImage.right - rectImage.left), (rectImage.bottom - rectImage.top), bFunction);
	//FrameRect(hdc, &rectDraw, (HBRUSH)GetStockObject(BLACK_BRUSH));
	
	DeleteDC(memDC);
}
void Image::PaintRotation(HDC hdc, Vector2 vPoints[3]) const
{
	HDC memDCBack = CreateCompatibleDC(hdc);
	HDC memDCObject = CreateCompatibleDC(hdc);

	static HBITMAP hBitmapBack = CreateCompatibleBitmap(hdc, WINDOWSIZE_X, WINDOWSIZE_Y);
	SelectObject(memDCBack, hBitmapBack);
	SelectObject(memDCObject, hBitmap);

	POINT points[3] = { vPoints[0], vPoints[1], vPoints[2] };
	RECT rectDraw = GetRotatedBody(vPoints);
	int wDest = rectDraw.right - rectDraw.left;
	int hDest = rectDraw.bottom - rectDraw.top;

	RECT rectFill = { 0,0,rectImage.right + rectImage.bottom, rectImage.right  + rectImage.bottom};
	FillRect(memDCBack, &rectFill, transBrush);
	rectFill = rectDraw;
	rectFill.right += wDest;
	rectFill.bottom += hDest;
	FillRect(memDCBack, &rectFill, transBrush);

	AlphaBlend(memDCBack, 0, 0, rectImage.right, rectImage.bottom,
		memDCObject, rectImage.left, rectImage.top, rectImage.right, rectImage.bottom, bFunction);

	PlgBlt(memDCBack, points, memDCBack, 0, 0, rectImage.right, rectImage.bottom, NULL, 0, 0);

	TransparentBlt(hdc, rectDraw.left, rectDraw.top, wDest, hDest, memDCBack, rectDraw.left, rectDraw.top, wDest, hDest, transRGB);
	//FrameRect(hdc, &rectDraw, (HBRUSH)GetStockObject(BLACK_BRUSH));

	DeleteDC(memDCBack);
	DeleteDC(memDCObject);
}

void ObjectImage::Load(const WCHAR* fileName, POINT imgSize, POINT bodyDrawPoint, POINT bodySize)
{
	Image::Load(fileName, imgSize);
	
	this->bodyDrawPoint = bodyDrawPoint;
	drawSize.x = rectImage.right - rectImage.left;
	drawSize.y = rectImage.bottom - rectImage.top;

	if (bodySize.x == 0)
	{
		bodySize = imgSize;
	}
	this->bodySize = bodySize;
}

void ObjectImage::Paint(HDC hdc, const RECT& rectBody, const RECT* rectImage) const
{
	if (rectImage == nullptr)
	{
		rectImage = &this->rectImage;
	}

	RECT rectDraw = { 0, };
	rectDraw.left = rectBody.left - bodyDrawPoint.x;
	rectDraw.top = rectBody.top - bodyDrawPoint.y;
	rectDraw.right = rectDraw.left + drawSize.x;
	rectDraw.bottom = rectDraw.top + drawSize.y;

	Image::Paint(hdc, rectDraw, *rectImage);
}
void ObjectImage::Paint(const RECT& rectDest, HDC hdc) const
{
	Image::Paint(hdc, rectDest, this->rectImage);
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

	this->scaleX = scaleX;
	this->scaleY = scaleY;
}


void EffectImage::Load(const WCHAR* fileName, POINT imgSize, int maxFrame, int alpha)
{
	Image::Load(fileName, imgSize, alpha);
	++rectImage.left;
	++rectImage.top;
	++rectImage.right;
	++rectImage.bottom;
	this->maxFrame = maxFrame;
	this->drawSize = imgSize;
}
void EffectImage::Paint(HDC hdc, POINT drawPoint, const RECT* rectImage) const
{
	if (rectImage == nullptr)
	{
		rectImage = &this->rectImage;
	}

	RECT rectDraw = { 0, };
	rectDraw.left = drawPoint.x - (drawSize.x / 2);
	rectDraw.top = drawPoint.y - (drawSize.y / 2);
	rectDraw.right = rectDraw.left + drawSize.x;
	rectDraw.bottom = rectDraw.top + drawSize.y;

	Image::Paint(hdc, rectDraw, *rectImage);
}
void EffectImage::Paint(HDC hdc, const RECT& rectDest, const RECT* rectImage) const
{
	if (rectImage == nullptr)
	{
		rectImage = &this->rectImage;
	}

	Image::Paint(hdc, rectDest, *rectImage);
}
void EffectImage::ScaleImage(float scaleX, float scaleY)
{
	drawSize.x = (LONG)((float)drawSize.x * scaleX);
	drawSize.y = (LONG)((float)drawSize.y * scaleY);
}






void GUIImage::Load(const WCHAR* fileName, POINT imgSize, int alpha)
{
	Image::Load(fileName, imgSize, alpha);
}
void GUIImage::Paint(HDC hdc, const RECT& rectDest)
{
	Image::Paint(hdc, rectDest, rectImage);
}
void GUIImage::PaintBlack(HDC hdc, const RECT& rectDest)
{
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rectImage.right, rectImage.bottom);
	SelectObject(memDC, hBitmap);

	RECT rectMask = { 0, 0, rectImage.right, rectImage.bottom };
	Image::Paint(memDC, rectMask, rectImage);
	SelectObject(hdc, GetStockObject(GRAY_BRUSH));
	StretchBlt(hdc, rectDest.left, rectDest.top, rectDest.right - rectDest.left, rectDest.bottom - rectDest.top, memDC, 0, 0, rectImage.right, rectImage.bottom, MERGECOPY);

	DeleteDC(memDC);
	DeleteObject(hBitmap);
}
void GUIImage::PaintGauge(HDC hdc, const RECT& rectDest, float current, float max)
{
	constexpr int gapY = 6;
	const int imgWidth = rectImage.right - rectImage.left;
	const int imgHeight = rectImage.bottom - rectImage.top;
	const int wDest = rectDest.right - rectDest.left;
	const int hDest = rectDest.bottom - rectDest.top;

	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, imgWidth, imgHeight);
	SelectObject(memDC, hBitmap);

	FillRect(memDC, &rectImage, transBrush);
	Image::Paint(memDC, rectImage, rectImage);

	RECT rectErase = rectImage;
	float unitHeight = imgHeight / max;
	rectErase.bottom = rectErase.top + ((max - current) * unitHeight);

	FillRect(memDC, &rectErase, transBrush);

	TransparentBlt(hdc, rectDest.left, rectDest.top, wDest, hDest, memDC, rectImage.left, rectImage.top, imgWidth, imgHeight, transRGB);

	DeleteDC(memDC);
	DeleteObject(hBitmap);
}





RECT ISprite::GetRectImage(const Image& image, int frame, int spriteRow) const
{
	POINT drawSize = image.GetDrawSize();
	RECT rectImage = image.GetRectImage();

	int width = (rectImage.right - rectImage.left) + 1;
	int height = (rectImage.bottom - rectImage.top) + 1;
	rectImage.left += (width * frame);
	rectImage.right += (width * frame);
	rectImage.top += (height * spriteRow);
	rectImage.bottom += (height * spriteRow);

	return rectImage;
}