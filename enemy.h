#pragma once
#include "object.h"
#include "player.h"
#include "timer.h"

class Player;
struct Vector2;

class Enemy : public GameObject, public IAnimatable, public IMovable {
private:
	const Player* player;
	Vector2 posDest = { 0, };

	void SetVectorDest() override;
public:
	Enemy(const Player& player, ObjectImage image, double scaleX, double scaleY, Vector2 pos = { 0, 0 });
	void Move() override;
	void Stop() override;

	void Animate() override;
};