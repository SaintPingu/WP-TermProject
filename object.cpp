#include "stdafx.h"
#include "object.h"

GameObject::GameObject(ObjectImage& image, float scaleX, float scaleY, Vector2 pos)
{
	this->image = &image;
	this->image->ScaleImage(scaleX, scaleY);
	bodySize = this->image->GetBodySize();

	SetPos(pos);
}

void GameObject::SetPos(Vector2 pos)
{
	posCenter.x = pos.x;
	posCenter.y = pos.y;

	rectBody.left = (LONG)posCenter.x - (bodySize.x / 2);
	rectBody.top = (LONG)posCenter.y - (bodySize.y / 2);
	rectBody.right = rectBody.left + bodySize.x;
	rectBody.bottom = rectBody.top + bodySize.y;
}

void GameObject::Paint(HDC hdc, const RECT* rectImage)
{
	image->Paint(hdc, rectBody, rectImage);
	if (isShowHitbox == true)
	{
		FrameRect(hdc, &rectBody, (HBRUSH)GetStockObject(WHITE_BRUSH));
	}
}

RECT GameObject::GetRectBody(POINT pos) const
{
	RECT rectBody = { 0, };
	rectBody.left = pos.x - (bodySize.x / 2);
	rectBody.right = rectBody.left + bodySize.x;
	rectBody.top = pos.y - (bodySize.y / 2);
	rectBody.bottom = rectBody.top + bodySize.y;
	return rectBody;
}

bool GameObject::IsCollide(const RECT& rectSrc) const
{
	RECT notuse;
	return (bool)IntersectRect(&notuse, &rectBody, &rectSrc);
}


Vector2 Lerp(Vector2 src, Vector2 dst, float alpha)
{
	Vector2 transform;
	transform.x = (src.x * (1 - alpha)) + (dst.x * alpha);
	transform.y = (src.y * (1 - alpha)) + (dst.y * alpha);
	return transform;
}

void CalculateDamage(float& damage, Type destType, Type srcType)
{
	switch (destType)
	{
	case Type::Elec:
		switch (srcType)
		{
		case Type::Elec:
			damage /= 0.8f;
			break;
		case Type::Fire:
			damage /= 0.6f;
			break;
		case Type::Water:
			damage *= 1.2f;
			break;
		default:
			assert(0);
			break;
		}
		break;
	case Type::Fire:
		switch (srcType)
		{
		case Type::Fire:
			damage /= 0.8f;
			break;
		case Type::Water:
			damage /= 0.6f;
			break;
		case Type::Elec:
			damage *= 1.2f;
			break;
		default:
			assert(0);
			break;
		}
		break;
	case Type::Water:
		switch (srcType)
		{
		case Type::Water:
			damage /= 0.8f;
			break;
		case Type::Elec:
			damage /= 0.6f;
			break;
		case Type::Fire:
			damage *= 1.2f;
			break;
		default:
			assert(0);
			break;
		}
		break;
	default:
		assert(0);
		break;
	}
}