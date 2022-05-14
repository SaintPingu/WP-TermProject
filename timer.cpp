#include "timer.h"

void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	InvalidateRect(hWnd, NULL, FALSE);
}

extern FlyPokemon* flyPokemon;
void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	flyPokemon->Move(hWnd, TIMERID_MOVE_PLAYER);
}