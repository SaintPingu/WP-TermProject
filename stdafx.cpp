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

void GetRotationPos(const RECT& rect, const Vector2& unitVector, Vector2 vPoints[3])
{
	float theta = Vector2::GetTheta(unitVector, Vector2::Forward());
	float rotationDegree = RADIAN_TO_DEGREE(theta);

	Vector2 posCenter;
	posCenter.x = rect.left + ((float)(rect.right - rect.left) / 2);
	posCenter.y = rect.top + ((float)(rect.bottom - rect.top) / 2);

	vPoints[0] = { (float)rect.left, (float)rect.top };
	vPoints[1] = { (float)rect.right, (float)rect.top };
	vPoints[2] = { (float)rect.left, (float)rect.bottom };

	Vector2 directionVec[3] = { 0, };
	float distance[3] = { 0, };
	for (int i = 0; i < 3; ++i)
	{
		directionVec[i] = (vPoints[i] - posCenter).Normalized();
		distance[i] = (vPoints[i] - posCenter).Norm();

		directionVec[i] = Rotate(directionVec[i], rotationDegree);
		vPoints[i] = posCenter + (directionVec[i] * distance[i]);
	}
}