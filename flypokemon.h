#pragma once
#include <Windows.h>
#include "object.h"
#include "timer.h"

class FlyPokemon : public GameObject, public IMovable {
private:
	Dir direction = Dir::Empty;
	POINT posDst = { 0, };
	POINT vector = { 0, };

	double alpha = 0;
	bool isMove = false;

	virtual void SetPosDest();
	POINT Lerp(POINT src, POINT dst, double alpha);
public:
	FlyPokemon(HWND hWnd, ObjectImage image, double scaleX, double scaleY, POINT pos = { 0, 0 });
	void Paint(HDC hdc);

	void SetDirection(Dir dir);
	void SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc);
	void Move(HWND hWnd, int timerID);
	void Stop(HWND hWnd, Dir dir);

	inline bool IsMove() const
	{
		return isMove;
	}
};