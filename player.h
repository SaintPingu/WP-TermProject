#pragma once
#include <Windows.h>
#include "object.h"
#include "timer.h"
#include "bullet.h"

class Player : public GameObject, public IControllable, public IAnimatable {
private:
	Vector2 posDest = { 0, };
	Vector2 vectorMove = { 0, };
	double alpha = 0;

	const RECT* rectWindow = nullptr;

	BulletController* bulletController = nullptr;

	void SetVectorDest() override;
public:
	Player(HWND hWnd, const RECT& rectWindow, ObjectImage image, double scaleX, double scaleY, Vector2 pos = { 0, 0 });
	void Paint(HDC hdc);

	void SetDirection(Dir dir);
	void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc) override;
	void Move(HWND hWnd, int timerID) override;
	void Stop(Dir dir) override;

	void Animate() override;
	void Shot();
	void MoveBullets();
};