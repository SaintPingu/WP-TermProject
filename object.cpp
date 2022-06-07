#include "stdafx.h"
#include "object.h"
#include "interface.h"

extern GameData gameData;

GameObject::GameObject(ObjectImage& image, Vector2 pos)
{
	this->image = &image;
	bodySize = this->image->GetBodySize();

	SetPos(pos);
}
void GameObject::Init(ObjectImage& image, Vector2 pos)
{
	this->image = &image;
	bodySize = this->image->GetBodySize();

	SetPos(pos);
}

void GameObject::SetPos(Vector2 pos)
{
	posCenter.x = pos.x;
	posCenter.y = pos.y;

	rectBody.left = posCenter.x - ((float)bodySize.x / 2);
	rectBody.top = posCenter.y - ((float)bodySize.y / 2);
	rectBody.right = rectBody.left + bodySize.x;
	rectBody.bottom = rectBody.top + bodySize.y;
}

void GameObject::Paint(HDC hdc, const RECT* rectImage)
{
	image->Paint(hdc, rectBody, rectImage);
	
}

FRECT GameObject::GetRectBody(Vector2 pos) const
{
	FRECT rectBody = { 0, };
	rectBody.left = pos.x - ((float)bodySize.x / 2);
	rectBody.right = rectBody.left + bodySize.x;
	rectBody.top = pos.y - ((float)bodySize.y / 2);
	rectBody.bottom = rectBody.top + bodySize.y;
	return rectBody;
}

bool GameObject::IsCollide(const RECT& rectSrc, RECT* lprcDst) const
{
	RECT rect = rectBody;
	if (lprcDst == nullptr)
	{
		RECT notuse = { 0, };
		return (bool)IntersectRect(&notuse, &rect, &rectSrc);
	}
	else
	{
		return (bool)IntersectRect(lprcDst, &rect, &rectSrc);
	}
}


Vector2 Lerp(Vector2 src, Vector2 dst, float alpha)
{
	Vector2 transform;
	transform.x = (src.x * (1 - alpha)) + (dst.x * alpha);
	transform.y = (src.y * (1 - alpha)) + (dst.y * alpha);
	return transform;
}

float CalculateDamage(float damage, Type destType, Type srcType)
{
	switch (destType)
	{
	case Type::Elec:
		switch (srcType)
		{
		case Type::Elec:
			damage /= 1.35f;
			break;
		case Type::Water:
			damage /= 1.25f;
			break;
		case Type::Fire:
			damage *= 1.15f;
			break;
		case Type::Dark:
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
			damage /= 1.35f;
			break;
		case Type::Elec:
			damage /= 1.25f;
			break;
		case Type::Water:
			damage *= 1.15f;
			break;
		case Type::Dark:
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
			damage /= 1.35f;
			break;
		case Type::Fire:
			damage /= 1.25f;
			break;
		case Type::Elec:
			damage *= 1.15f;
			break;
		case Type::Dark:
			break;
		default:
			assert(0);
			break;
		}
		break;
	case Type::Dark:
		break;
	default:
		assert(0);
		break;
	}

	return damage;
}