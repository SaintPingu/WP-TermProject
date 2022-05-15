#include "timer.h"

void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	InvalidateRect(hWnd, NULL, FALSE);
}

extern Player* player;

void CALLBACK T_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Animate();
}

void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Move(hWnd, TIMERID_MOVE_PLAYER);
}

void CALLBACK T_ShotBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Shot();
}

void CALLBACK T_MoveBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->MoveBullets();
}