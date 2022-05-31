#include "stdafx.h"

Vector2 Rotate(Vector2 vector, float degree)
{
	Vector2 result = { 0, };

	float theta = DEGREE_TO_RADIAN(degree);

	result.x = (vector.x * cos(theta)) - (vector.y * sin(theta));
	result.y = (vector.x * sin(theta)) + (vector.y * cos(theta));

	return result;
}

bool OutOfRange(const RECT& rect, const RECT& rectRange)
{
	if (rect.right < rectRange.left || rect.bottom < rectRange.top || rect.left > rectRange.right || rect.top > rectRange.bottom)
	{
		return true;
	}

	return false;
}