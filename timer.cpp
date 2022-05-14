#include "timer.h"

void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	InvalidateRect(hWnd, NULL, FALSE);
}

extern FlyPokemon* flyPokemon;

void CALLBACK T_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	flyPokemon->Animate(hWnd);
}

void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	flyPokemon->Move(hWnd, TIMERID_MOVE_PLAYER);
}

void CALLBACK T_ShotBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	flyPokemon->Shot();
}

void CALLBACK T_MoveBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	flyPokemon->MoveBullets();
}