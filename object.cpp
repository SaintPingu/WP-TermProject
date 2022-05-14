#include "object.h"

GameObject::GameObject(ObjectImage image, double scaleX, double scaleY, POINT pos)
{
	this->image = image;
	this->image.ScaleImage(scaleX, scaleY);
	bodySize = this->image.GetBodySize();

	SetPos(pos);
}

void GameObject::SetPos(POINT pos)
{
	posCenter.x = pos.x;
	posCenter.y = pos.y;

	rectBody.left = posCenter.x - (bodySize.x / 2);
	rectBody.top = posCenter.y - (bodySize.y / 2);
	rectBody.right = rectBody.left + bodySize.x;
	rectBody.bottom = rectBody.top + bodySize.y;
}

void GameObject::Paint(HDC hdc)
{
	image.Paint(hdc, &rectBody);
	if (isShowHitbox == true)
	{
		FrameRect(hdc, &rectBody, (HBRUSH)GetStockObject(BLACK_BRUSH));
	}
}

void GameObject::ShowHitbox()
{
	isShowHitbox = !(isShowHitbox);
}

bool GameObject::IsCollide(const RECT* rectSrc) const
{
	RECT notuse;
	return (bool)IntersectRect(&notuse, &rectBody, rectSrc);
}
