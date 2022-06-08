#include "stdafx.h"
#include "interface.h"
#include "enemy.h"
#include "player.h"
#include "timer.h"
#include "effect.h"
#include "boss.h"

extern Player* player;
extern EnemyController* enemies;
extern Boss* boss;
extern EffectManager* effects;
extern GUIManager* gui;

void CALLBACK T_Battle_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
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

void CALLBACK T_Battle_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Animate();
	enemies->Animate();
	boss->AnimateSkill();
}

void CALLBACK T_Battle_AnimateBoss(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	boss->Animate();
}

void CALLBACK T_Battle_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	player->Move(hWnd, TIMERID_BATTLE_MOVE_PLAYER);
}

void CALLBACK T_Battle_Effect(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	effects->Animate();
}
void CALLBACK T_Battle_GUI(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	gui->Update();
}