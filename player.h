#pragma once
#include <Windows.h>
#include "object.h"
#include "timer.h"
#include "bullet.h"

class Player : public GameObject, public IControllable, public IAnimatable {
private:
	POINT posDst = { 0, };
	POINT vector = { 0, };
	double alpha = 0;
	bool isMove = false;

	const RECT* rectWindow = nullptr;

	BulletController* bulletController = nullptr;

	virtual void SetPosDest();
public:
	Player(HWND hWnd, const RECT& rectWindow, ObjectImage image, double scaleX, double scaleY, POINT pos = { 0, 0 });
	void Paint(HDC hdc);

	void SetDirection(Dir dir);
	void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc);
	void Move(HWND hWnd, int timerID);
	void Stop(Dir dir);

	void Animate();
	void Shot();
	void MoveBullets();

	inline bool IsMove() const
	{
		return isMove;
	}
};