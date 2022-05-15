#pragma once
#include "object.h"
#include "player.h"
#include "timer.h"

class Player;

class Enemy : public GameObject, public IAnimatable, public IMovable {
private:
	const Player* player;
	POINT posDst = { 0, };

	void SetPosDest() override;
public:
	Enemy(const Player& player, ObjectImage image, double scaleX, double scaleY, POINT pos = { 0, 0 });
	void Move() override;
	void Stop() override;

	void Animate() override;
};