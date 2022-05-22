#pragma once
#include "object.h"

class BulletController;

class Player : public GameObject, public IControllable, public IAnimatable {
private:
	Vector2 posDest = { 0, };
	Vector2 vectorMove = { 0, };
	float alpha = 0;

	const RECT* rectWindow = nullptr;

	BulletController* bulletController = nullptr;

	void SetPosDest() override;
public:
	Player(HWND hWnd, const RECT& rectWindow, ObjectImage& image, float scaleX, float scaleY, Vector2 pos);
	void Paint(HDC hdc);

	void SetDirection(Dir dir);
	void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) override;
	void Move(HWND hWnd, int timerID) override;
	void Stop(Dir dir) override;
	Vector2 CheckCollideWindow(Vector2 pos) const;

	void Animate() override;
	void SetRectImage(int frame);
	void Shot();
	void MoveBullets();
};