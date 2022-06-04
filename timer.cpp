#include "stdafx.h"
#include "timer.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "interface.h"

extern EnemyController* enemies;
void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	InvalidateRect(hWnd, NULL, FALSE);
	enemies->CreateMelee();
	enemies->CreateRange();
	enemies->CheckAtkDelay();
}

extern Player* player;
extern EffectManager* effects;
extern GUIManager* gui;

void CALLBACK T_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Animate();
	enemies->Animate();
}

void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Move(hWnd, TIMERID_MOVE_PLAYER);
}

void CALLBACK T_FireBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Fire();
}

void CALLBACK T_MoveObject(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->MoveBullets();
	enemies->MoveBullets();
	enemies->Move();
}

void CALLBACK T_Effect(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	effects->Animate();
}
void CALLBACK T_GUI(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	gui->Update();
}