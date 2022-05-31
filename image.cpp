#include "stdafx.h"
#include "image.h"

void Image::Load(const WCHAR* fileName, POINT imgSize)
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

	RECT rectDraw = { 0, };
	rectDraw.left = points[0].x;
	rectDraw.right = points[0].x;
	rectDraw.top = points[0].y;
	rectDraw.bottom = points[0].y;

	for (int i = 1; i < 3; ++i)
	{
		if (rectDraw.left > points[i].x)
		{
			rectDraw.left = points[i].x;
		}
		else if (rectDraw.right < points[i].x)
		{
			rectDraw.right = points[i].x;
		}
		if (rectDraw.top > points[i].y)
		{
			rectDraw.top = points[i].y;
		}
		else if (rectDraw.bottom < points[i].y)
		{
			rectDraw.bottom = points[i].y;
		}
	}
	int wDest = (rectDraw.right - rectDraw.left) * 2;
	int hDest = (rectDraw.bottom - rectDraw.top) * 2;

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


void EffectImage::Load(const WCHAR* fileName, POINT imgSize, int maxFrame)
{
	Image::Load(fileName, imgSize);
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
void EffectImage::ScaleImage(float scaleX, float scaleY)
{
	drawSize.x = (LONG)((float)drawSize.x * scaleX);
	drawSize.y = (LONG)((float)drawSize.y * scaleY);
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