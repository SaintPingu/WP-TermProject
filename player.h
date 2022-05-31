#pragma once
#include "object.h"

class PlayerBullet;

typedef struct PlayerData {
	int hp = 0;
	int speed = 0;
}PlayerData;

class Player : public GameObject, public IControllable, public IAnimatable {
private:
	Vector2 posDest = { 0, };
	Vector2 vectorMove = { 0, };
	float alpha = 0;

	PlayerData data;

	const RECT* rectWindow = nullptr;

	PlayerBullet* bullets = nullptr;

	void SetPosDest() override;
public:
	Player(HWND hWnd, const RECT& rectWindow, ObjectImage& image, float scaleX, float scaleY, Vector2 pos, PlayerData data);
	void Paint(HDC hdc);

	void SetDirection(Dir dir);
	void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) override;
	void Move(HWND hWnd, int timerID) override;
	void Stop(Dir dir) override;
	Vector2 CheckCollideWindow(Vector2 pos) const;

	void Animate() override;
	void Shot();
	void MoveBullets();
	void GetDamage(int damage);
};