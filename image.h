#pragma once

class Image abstract {
private:
	HBITMAP hBitmap{};
	BLENDFUNCTION bFunction{};

protected:
	RECT rectImage = { 0, };
	POINT drawSize = { 0, };
	void Load(const WCHAR* fileName, POINT imgSize);
	BITMAP SelectHBitmap(HDC hdc) const;
	void Paint(HDC hdc, HDC memDC, const RECT& rectDraw, const RECT& rectImage) const;

public:
	virtual void ScaleImage(float scaleX, float scaleY) abstract;

	inline RECT GetRectImage() const
	{
		return rectImage;
	}
	inline POINT GetDrawSize() const
	{
		return drawSize;
	}
};

class ObjectImage : public Image{
private:
	POINT bodyDrawPoint = { 0, };
	POINT bodySize = { 0, };
	bool isScaled = false;

public:
	void Load(const WCHAR* fileName, POINT imgSize, POINT bodyDrawPoint, POINT bodySize);
	void Paint(HDC hdc, const RECT& rectBody, const RECT* rectImage = nullptr) const;
	void ScaleImage(float scaleX, float scaleY);
	
	inline POINT GetBodyDrawPoint() const
	{
		return bodyDrawPoint;
	}
	inline POINT GetBodySize() const
	{
		return bodySize;
	}
};

class EffectImage : public Image {
private:
	int maxFrame = 0;
	POINT imgSize = { 0, };
public:
	void Load(const WCHAR* fileName, POINT imgSize, int maxFrame);
	void Paint(HDC hdc, POINT drawPoint, const RECT* rectImage) const;
	void ScaleImage(float scaleX, float scaleY);

	inline int GetMaxFrame() const
	{
		return maxFrame;
	}
};






class ISprite abstract {
protected:
	int frame = 0;
	RECT GetRectImage(const Image& image, int frame, int spriteRow = 0) const;
};

class IAnimatable abstract : public ISprite {
private:
	Action action = Action::Idle;
protected:
	bool isRevFrame = false;

	inline Action GetAction() const
	{
		return action;
	}
	inline void SetAction(Action action, int frame)
	{
		this->frame = frame;
		this->action = action;
		this->isRevFrame = false;
	}
public:
	virtual void Animate() abstract;
};