#pragma once
#include "object.h"
#include "timer.h"

class Enemy : public GameObject, public IAnimatable, public IMovable {
private:
	POINT posDst = { 0, };

	bool isMove = false;
public:
};