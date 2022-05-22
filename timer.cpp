#include "stdafx.h"
#include "timer.h"
#include "player.h"
#include "enemy.h"

void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	InvalidateRect(hWnd, NULL, FALSE);
}

extern Player* player;
extern EnemyController* enemies;

void CALLBACK T_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Animate();
	enemies->Animate();
}

void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Move(hWnd, TIMERID_MOVE_PLAYER);
}

void CALLBACK T_ShotBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Shot();
}

void CALLBACK T_MoveObject(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->MoveBullets();
	enemies->Move();
}
void CALLBACK T_CreateEnemy(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	enemies->CreateMelee();
}