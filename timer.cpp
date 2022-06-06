#include "stdafx.h"
#include "timer.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "interface.h"
#include "boss.h"

extern Player* player;
extern EnemyController* enemies;
extern Boss* boss;
extern EffectManager* effects;
extern GUIManager* gui;
void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	InvalidateRect(hWnd, NULL, FALSE);
	player->CheckShot();
	enemies->CreateCheckMelee();
	enemies->CreateCheckRange();
	enemies->CheckAttackDelay();
	boss->CheckActDelay();
	boss->CheckAttackDelay();

	player->MoveBullets();
	enemies->MoveBullets();
	enemies->Move();
	boss->Move();
}

void CALLBACK T_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Animate();
	enemies->Animate();
	boss->AnimateSkill();
}

void CALLBACK T_AnimateBoss(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	boss->Animate();
}

void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Move(hWnd, TIMERID_MOVE_PLAYER);
}

void CALLBACK T_Effect(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	effects->Animate();
}
void CALLBACK T_GUI(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	gui->Update();
}