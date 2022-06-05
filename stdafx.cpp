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

void GetRotationPos(const RECT& rect, const Vector2& unitVector, Vector2 vPoints[4])
{
	int rotationDir = unitVector.x > 0 ? 1 : -1;
	float theta = Vector2::GetTheta(unitVector, Vector2::Up());
	float rotationDegree = RADIAN_TO_DEGREE(theta) * rotationDir;

	Vector2 posCenter;
	posCenter.x = rect.left + ((float)(rect.right - rect.left) / 2);
	posCenter.y = rect.top + ((float)(rect.bottom - rect.top) / 2);

	vPoints[0] = { (float)rect.left, (float)rect.top };
	vPoints[1] = { (float)rect.right, (float)rect.top };
	vPoints[2] = { (float)rect.left, (float)rect.bottom };
	vPoints[3] = { (float)rect.right, (float)rect.bottom };

	Vector2 directionVec[4] = { 0, };
	float distance[4] = { 0, };
	for (int i = 0; i < 4; ++i)
	{
		directionVec[i] = (vPoints[i] - posCenter).Normalized();
		distance[i] = (vPoints[i] - posCenter).Norm();

		directionVec[i] = Rotate(directionVec[i], rotationDegree);
		vPoints[i] = posCenter + (directionVec[i] * distance[i]);
	}
}

RECT GetRotatedBody(Vector2 vPoints[4])
{
	POINT points[4] = { vPoints[0], vPoints[1], vPoints[2], vPoints[3]};

	RECT rectBody = { 0, };
	rectBody.left = points[0].x;
	rectBody.right = points[0].x;
	rectBody.top = points[0].y;
	rectBody.bottom = points[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (rectBody.left > points[i].x)
		{
			rectBody.left = points[i].x;
		}
		else if (rectBody.right < points[i].x)
		{
			rectBody.right = points[i].x;
		}
		if (rectBody.top > points[i].y)
		{
			rectBody.top = points[i].y;
		}
		else if (rectBody.bottom < points[i].y)
		{
			rectBody.bottom = points[i].y;
		}
	}

	return rectBody;
}

void PaintHitbox(HDC hdc, RECT rectBody)
{
	FrameRect(hdc, &rectBody, (HBRUSH)GetStockObject(BLACK_BRUSH));
}